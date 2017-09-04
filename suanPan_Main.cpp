#include <suanPan>

void example_symm_mat();
mat spmm(const SymmMat<double>& A, const Mat<double>& X);

int main(int argc, char** argv)
{
#ifndef SUANPAN_DEBUG
    wall_clock T;
    T.tic();
#endif

    // argument_parser(argc, argv);

    Storage<Constraint> A;

    example_symm_mat();

#ifndef SUANPAN_DEBUG
    cout << T.toc() << "\n";
#endif

    return 0;
}

void example_symm_mat()
{
    const auto N = 10;
    mat A(N, N, fill::randn);
    A = A + A.t();
    const symm_mat B(A);

    const mat D(10, 5, fill::randn);

    (A * D).print();
    cout << endl;
    spmm(B, D).print();
    cout << endl;
}

mat spmm(const SymmMat<double>& A, const Mat<double>& X)
{
    auto Y = X;

    auto SIDE = 'R';
    auto UPLO = 'U';
    auto TRAN = 'N';
    auto M = static_cast<int>(A.n_size);
    auto N = static_cast<int>(X.n_cols);
    auto ALPHA = 1.;
    auto LDB = M;
    auto BETA = 0.;
    auto LDC = M;

    arma_fortran(arma_dspmm)(&SIDE, &UPLO, &TRAN, &M, &N, const_cast<double*>(A.memptr()),
        &ALPHA, const_cast<double*>(X.memptr()), &LDB, &BETA, Y.memptr(), &LDC);

    return Y;
};
