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
 * @date 27/07/2017
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
        const double elastic_modulus, damping, alpha;
        double strain_rate = 0., strain_acceleration = 0.;
        Damper(const double, const double, const double);
        vec eval(const double, const vec&) override;
        void set_strain_rate(const vec&);
        void set_strain_acceleration(const vec&);
    };

    unique_ptr<ODE_Solver> viscous_damper;

public:
    explicit Maxwell(const unsigned, // tag
        const double,                // elastic modulus
        const double,                // damping
        const double                 // alpha
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
