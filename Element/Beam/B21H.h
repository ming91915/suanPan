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
 * @class B21H
 * @brief The B21H class.
 * @author T
 * @date 11/10/2017
 * @version 0.1.1
 * @file B21H.h
 * @addtogroup Beam
 * @ingroup Element
 * @{
 */

#ifndef B21H_H
#define B21H_H

#include <Element/SectionElement.h>

class B21H final : public SectionElement {
    static const unsigned b_node, b_dof;

    const double hinge_length;

    double length = 0., inclination = 0.;

    vec direction_cosine; /**< direction cosine */

    struct IntegrationPoint {
        double coor, weight;
        unique_ptr<Section> b_section;
        mat strain_mat;
        IntegrationPoint(const double, const double, unique_ptr<Section>&&);
    };

    vector<IntegrationPoint> int_pt, elastic_int_pt;

    mat trans_mat, elastic_local_stiffness;

public:
    B21H(const unsigned,   // tag
        const uvec&,       // node tags
        const unsigned,    // section tag
        const double = .2, // hinge length
        const bool = false // nonliear geometry switch
    );

    void initialize(const shared_ptr<DomainBase>&) override;

    int update_status() override;

    int commit_status() override;
    int clear_status() override;
    int reset_status() override;

    vector<vec> record(const OutputType&) override;

    void print() override;
};

#endif

//! @}
