/*******************************************************************************
 * Copyright (C) 2017 Theodore Chang
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/
/**
 * @fn RCM
 * @brief A renumber function using RCM algorithm.
 *
 * The function takes both mat and sp_mat.
 *
 * Example Usage:
 *
 * ```cpp
 *     sp_mat test_rcm=sprandn(100000, 100000, 0.00005);
 *     auto R = RCM(test_rcm + test_rcm.t());
 * ```
 *
 * R gives the new numbering order of the orginal symmtric matrix.
 *
 * @author T
 * @date 02/08/2017
 * @version 0.1.2
 * @file RCM.h
 * @addtogroup Utility
 * @{
 */

#ifndef RCM_H
#define RCM_H

#include <suanPan.h>

using std::vector;

uvec RCM(const vector<uvec>&, const uvec&);

template <typename eT> uvec RCM(const SpMat<eT>&);

template <typename eT> uvec RCM(const Mat<eT>&);

#endif

//! @}
