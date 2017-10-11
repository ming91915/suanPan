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
 * @class Section
 * @brief A Section class.
 * @author T
 * @date 27/07/2017
 * @version 0.1.0
 * @file Section.h
 */

#ifndef SECTION_H
#define SECTION_H

#include <Material/Material.h>

class Section : public Material {
public:
    explicit Section(const unsigned& T = 0, const unsigned& CT = CT_SECTION)
        : Material(T, CT) {}

    virtual ~Section() {}

    void print() override;
};

#endif
