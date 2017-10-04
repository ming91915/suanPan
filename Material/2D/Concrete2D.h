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
 * @class Concrete2D
 * @brief A Concrete2D material class.
 * @author T
 * @date 03/10/2017
 * @version 0.1.1
 * @file Concrete2D.h
 * @addtogroup Material-1D
 * @ingroup Material
 * @{
 */

#ifndef CONCRETE2D_H
#define CONCRETE2D_H

#include <Material/Material.h>

class Concrete2D : public Material {
    const unsigned concrete_tag;

    const PlaneType plane_type;

    const double poissons_ratio;

    unique_ptr<Material> concrete_major, concrete_minor;

    double principal_direction = 0.;

public:
    Concrete2D(const unsigned&, // tag
        const unsigned&,        // material tag
        const double&,          // poissons_ratio
        const PlaneType& = PlaneType::S,
        const double& = 0. // density
    );
    Concrete2D(const Concrete2D&);

    void initialize(const shared_ptr<DomainBase>&) override;

    unique_ptr<Material> get_copy() override;

    int update_incre_status(const vec&) override;
    int update_trial_status(const vec&) override;

    int clear_status() override;
    int commit_status() override;
    int reset_status() override;
};

#endif

//! @}
