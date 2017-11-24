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
    const double elastic_modulus, poissons_ratio, shear_modulus, bulk_modulus;
    const double fbfc, alpha, alpha_p, factor_a, factor_b; /**< dilatancy */

    const double peak_stress, peak_strain;
    const double crack_stress, crack_strain;

    const double bar_d_t, a_t, cb_t, bar_d_c, a_c, cb_c;

    const double g_t, g_c;

    double compute_tension_backbone(const double) const;
    double compute_compression_backbone(const double) const;
    double compute_effective_tension_backbone(const double) const;
    double compute_effective_compression_backbone(const double) const;
    double compute_beta(const double, const double) const;
    double compute_ramp_weight(const vec&) const;
    vec compute_ramp_weight_prime(const vec&) const;

public:
    explicit CDP(const unsigned = 0, /**< tag */
        const double = 2E5,          /**< elastic modulus */
        const double = .25,          /**< poisson's ratio */
        const double = 400.,         /**< initial yield stress */
        const double = 0.,           /**< hardening ratio */
        const double = 0.,           /**< isotropic/kinematic hardening factor */
        const double = 0.);          /**< density */

    void initialize(const shared_ptr<DomainBase>& = nullptr) override;

    unique_ptr<Material> get_copy() override;

    int update_trial_status(const vec&) override;

    int clear_status() override;
    int commit_status() override;
    int reset_status() override;
};

#endif

//! @}
