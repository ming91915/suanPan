/**
* @fn RCM
* @brief A renumber function using RCM algorithm.
*
* The function takes both mat and sp_mat.
*
* Example Usage:
*
*     sp_mat test_rcm=sprandn(100000, 100000, 0.00005);
*     auto R = RCM(test_rcm + test_rcm.t());
*
* @author T
* @date 02/08/2017
* @version 0.1.2
* @file RCM.h
*/

#ifndef RCM_H
#define RCM_H

#include <suanPan.h>

using std::vector;

uvec RCM(const vector<uvec>&, const uvec&);

template <typename eT>
uvec RCM(const SpMat<eT>&);

template <typename eT>
uvec RCM(const Mat<eT>&);

#endif
