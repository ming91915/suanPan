#include "Solver.h"
#include <Convergence/Convergence.h>
#include <Domain/Domain.h>
#include <Domain/Workroom.h>

Solver::Solver(const unsigned& T,
    const unsigned& CT,
    const shared_ptr<Domain>& D,
    const shared_ptr<Convergence>& C)
    : Tag(T, CT)
    , database(D)
    , converger(C)
{
}

Solver::~Solver() {}

void Solver::initialize()
{
    if(database == nullptr || converger == nullptr) {
        suanpan_fatal("initialize() needs a valid Domain and Convergence.\n");
        return;
    }

    database->initialize();

    auto& tmp_workroom = database->get_workroom();

    tmp_workroom->set_analysis_type(SUANPAN_STATICS);

    if(band_mat)
        tmp_workroom->enable_band();
    else
        tmp_workroom->disable_band();

    if(symm_mat)
        tmp_workroom->enable_symm();
    else
        tmp_workroom->disable_symm();

    tmp_workroom->initialize();

    converger->set_domain(database);
}

void Solver::setDomain(const shared_ptr<Domain>& D) { database = D; }

const shared_ptr<Domain>& Solver::getDomain() const { return database; }

void Solver::setConvergence(const shared_ptr<Convergence>& C) { converger = C; }

const shared_ptr<Convergence>& Solver::getConvergence() const { return converger; }

void Solver::enable_band() { band_mat = true; }

void Solver::disable_band() { band_mat = false; }

void Solver::enable_symm() { symm_mat = true; }

void Solver::disable_symm() { symm_mat = false; }

int ge_solve(vec& X, mat& A, const vec& B)
{
    X = B;

    auto N = static_cast<int>(A.n_cols);
    auto NRHS = 1;
    auto LDA = N;
    auto LDB = N;
    auto IPIV = new int[N];
    auto INFO = 0;

    dgesv_(&N, &NRHS, A.memptr(), &LDA, IPIV, X.memptr(), &LDB, &INFO);

    delete[] IPIV;

    return INFO;
}

int gb_solve(vec& X, mat& A, const vec& B, const unsigned& L, const unsigned& U)
{
    X = B;

    auto N = static_cast<int>(A.n_cols);
    auto KL = static_cast<int>(L);
    auto KU = static_cast<int>(U);
    auto NRHS = 1;
    auto LDAB = 2 * KL + KU + 1;
    auto LDB = N;
    auto IPIV = new int[N];
    auto INFO = 0;

    suanPan::dgbsv_(
        &N, &KL, &KU, &NRHS, A.memptr(), &LDAB, IPIV, X.memptr(), &LDB, &INFO);

    delete[] IPIV;

    return INFO;
}

int sy_solve(vec& X, mat& A, const vec& B)
{
    X = B;

    auto UPLO = 'L';
    auto N = static_cast<int>(A.n_cols);
    auto NRHS = 1;
    auto LDA = N;
    auto LDB = N;
    auto IPIV = new int[N];
    auto INFO = 0;

    auto LWORK = 4 * N;
    auto WORK = new double[LWORK];

    suanPan::dsysv_(
        &UPLO, &N, &NRHS, A.memptr(), &LDA, IPIV, X.memptr(), &LDB, WORK, &LWORK, &INFO);

    delete[] IPIV;
    delete[] WORK;

    return INFO;
}

int po_solve(vec& X, mat& A, const vec& B)
{
    X = B;

    auto UPLO = 'L';
    auto N = static_cast<int>(A.n_cols);
    auto NRHS = 1;
    auto LDA = N;
    auto LDB = N;
    auto INFO = 0;

    suanPan::dposv_(&UPLO, &N, &NRHS, A.memptr(), &LDA, X.memptr(), &LDB, &INFO);

    return INFO;
}

int pb_solve(vec& X, mat& A, const vec& B)
{
    X = B;

    auto UPLO = 'L';
    auto N = static_cast<int>(A.n_cols);
    auto KD = static_cast<int>(A.n_rows - 1);
    auto NRHS = 1;
    auto LDAB = KD + 1;
    auto LDB = N;
    auto INFO = 0;

    suanPan::dpbsv_(&UPLO, &N, &KD, &NRHS, A.memptr(), &LDAB, X.memptr(), &LDB, &INFO);

    return INFO;
}
