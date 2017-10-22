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
 * @class Damper01
 * @brief A Damper01 class.
 * @author T
 * @date 19/10/2017
 * @file Damper01.h
 * @addtogroup Material-1D
 * @{
 */

#ifndef DAMPER01_H
#define DAMPER01_H

#include <Material/Material1D/Material1D.h>
#include <Solver/ODE_Solver/ODE.h>

class Damper01 : public Material1D {
    const double* current_time_anchor = nullptr;
    const double* incre_time_anchor = nullptr;
    const double* trial_time_anchor = nullptr;

    struct Damper : ODE {
        const double damping, alpha;
        Damper(const double, const double);
        vec eval(const double, const vec&) override;
    };

    Damper ode_system;

public:
    explicit Damper01(const unsigned = 0, const double = 2E5, const double = 0.);

    void initialize(const shared_ptr<DomainBase>&) override;

    unique_ptr<Material> get_copy() override;

    int update_trial_status(const vec&) override;

    int clear_status() override;
    int commit_status() override;
    int reset_status() override;

    void print() override;
};

#endif

//! @}
