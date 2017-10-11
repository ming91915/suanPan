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
 * @class B21
 * @brief The B21 class.
 * @author T
 * @date 14/09/2017
 * @version 0.1.1
 * @file B21.h
 * @addtogroup Beam
 * @ingroup Element
 * @{
 */

#ifndef B21_H
#define B21_H

#include <Element/Element.h>

class Section;

class B21 final : public Element {
    static const unsigned b_node, b_dof;

    double length = 0.;

    vec direction_cosine; /**< direction cosine */
    double ini_angle = 0.;
    mat strain_mat;

    vector<unique_ptr<Section>> b_section;

    mat local_stiff;

public:
    B21(const unsigned&,    // tag
        const uvec&,        // node tags
        const uvec&,        // section tags
        const bool& = false // nonliear geometry switch
    );

    void initialize(const shared_ptr<DomainBase>&) override;

    int update_status() override;

    int commit_status() override;
    int clear_status() override;
    int reset_status() override;

    void print() override;
};

#endif

//! @}
