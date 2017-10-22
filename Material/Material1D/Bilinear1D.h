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
 * @class Bilinear1D
 * @brief A Bilinear1D material class.
 * @author T
 * @date 08/08/2017
 * @version 0.1.0
 * @file Bilinear1D.h
 * @addtogroup Material-1D
 * @{
 */

#ifndef BILINEAR1D_H
#define BILINEAR1D_H

#include <Material/Material1D/Material1D.h>

class Bilinear1D final : public Material1D {
    const double elastic_modulus; /**< elastic modulus */
    const double yield_stress;    /**< initial yield stress */
    const double hardening_ratio; /**< hardening ratio */
    const double beta;            /**< isotropic/kinematic hardening factor */
    const double plastic_modulus; /**< plastic modulus */

    const double tolerance;

public:
    explicit Bilinear1D(const unsigned& = 0, /**< tag */
        const double& = 2E5,                 /**< elastic modulus */
        const double& = 400.,                /**< initial yield stress */
        const double& = .05,                 /**< hardening ratio */
        const double& = 0.,                  /**< isotropic/kinematic hardening factor */
        const double& = 0.);                 /**< density */

    void initialize(const shared_ptr<DomainBase>&) override;

    unique_ptr<Material> get_copy() override;

    int update_trial_status(const vec&) override;

    int clear_status() override;
    int commit_status() override;
    int reset_status() override;
};

#endif

//! @}
