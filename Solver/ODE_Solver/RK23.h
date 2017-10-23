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
 * @class RK23
 * @brief A solver using Runge--Kutta 2/3 solver.
 *
 * The Runge--Kutta order 2/order 3 embedded pair.
 * \f{align}{
 * s_1&=f\left(t_i,w_i\right)\\
 * s_2&=f\left(t_i+h,w_i+hs_1\right)\\
 * s_3&=f\left(t_i+\dfrac{1}{2}h,w_i+\dfrac{1}{4}hs_1+\dfrac{1}{4}hs_2\right)\\
 * z_{i+1}&=w_i+\dfrac{h}{6}\left(s_1+s_2+4s_3\right)\\
 * \f}
 *
 * @author T
 * @date 27/07/2017
 * @version 0.1.1
 * @file RK23.h
 * @addtogroup ODE_Explicit
 * @{
 */

#ifndef RK23_H
#define RK23_H

#include <Solver/ODE_Solver/ODE_Explicit.h>

class RK23 final : public ODE_Explicit {
    vec S1, S2, S3; /**< intermediate variables */
public:
    explicit RK23(const unsigned = 0);

    unique_ptr<ODE_Solver> get_copy() override;

    int update_status() override;

    void print() override;
};

#endif

//! @}
