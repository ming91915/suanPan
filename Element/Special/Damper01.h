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
 * @class Damper01
 * @brief A Damper01 class.
 * @author T
 * @date 19/10/2017
 * @file Damper01.h
 * @addtogroup Special
 * @ingroup Element
 * @{
 */

#ifndef DAMPER01_H
#define DAMPER01_H

#include <Element/Element.h>

class Damper01 : public Element {
    static unsigned d_node, d_dof, d_size;

    const double damping, alpha;

    double length = 0., new_length = 0.;

    vec direction_cosine;

public:
    explicit Damper01(const unsigned, // tag
        const uvec&,                  // node tags
        const double,                 // damping
        const double                  // alpha
    );

    void initialize(const shared_ptr<DomainBase>&) override;

    int update_status() override;

    int commit_status() override;
    int clear_status() override;
    int reset_status() override;
};

#endif

//! @}
