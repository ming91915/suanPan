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
 * @class Mass
 * @brief The Mass class.
 *
 * @author T
 * @date 06/10/2017
 * @version 0.1.0
 * @file Mass.h
 * @addtogroup Element
 * @{
 */

#ifndef ELEMENTTEMPLATE_H
#define ELEMENTTEMPLATE_H

#include <Element/Element.h>

class Mass : public Element {
    const double mass;
public:
    Mass(const unsigned&, const unsigned&, const double&, const uvec& );

    void initialize(const shared_ptr<DomainBase>&) override;

    int update_status() override;

    int commit_status() override;
    int clear_status() override;
    int reset_status() override;
};

#endif

//! @}
