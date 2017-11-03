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
 * @class MPF
 * @brief A 1-D Elastic class.
 * @author T
 * @date 11/08/2017
 * @version 0.1.0
 * @file MPF.h
 * @addtogroup Material-1D
 * @{
 */

#ifndef MPF_H
#define MPF_H

#include <Material/Material1D/Material1D.h>

class MPF final : public Material1D {
    const double elastic_modulus;    // elastic modulus
    const double yield_stress;       // yield stress
    const double hardening_ratio;    // hardening ratio
    const double R0, A1, A2, A3, A4; // model parameters

    const bool isotropic_hardening; // isotropic hardening switch
    const bool constant_radius;     // constant radius switch

    const double yield_strain; // yield strain

public:
    explicit MPF(const unsigned = 0, // tag
        const double = 2E5,          // elastic modulus
        const double = 400.,         // yield stress
        const double = .05,          // hardening ratio
        const double = 20.,          // R0
        const double = 18.5,         // A1
        const double = .15,          // A2
        const double = .01,          // A3
        const double = 7.,           // A4
        const bool = false,          // isotropic hardening switch
        const bool = false,          // constant radius switch
        const double = 0.            // density
    );

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
