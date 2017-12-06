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
 * @class CDP
 * @brief The CDP class.
 * @author T
 * @date 08/08/2017
 * @version 0.1.0
 * @file CDP.h
 * @addtogroup Material-3D
 * @{
 */

#ifndef CDP_H
#define CDP_H

#include <Material/Material3D/Material3D.h>

class CDP : public Material3D {
    static const vec norm_weight;

    const double elastic_modulus, poissons_ratio, shear_modulus, double_shear, bulk_modulus;
    const double alpha, alpha_p;

    const double f_t, f_c, g_t, g_c, a_t, a_c, cb_t, cb_c;

    const double factor_a, factor_b, factor_c, tolerance;

    vec trial_plastic_strain;
    vec current_plastic_strain;

    vec compute_response(const double, const double, const double, const double) const;
    double compute_r(const vec&) const;
    vec compute_d_r(const vec&) const;

public:
    explicit CDP(const unsigned = 0, // tag
        const double = 3E4,          // elastic modulus
        const double = .2,           // poissons ratio
        const double = 5.,           // crack stress (+)
        const double = 50.,          // crush stress (+)
        const double = 5E-4,         // normalized crack energy
        const double = 5E-2,         // normalized crush energy
        const double = .2,           // dilatancy parameter
        const double = 1.16,         // biaxial compression strength ratio
        const double = 2400E-12      // density
    );

    void initialize(const shared_ptr<DomainBase>& = nullptr) override;

    unique_ptr<Material> get_copy() override;

    int update_trial_status(const vec&) override;

    int clear_status() override;
    int commit_status() override;
    int reset_status() override;
};

#endif

//! @}
