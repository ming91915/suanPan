#ifdef ARMA_BLAS_CAPITALS

#define arma_sgbmv SGBMV
#define arma_dgbmv DGBMV
#define arma_dsymv DSYMV
#define arma_ssbmv SSBMV
#define arma_dsbmv DSBMV
#define arma_sspmv SSPMV
#define arma_dspmv DSPMV
#define arma_sspmm SSPMM
#define arma_dspmm DSPMM
#define arma_dgesv DGESV
#define arma_dgetrf DGETRF
#define arma_dgetri DGETRI
#define arma_sgbsv SGBSV
#define arma_dgbsv DGBSV
#define arma_dsysv DSYSV
#define arma_dposv DPOSV
#define arma_spbsv SPBSV
#define arma_dpbsv DPBSV
#define arma_dsygvx DSYGVX
#define arma_sspsv SSPSV
#define arma_dspsv DSPSV
#define arma_ssptrf SSPTRF
#define arma_dsptrf DSPTRF
#define arma_ssptri SSPTRI
#define arma_dsptri DSPTRI

#else

#define arma_sgbmv sgbmv
#define arma_dgbmv dgbmv
#define arma_dsymv dsymv
#define arma_ssbmv ssbmv
#define arma_dsbmv dsbmv
#define arma_sspmv sspmv
#define arma_dspmv dspmv
#define arma_sspmm sspmm
#define arma_dspmm dspmm
#define arma_dgesv dgesv
#define arma_dgetrf dgetrf
#define arma_dgetri dgetri
#define arma_sgbsv sgbsv
#define arma_dgbsv dgbsv
#define arma_dsysv dsysv
#define arma_dposv dposv
#define arma_spbsv spbsv
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
void arma_fortran(arma_sgbmv)(const char* TRANS, const int* M, const int* N, const int* KL, const int* KU, const float* ALPHA, const float* A, const int* LDA, const float* X, const int* INCX, const float* BETA, float* Y, const int* INCY);

void arma_fortran(arma_dgbmv)(const char* TRANS, const int* M, const int* N, const int* KL, const int* KU, const double* ALPHA, const double* A, const int* LDA, const double* X, const int* INCX, const double* BETA, double* Y, const int* INCY);

void arma_fortran(arma_dsymv)(char* UPLO, int* N, double* ALPHA, double* A, int* LDA, double* X, int* INCX, double* BETA, double* Y, int* INCY);

void arma_fortran(arma_ssbmv)(const char* UPLO, const int* N, const int* K, const float* ALPHA, const float* A, const int* LDA, const float* X, const int* INCX, const float* BETA, float* Y, const int* INCY);

void arma_fortran(arma_dsbmv)(const char* UPLO, const int* N, const int* K, const double* ALPHA, const double* A, const int* LDA, const double* X, const int* INCX, const double* BETA, double* Y, const int* INCY);

void arma_fortran(arma_sspmv)(const char* UPLO, const int* N, const float* ALPHA, const float* AP, const float* X, const int* INCX, const float* BETA, float* Y, const int* INCY);

void arma_fortran(arma_dspmv)(const char* UPLO, const int* N, const double* ALPHA, const double* AP, const double* X, const int* INCX, const double* BETA, double* Y, const int* INCY);

void arma_fortran(arma_sspmm)(const char* SIDE, const char* UPLO, const char* TRAN, const int* M, const int* N, const float* A, const float* ALPHA, const float* B, const int* LDB, const float* BETA, float* C, const int* LDC);

void arma_fortran(arma_dspmm)(const char* SIDE, const char* UPLO, const char* TRAN, const int* M, const int* N, const double* A, const double* ALPHA, const double* B, const int* LDB, const double* BETA, double* C, const int* LDC);

// LAPACK SOLVER
void arma_fortran(arma_dgesv)(int* N, int* NRHS, double* A, int* LDA, int* IPIV, double* B, int* LDB, int* INFO);

void arma_fortran(arma_dgetrf)(int* M, int* N, double* A, int* LDA, int* IPIV, int* INFO);

void arma_fortran(arma_dgetri)(int* N, double* A, int* LDA, int* IPIV, double* WORK, int* LWORK, int* INFO);

void arma_fortran(arma_sgbsv)(const int* N, const int* KL, const int* KU, const int* NRHS, float* AB, const int* LDAB, int* IPIV, float* B, const int* LDB, int* INFO);

void arma_fortran(arma_dgbsv)(const int* N, const int* KL, const int* KU, const int* NRHS, double* AB, const int* LDAB, int* IPIV, double* B, const int* LDB, int* INFO);

void arma_fortran(arma_dsysv)(char* UPLO, int* N, int* NRHS, double* A, int* LDA, int* IPIV, double* B, int* LDB, double* WORK, int* LWORK, int* INFO);

void arma_fortran(arma_dposv)(char* UPLO, int* N, int* NRHS, double* A, int* LDA, double* B, int* LDB, int* INFO);

void arma_fortran(arma_spbsv)(const char* UPLO, const int* N, const int* KD, const int* NRHS, float* AB, const int* LDAB, float* B, const int* LDB, int* INFO);

void arma_fortran(arma_dpbsv)(const char* UPLO, const int* N, const int* KD, const int* NRHS, double* AB, const int* LDAB, double* B, const int* LDB, int* INFO);

void arma_fortran(arma_dsygvx)(int* ITYPE, char* JOBZ, char* RANGE, char* UPLO, int* N, double* A, int* LDA, double* B, int* LDB, double* VL, double* VU, int* IL, int* IU, double* ABSTOL, int* M, double* W, double* Z, int* LDZ, double* WORK, int* LWORK, int* IWORK, int* IFAIL, int* INFO);

// symmetric matrix stored in packed format
void arma_fortran(arma_sspsv)(const char* UPLO, const int* N, const int* NRHS, float* AP, int* IPIV, float* B, const int* LDB, int* INFO);

void arma_fortran(arma_dspsv)(const char* UPLO, const int* N, const int* NRHS, double* AP, int* IPIV, double* B, const int* LDB, int* INFO);

void arma_fortran(arma_ssptrf)(char* UPLO, int* N, float* AP, int* IPIV, int* INFO);

void arma_fortran(arma_dsptrf)(char* UPLO, int* N, double* AP, int* IPIV, int* INFO);

void arma_fortran(arma_ssptri)(char* UPLO, int* N, float* AP, int* IPIV, float* WORK, int* INFO);

void arma_fortran(arma_dsptri)(char* UPLO, int* N, double* AP, int* IPIV, double* WORK, int* INFO);
}
