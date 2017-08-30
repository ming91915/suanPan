#ifndef LAPACK_WRAPPER_H
#define LAPACK_WRAPPER_H

namespace suanpan
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

extern "C" void sspmv_(char* UPLO,
    int* N,
    float* ALPHA,
    float* AP,
    float* X,
    int* INCX,
    float* BETA,
    float* Y,
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

extern "C" void dgetrf_(int* M, int* N, double* A, int* LDA, int* IPIV, int* INFO);

extern "C" void
dgetri_(int* N, double* A, int* LDA, int* IPIV, double* WORK, int* LWORK, int* INFO);

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

// symmetric matrix stored in packed format
extern "C" void sspsv_(char* UPLO,
    int* N,
    int* NRHS,
    float* AP,
    int* IPIV,
    float* B,
    int* LDB,
    int* INFO);
extern "C" void dspsv_(char* UPLO,
    int* N,
    int* NRHS,
    double* AP,
    int* IPIV,
    double* B,
    int* LDB,
    int* INFO);
extern "C" void ssptrf_(char* UPLO, int* N, float* AP, int* IPIV, int* INFO);
extern "C" void dsptrf_(char* UPLO, int* N, double* AP, int* IPIV, int* INFO);
extern "C" void ssptri_(char* UPLO, int* N, float* AP, int* IPIV, float* WORK, int* INFO);
extern "C" void
dsptri_(char* UPLO, int* N, double* AP, int* IPIV, double* WORK, int* INFO);
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
