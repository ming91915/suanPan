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
 * @class Rectangle1D
 * @brief A Rectangle1D class.
 * @author T
 * @date 13/10/2017
 * @version 0.1.0
 * @file Rectangle1D.h
 * @addtogroup Section-2D
 * @ingroup Section
 * @{
 */

#ifndef RECTANGLE1D_H
#define RECTANGLE1D_H

#include <Section/Section1D/Section1D.h>

class Rectangle1D : public Section1D {
    const double width, height;

    unique_ptr<Material> s_material;

public:
    explicit Rectangle1D(const unsigned, // tag
        const double,                    // width
        const double,                    // height
        const unsigned                   // material tag
    );
    Rectangle1D(const Rectangle1D&);

    void initialize(const shared_ptr<DomainBase>&) override;

    unique_ptr<Section> get_copy() override;

    double get_parameter(const ParameterType&) override;

    int update_trial_status(const vec&) override;

    int clear_status() override;
    int commit_status() override;
    int reset_status() override;

    void print() override;
};

#endif

//! @}
