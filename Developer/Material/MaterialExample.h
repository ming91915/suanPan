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
 * @class MaterialExample
 * @brief A MaterialExample material class.
 * @author T
 * @date 23/08/2017
 * @version 0.1.0
 * @file MaterialExample.h
 * @addtogroup Material-1D
 * @ingroup Material
 * @{
 */

#ifndef MATERIALEXAMPLE_H
#define MATERIALEXAMPLE_H

#include <Material/Material.h>

class MaterialExample final : public Material {
    const double elastic_modulus; /**< elastic modulus */
    const double yield_stress;    /**< initial yield stress */
    const double hardening_ratio; /**< hardening ratio */
    const double beta;            /**< isotropic/kinematic hardening factor */
    const double plastic_modulus; /**< plastic modulus */

    const double tolerance;

    double current_back_stress = 0.;
    double current_plastic_strain = 0.;
    double trial_back_stress = 0.;
    double trial_plastic_strain = 0.;

public:
    explicit MaterialExample(const unsigned& = 0, /**< tag */
        const double& = 2E5,                      /**< elastic modulus */
        const double& = 400.,                     /**< initial yield stress */
        const double& = .05,                      /**< hardening ratio */
        const double& = 0.,                       /**< isotropic/kinematic hardening factor */
        const double& = 0.);                      /**< density */

    void initialize(const shared_ptr<DomainBase>&) override;

    unique_ptr<Material> get_copy() override;

    int update_trial_status(const vec&) override;

    int clear_status() override;
    int commit_status() override;
    int reset_status() override;
};

#endif

//! @}
