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
 * @class GeneralizedAlpha
 * @brief A GeneralizedAlpha class defines a solver using GeneralizedAlpha algorithm.
 *
 * Unlike Newmark method, in which the equilibrium is satisfied at the end of current time step, i.e., \f$t=t_0+\Delta{}t\f$, the generalized-\f$\alpha\f$ approach applies it at somewhere in curretn step, i.e., \f$t=t_0+\Delta{}t-\alpha\f$. Similar to the Wilson-\f$\theta\f$ method, or the generalized midpoint concept.
 *
 * @author T
 * @date 21/10/2017
 * @version 0.1.0
 * @file GeneralizedAlpha.h
 */

#ifndef GENERALIZEDALPHA_H
#define GENERALIZEDALPHA_H

#include "Integrator.h"

class GeneralizedAlpha final : public Integrator {
    const double alpha_f; /**< parameter = .25 */
    const double alpha_m;
    const double beta; /**< parameter = .5 */
    const double gamma;

    double DT = 0.; /**< previous incremental time */

    double C0 = 0., C1 = 0., C2 = 0., C3 = 0., C4 = 0., C5 = 0., C6 = 0., C7 = 0., C8 = 0., C9 = 0., C10 = 0., C11 = 0., C12 = 0.; /**< parameters */

    void update_parameter();

public:
    explicit GeneralizedAlpha(const unsigned& = 0, const double& = .25, const double& = .25, const double& = .5);

    void assemble_resistance() override;
    void assemble_matrix() override;

    int process_load() const override;

    void commit_status() const override;

    void print() override;
};

#endif
