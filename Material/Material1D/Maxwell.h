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
 * @class Maxwell
 * @brief A 1-D Maxwell material class.
 * @author T
 * @date 24/10/2017
 * @file Maxwell.h
 * @addtogroup Material-1D
 * @{
 */

#ifndef MAXWELL_H
#define MAXWELL_H

#include <Material/Material1D/Material1D.h>
#include <Solver/ODE_Solver/ODE.h>

class ODE_Solver;

class Maxwell : public Material1D {
    const double* incre_time = nullptr;

    struct Damper : ODE {
        const double elastic_modulus, alpha, damping_positive, damping_negative;
        double current_strain_rate = 0., current_strain_acceleration = 0.;
        Damper(const double, const double, const double, const double);
        unique_ptr<ODE> get_copy() override;
        vec eval(const double, const vec&) override;
        void set_current_status(const vec&, const vec&);
    };

    unique_ptr<ODE> viscosity;
    unique_ptr<ODE_Solver> solver;

    Damper* damper_ptr = dynamic_cast<Damper*>(viscosity.get());

public:
    explicit Maxwell(const unsigned, // tag
        const double,                // elastic modulus
        const double,                // alpha
        const double,                // damping positive
        const double                 // damping negative
    );
    Maxwell(const Maxwell&);

    void initialize(const shared_ptr<DomainBase>&) override;

    unique_ptr<Material> get_copy() override;

    int update_trial_status(const vec&, const vec&) override;

    int clear_status() override;
    int commit_status() override;
    int reset_status() override;

    void print() override;
};

#endif

//! @}
