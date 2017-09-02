#ifndef LAPACK_WRAPPER_H
#define LAPACK_WRAPPER_H

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
