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
 * @date 07/12/2017
 * @version 0.1.0
 * @file CDP.h
 * @addtogroup Material-3D
 * @{
 */

#ifndef CDP_H
#define CDP_H

#include <Material/Material3D/Material3D.h>

class CDP : public Material3D {
    static const double sqrt_three_over_two;
    const double elastic_modulus, poissons_ratio, shear_modulus, double_shear, bulk_modulus;
    const double f_t, a_t, cb_t, g_t, f_c, a_c, cb_c, g_c;
    const double alpha, alpha_p;
    const double factor_a, factor_b, factor_c, tolerance;

    const vec unit_alpha_p;

    vec trial_plastic_strain;
    vec current_plastic_strain;

    mat inv_stiffness;

    static vec compute_backbone(const double, const double, const double, const double);
    static vec compute_d_weight(const vec&);
    static double compute_weight(const vec&);

public:
    explicit CDP(const unsigned = 0, // tag
        const double = 3E4,          // elastic modulus
        const double = .2,           // poissons ratio
        const double = 3.,           // crack stress (+)
        const double = 30.,          // crush stress (-)
        const double = 50E-5,        // normalized crack energy (+)
        const double = 50E-3,        // normalized crush energy (+)
        const double = .2,           // hardening after crack stress a_t
        const double = 2.5,          // hardening after crush stress a_c
        const double = .5,           // reference damage factor at half crack stress
        const double = .7,           // reference damage factor at crush stress
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
