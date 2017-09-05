#ifdef ARMA_BLAS_CAPITALS

#define arma_dgbmv DGBMV
#define arma_dsymv DSYMV
#define arma_dsbmv DSBMV
#define arma_sspmv SSPMV
#define arma_dspmv DSPMV
#define arma_dspmm DSPMM
#define arma_dgesv DGESV
#define arma_dgetrf DGETRF
#define arma_dgetri DGETRI
#define arma_dgbsv DGBSV
#define arma_dsysv DSYSV
#define arma_dposv DPOSV
#define arma_dpbsv DPBSV
#define arma_dsygvx DSYGVX
#define arma_sspsv SSPSV
#define arma_dspsv DSPSV
#define arma_ssptrf SSPTRF
#define arma_dsptrf DSPTRF
#define arma_ssptri SSPTRI
#define arma_dsptri DSPTRI

#else

#define arma_dgbmv dgbmv
#define arma_dsymv dsymv
#define arma_dsbmv dsbmv
#define arma_sspmv sspmv
#define arma_dspmv dspmv
#define arma_dspmm dspmm
#define arma_dgesv dgesv
#define arma_dgetrf dgetrf
#define arma_dgetri dgetri
#define arma_dgbsv dgbsv
#define arma_dsysv dsysv
#define arma_dposv dposv
#define arma_dpbsv dpbsv
#define arma_dsygvx dsygvx
#define arma_sspsv sspsv
#define arma_dspsv dspsv
#define arma_ssptrf ssptrf
#define arma_dsptrf dsptrf
#define arma_ssptri ssptri
#define arma_dsptri dsptri

#endif

extern "C" {
void arma_fortran(arma_dgbmv)(char* TRANS, int* M, int* N, int* KL, int* KU, double* ALPHA, double* A, int* LDA, double* X, int* INCX, double* BETA, double* Y, int* INCY);

void arma_fortran(arma_dsymv)(char* UPLO, int* N, double* ALPHA, double* A, int* LDA, double* X, int* INCX, double* BETA, double* Y, int* INCY);

void arma_fortran(arma_dsbmv)(char* UPLO, int* N, int* K, double* ALPHA, double* A, int* LDA, double* X, int* INCX, double* BETA, double* Y, int* INCY);

void arma_fortran(arma_sspmv)(char* UPLO, int* N, float* ALPHA, float* AP, float* X, int* INCX, float* BETA, float* Y, int* INCY);

void arma_fortran(arma_dspmv)(char* UPLO, int* N, double* ALPHA, double* AP, double* X, int* INCX, double* BETA, double* Y, int* INCY);

void arma_fortran(arma_dspmm)(char* SIDE, char* UPLO, char* TRAN, int* M, int* N, double* A, double* ALPHA, double* B, int* LDB, double* BETA, double* C, int* LDC);

// LAPACK SOLVER
void arma_fortran(arma_dgesv)(int* N, int* NRHS, double* A, int* LDA, int* IPIV, double* B, int* LDB, int* INFO);

void arma_fortran(arma_dgetrf)(int* M, int* N, double* A, int* LDA, int* IPIV, int* INFO);

void arma_fortran(arma_dgetri)(int* N, double* A, int* LDA, int* IPIV, double* WORK, int* LWORK, int* INFO);

void arma_fortran(arma_dgbsv)(int* N, int* KL, int* KU, int* NRHS, double* AB, int* LDAB, int* IPIV, double* B, int* LDB, int* INFO);

void arma_fortran(arma_dsysv)(char* UPLO, int* N, int* NRHS, double* A, int* LDA, int* IPIV, double* B, int* LDB, double* WORK, int* LWORK, int* INFO);

void arma_fortran(arma_dposv)(char* UPLO, int* N, int* NRHS, double* A, int* LDA, double* B, int* LDB, int* INFO);

void arma_fortran(arma_dpbsv)(char* UPLO, int* N, int* KD, int* NRHS, double* AB, int* LDAB, double* B, int* LDB, int* INFO);

void arma_fortran(arma_dsygvx)(int* ITYPE, char* JOBZ, char* RANGE, char* UPLO, int* N, double* A, int* LDA, double* B, int* LDB, double* VL, double* VU, int* IL, int* IU, double* ABSTOL, int* M, double* W, double* Z, int* LDZ, double* WORK, int* LWORK, int* IWORK, int* IFAIL, int* INFO);

// symmetric matrix stored in packed format
void arma_fortran(arma_sspsv)(char* UPLO, int* N, int* NRHS, float* AP, int* IPIV, float* B, int* LDB, int* INFO);

void arma_fortran(arma_dspsv)(char* UPLO, int* N, int* NRHS, double* AP, int* IPIV, double* B, int* LDB, int* INFO);

void arma_fortran(arma_ssptrf)(char* UPLO, int* N, float* AP, int* IPIV, int* INFO);

void arma_fortran(arma_dsptrf)(char* UPLO, int* N, double* AP, int* IPIV, int* INFO);

void arma_fortran(arma_ssptri)(char* UPLO, int* N, float* AP, int* IPIV, float* WORK, int* INFO);

void arma_fortran(arma_dsptri)(char* UPLO, int* N, double* AP, int* IPIV, double* WORK, int* INFO);
}
