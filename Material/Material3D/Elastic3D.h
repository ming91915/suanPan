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
 * @class Elastic3D
 * @brief The Elastic3D class defines a isotropic elastic material for 3-D problems.
 *
 * The Young's modulus is stored in `elastic_modulus`. The Poisson's ratio is stored in `poissons_ratio`.
 *
 * @author T
 * @date 30/07/2017
 * @version 0.1.1
 * @file Elastic3D.h
 * @addtogroup Material-3D
 * @ingroup Material
 * @{
 */

#ifndef Elastic3D_H
#define Elastic3D_H

#include <Material/Material3D/Material3D.h>

class Elastic3D : public Material3D {
    double elastic_modulus; /**< elastic modulus */
    double poissons_ratio;  /**< poissons ratio */

public:
    explicit Elastic3D(const unsigned& = 0, const double& = 2E5, const double& = .2, const double& = 0);

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
