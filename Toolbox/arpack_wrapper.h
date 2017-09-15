#ifndef ARPACK_WRAPPER_H
#define ARPACK_WRAPPER_H

#include <armadillo>

using namespace arma;

// GENERAL MATRIX
int eig_solve(cx_vec&, cx_mat&, mat&, const unsigned&, const char* = "SM");
int eig_solve(cx_vec&, cx_mat&, mat&, mat&, const unsigned&, const char* = "SM");
int eig_solve(vec&, mat&, mat&, const unsigned&, const char* = "SM");
int eig_solve(vec&, mat&, mat&, mat&, const unsigned&, const char* = "SM");

#endif
