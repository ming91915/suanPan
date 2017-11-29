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
 * @class Spring01
 * @brief The Spring01 class.
 *
 * @author T
 * @date 06/10/2017
 * @version 0.1.0
 * @file Spring01.h
 * @addtogroup Special
 * @ingroup Element
 * @{
 */

#ifndef SPRING01_H
#define SPRING01_H

#include <Element/Element.h>

class Spring01 : public Element {
    static const unsigned s_node, s_dof, s_size;

    double length = 0.;

    unique_ptr<Material> s_material;

public:
    Spring01(const unsigned, // tag
        const uvec&,         // node tags
        const unsigned       // material tags
    );

    void initialize(const shared_ptr<DomainBase>&) override;

    int update_status() override;

    int commit_status() override;
    int clear_status() override;
    int reset_status() override;
};

#endif

//! @}
