/**
 * @class Solver
 * @brief A Solver class defines solvers used in analysis.
 *
 * As any Solver needs to compute the inverse of a given matrix, which is handle by
 * `solve()` (calls LAPACK) and `spsolve()` (calls SuperLU) depending on different matrix
 * types and Solver configurations, provided by `Armadillo`.
 *
 * By default, Workroom uses sparse matrix for storing data. Correspondingly, Solver uses
 * `spsolve()` for solving the system. The `spsolve()` provides two type flags, which can
 * convert the sparse matrix to dense matrix then call LAPACK subroutines to compute the
 * inverse. This can be controlled by the flag `dense_mat`.
 *
 * Permutation is important for solving speed. Bad permutation may lead to the failure of
 * solving. Two schemes are available: RCM and COLAMD. RCM algorithm is provided and used
 * in Domain object to reorder the DoF indices. So by default, the system is reordered
 * according to RCM algorithm before being sent to Solver. COLAMD algorithm is provided by
 * `SuperLU` library and can be invoked as a parameter when calling `spsolve()` method.
 * Two algorithms lead to different performance for LU decomposition.
 *
 * We will mainly deal with banded matrices so the following tips are tested only for
 * banded matrices:
 * - Sparse matrix is always preferred for all kinds of problems of various sizes with
 * different permutation or even without permutation.
 * - For dense matrix, permutations do not guarantee an improvement about the LU
 * decomposition speed. Sometimes directly solving the original matrix will be faster.
 * - For small problems with sparse storage, RCM seems to be slightly faster than COLAMD.
 * - For large problems with sparse storage, COLAMD is the known best permutation
 * algorithm.
 *
 * So for application, we shall always use sparse storage and call corresponding SuperLU
 * Solver. As RCM is applied by default in Domain and COLAMD is used by default in Solver,
 * for large problems, we do not have to do anything just use default settings. For small
 * problems, we could set `permutation_type=NATURAL` to avoid calling COLAMD every single
 * time, which may cost a significant percentage of the running time.
 *
 * @author T
 * @date 27/07/2017
 * @version 0.2.1
 * @file Solver.h
 * @{
 */

#ifndef SOLVER_H
#define SOLVER_H

namespace suanPan
{
// extern "C" void dgesv_(int* N, int* NRHS, double* A, int* LDA, int* IPIV, double* B,
// int* LDB, int* INFO);
extern "C" void dgbsv_(int* N,
    int* KL,
    int* KU,
    int* NRHS,
    double* AB,
    int* LDAB,
    int* IPIV,
    double* B,
    int* LDB,
    int* INFO);
extern "C" void dsysv_(char* UPLO,
    int* N,
    int* NRHS,
    double* A,
    int* LDA,
    int* IPIV,
    double* B,
    int* LDB,
    double* WORK,
    int* LWORK,
    int* INFO);
extern "C" void dposv_(char* UPLO,
    int* N,
    int* NRHS,
    double* A,
    int* LDA,
    double* B,
    int* LDB,
    int* INFO);
extern "C" void dpbsv_(char* UPLO,
    int* N,
    int* KD,
    int* NRHS,
    double* AB,
    int* LDAB,
    double* B,
    int* LDB,
    int* INFO);
extern "C" void dsygvx_(int* ITYPE,
    char* JOBZ,
    char* RANGE,
    char* UPLO,
    int* N,
    double* A,
    int* LDA,
    double* B,
    int* LDB,
    double* VL,
    double* VU,
    int* IL,
    int* IU,
    double* ABSTOL,
    int* M,
    double* W,
    double* Z,
    int* LDZ,
    double* WORK,
    int* LWORK,
    int* IWORK,
    int* IFAIL,
    int* INFO);
extern "C" void dspsv_(char* UPLO,
    int* N,
    int* NRHS,
    double* AP,
    int* IPIV,
    double* B,
    int* LDB,
    int* INFO);
}

#include <Domain/Tag.h>

class Domain;
class Convergence;

class Solver : public Tag
{
    bool band_mat = false;
    bool symm_mat = false;

    shared_ptr<Domain> database = nullptr;       /**< domain */
    shared_ptr<Convergence> converger = nullptr; /**< convergence */
public:
    explicit Solver(const unsigned& = 0,
        const unsigned& = CT_SOLVER,
        const shared_ptr<Domain>& = nullptr,
        const shared_ptr<Convergence>& = nullptr);
    virtual ~Solver();

    virtual void initialize();

    virtual int updateStatus() = 0;

    virtual int analyze(const double&) = 0;

    void setDomain(const shared_ptr<Domain>&);
    const shared_ptr<Domain>& getDomain() const;

    void setConvergence(const shared_ptr<Convergence>&);
    const shared_ptr<Convergence>& getConvergence() const;

    void enable_band();
    void disable_band();
    void enable_symm();
    void disable_symm();
};

// GENERAL MATRIX
int ge_solve(vec&, mat&, const vec&);
// GENERAL BAND MATRIX
int gb_solve(vec&, mat&, const vec&, const unsigned&, const unsigned&);
// SYMMETRIC MATRIX
int sy_solve(vec&, mat&, const vec&);
// SYMMETRIC POSITIVE DEFINITE MATRIX
int po_solve(vec&, mat&, const vec&);
// SYMMETRIC POSITIVE DEFINITE BAND MATRIX
int pb_solve(vec&, mat&, const vec&);

#endif

//! @}
