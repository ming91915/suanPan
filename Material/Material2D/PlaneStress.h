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
 * @class PlaneStress
 * @brief A PlaneStress class.
 * @author T
 * @date 04/10/2017
 * @version 0.1.0
 * @file PlaneStress.h
 * @addtogroup Material-2D
 * @{
 */

#ifndef PLANESTRESS_H
#define PLANESTRESS_H

#include <Material/Material2D/Material2D.h>

using std::array;

class PlaneStress : public Material2D {
    static const array<unsigned, 3> F;

    const unsigned base_tag;

    unique_ptr<Material> base;

    vec current_full_strain;
    vec trial_full_strain;

    static mat form_stiffness(const mat&);

public:
    explicit PlaneStress(const unsigned, /**< tag */
        const unsigned                   /**< 3D material tag */
    );
    PlaneStress(const PlaneStress&);

    void initialize(const shared_ptr<DomainBase>& = nullptr) override;

    double get_parameter(const ParameterType& = ParameterType::DENSITY) const override;

    unique_ptr<Material> get_copy() override;

    int update_trial_status(const vec&) override;

    int clear_status() override;
    int commit_status() override;
    int reset_status() override;
};

#endif

//! @}
