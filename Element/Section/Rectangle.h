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
 * @class Rectangle
 * @brief A Rectangle class.
 * @author T
 * @date 15/09/2017
 * @version 0.1.0
 * @file Rectangle.h
 */

#ifndef Rectangle_H
#define Rectangle_H

#include <Element/Section/Section.h>

class Rectangle : public Section {
    const double width, height;

public:
    explicit Rectangle(const unsigned& T = 0)
        : Section(T, ST_RECTANGLE)
        , width(0)
        , height(0) {}

    virtual ~Rectangle() {}

    void print() override {}
};

#endif
