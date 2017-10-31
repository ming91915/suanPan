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
 * @class MVLEM
 * @brief The MVLEM class.
 * @author T
 * @date 14/09/2017
 * @version 0.1.1
 * @file MVLEM.h
 * @addtogroup Beam
 * @ingroup Element
 * @{
 */

#ifndef MVLEM_H
#define MVLEM_H

#include <Element/MaterialElement.h>

class MVLEM final : public MaterialElement {
    static const unsigned b_node, b_dof;

    struct Fibre {
        double eccentricity = 0., width, thickness, c_area, s_area;
        unique_ptr<Material> c_material, s_material;
        Fibre(const double, const double, const double);
    };

    double total_area = 0.;

    vector<Fibre> axial_spring;

    const unsigned shear_spring_tag;

    unique_ptr<Material> shear_spring;

public:
    MVLEM(const unsigned, // tag
        const uvec&,      // node tags
        const vec&,       // width
        const vec&,       // thickness
        const vec&,       // reinforcement ratio
        const uvec&,      // concrete material tags
        const uvec&,      // steel material tags
        const unsigned    // shear spring tag
    );

    void initialize(const shared_ptr<DomainBase>&) override;

    int update_status() override;
    int commit_status() override;
    int clear_status() override;
    int reset_status() override;
};

#endif

//! @}
