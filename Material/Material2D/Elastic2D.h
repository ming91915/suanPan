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
 * @class Elastic2D
 * @brief The Elastic2D class defines a isotropic elastic material for
 * plane stress and
 * plane strain problems.
 *
 * The Young's modulus is stored in `elastic_modulus`. The Poisson's ratio is stored in `poissons_ratio`. The `plane_type` labels if it is plane stress or plane strain. The default value `PlaneType::S` represents plane stress. Initializing the object with a `PlaneType::E` value gives a plane strain type response.
 *
 * @author T
 * @date 04/10/2017
 * @version 0.1.2
 * @file Elastic2D.h
 * @addtogroup Material-2D
 * @ingroup Material
 * @{
 */

#ifndef ELASTIC2D_H
#define ELASTIC2D_H

#include <Material/Material2D/Material2D.h>

class Elastic2D : public Material2D {
    const double elastic_modulus; // elastic modulus
    const double poissons_ratio;  // poissons ratio
public:
    explicit Elastic2D(const unsigned& = 0, const double& = 2E5, const double& = .2, const double& = 0, const PlaneType& = PlaneType::S);

    void initialize(const shared_ptr<DomainBase>&) override;

    unique_ptr<Material> get_copy() override;

    int update_incre_status(const vec&) override;
    int update_trial_status(const vec&) override;

    int clear_status() override;
    int commit_status() override;
    int reset_status() override;

    void print() override;

    double get_parameter(const ParameterType& = ParameterType::DENSITY) const override;
};

#endif

//! @}
