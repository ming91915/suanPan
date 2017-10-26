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
 * @class DP45
 * \brief A solver using Dormand--Prince 4/5 pair.
 *
 * The Dormand--Prince order 4/order 5 embedded pair. The default solver in `ode45` function in MatLab.
 * \f{align}{
 * s_1&=f\left(t_i,w_i\right)\\
 * s_2&=f\left(t_i+\dfrac{1}{5}h,w_i+\dfrac{1}{5}hs_1\right)\\
 * s_3&=f\left(t_i+\dfrac{3}{10}h,w_i+\dfrac{3}{40}hs_1+\dfrac{9}{40}hs_2\right)\\
 * s_4&=f\left(t_i+\dfrac{4}{5}h,w_i+\dfrac{44}{45}hs_1-\dfrac{56}{15}hs_2+\dfrac{32}{9}hs_3\right)\\
 * s_5&=f\left(t_i+\dfrac{8}{9}h,w_i+\dfrac{19372}{6561}hs_1-\dfrac{25360}{2187}hs_2+\dfrac{64448}{6561}hs_3-\dfrac{212}{729}hs_4\right)\\
 * s_6&=f\left(t_i+h,w_i+\dfrac{9017}{3168}hs_1-\dfrac{355}{33}hs_2+\dfrac{46732}{5247}hs_3+\dfrac{49}{176}hs_4-\dfrac{5103}{18656}hs_5\right)\\
 * z_{i+1}&=w_i+h\left(\dfrac{35}{384}s_1+\dfrac{500}{1113}s_3+\dfrac{125}{192}s_4-\dfrac{2187}{6784}s_5+\dfrac{11}{84}s_6\right)\\
 * s_7&=f\left(t_i+h,z_{i+1}\right)
 * \f}
 *
 * @author T
 * @date 27/07/2017
 * @version 0.1.1
 * @file DP45.h
 * @addtogroup ODE_Explicit
 * @{
 */

#ifndef DP45_H
#define DP45_H

#include <Solver/ODE_Solver/ODE_Explicit.h>

class DP45 final : public ODE_Explicit {
    static const std::array<double, 29> F; /**< constants array */
    vec S1, S2, S3, S4, S5, S6, S7;        /**< intermediate variables */
public:
    explicit DP45(const unsigned = 0, ODE* = nullptr);

    unique_ptr<ODE_Solver> get_copy() override;

    int update_status() override;

    void print() override;
};

#endif

//! @}
