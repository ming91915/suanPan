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
 * @class CSMM
 * @brief A CSMM material class.
 * @author T
 * @date 31/10/2017
 * @version 0.1.0
 * @file CSMM.h
 * @addtogroup Material-2D
 * @{
 */

#ifndef CSMM_H
#define CSMM_H

#include <Material/Material2D/Material2D.h>

class CSMM : public Material2D {
    static const double crack_strain;

    const double peak_strain;
    const double peak_stress;

    const double crack_stress;

    const double initial_modulus;

    double concrete_angle = datum::pi / 4.;

    const double steel_modulus;
    const double yield_stress;

    const double yield_strain = yield_stress / steel_modulus;

public:
    explicit CSMM(const unsigned, // tag
        const double,
        const double);

    void initialize(const shared_ptr<DomainBase>&) override;

    unique_ptr<Material> get_copy() override;

    int update_trial_status(const vec&) override;

    int clear_status() override;
    int commit_status() override;
    int reset_status() override;
};

#endif

//! @}
