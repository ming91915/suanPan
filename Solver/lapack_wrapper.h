#ifndef LAPACK_WRAPPER_H
#define LAPACK_WRAPPER_H

namespace suanPan
{
// BLAS
extern "C" void dgbmv_(char* TRANS,
    int* M,
    int* N,
    int* KL,
    int* KU,
    double* ALPHA,
    double* A,
    int* LDA,
    double* X,
    int* INCX,
    double* BETA,
    double* Y,
    int* INCY);

extern "C" void dsymv_(char* UPLO,
    int* N,
    double* ALPHA,
    double* A,
    int* LDA,
    double* X,
    int* INCX,
    double* BETA,
    double* Y,
    int* INCY);

extern "C" void dsbmv_(char* UPLO,
    int* N,
    int* K,
    double* ALPHA,
    double* A,
    int* LDA,
    double* X,
    int* INCX,
    double* BETA,
    double* Y,
    int* INCY);

extern "C" void dspmv_(char* UPLO,
    int* N,
    double* ALPHA,
    double* AP,
    double* X,
    int* INCX,
    double* BETA,
    double* Y,
    int* INCY);

// LAPACK SOLVER
extern "C" void
dgesv_(int* N, int* NRHS, double* A, int* LDA, int* IPIV, double* B, int* LDB, int* INFO);

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

#include <armadillo>

using namespace arma;

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
