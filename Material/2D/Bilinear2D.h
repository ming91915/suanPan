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

#pragma once
#include <Material/3D/Bilinear3D.h>

class Bilinear2D : public Material {
    const unsigned material_type;

    vec trial_full_strain;

    Bilinear3D base;

public:
    explicit Bilinear2D(const unsigned& = 0, /**< tag */
        const double& = 2E5,                 /**< elastic modulus */
        const double& = .25,                 /**< poisson's ratio */
        const double& = 400.,                /**< initial yield stress */
        const double& = .05,                 /**< hardening ratio */
        const double& = 0.,                  /**< isotropic/kinematic hardening factor */
        const unsigned& = 0,                 /**< plane stress or plane strain */
        const double& = 0.                   /**< density */
    );

    void initialize(const shared_ptr<DomainBase>&) override;

    double get_parameter(const unsigned& = 0) const override;

    unique_ptr<Material> get_copy() override;

    int update_incre_status(const vec&) override;
    int update_trial_status(const vec&) override;

    int clear_status() override;
    int commit_status() override;
    int reset_status() override;
};
