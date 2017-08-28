#ifndef ARPACK_WRAPPER_H
#define ARPACK_WRAPPER_H

namespace suanpan
{
// ARPACK
extern "C" void dsaupd_(int* IDO,
    char* BMAT,
    int* N,
    char* WHICH,
    int* NEV,
    double* TOL,
    double* RESID,
    int* NCV,
    double* V,
    int* LDV,
    int* IPARAM,
    int* IPNTR,
    double* WORKD,
    double* WORKL,
    int* LWORKL,
    int* INFO);
// extern "C" void dseupd_(bool* RVEC,
//    char* HOWMNY,
//    bool* SELECT,
//    double* D,
//    double* Z,
//    int* LDZ,
//    double* SIGMA,
//    char* BMAT,
//    int* N,
//    char* WHICH,
//    int* NEV,
//    double* TOL,
//    double* RESID,
//    int* NCV,
//    double* V,
//    int* LDV,
//    int* IPARAM,
//    int* IPNTR,
//    double* WORKD,
//    double* WORKL,
//    int* LWORKL,
//    int* INFO);
}

#include <armadillo>

using namespace arma;

// GENERAL MATRIX
int eig_solve(vec&, mat&, mat&, mat&, const unsigned&, const char* = "sm");

#endif
