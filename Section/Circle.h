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
 * @class Circle
 * @brief A Circle class.
 * @author T
 * @date 15/09/2017
 * @version 0.1.0
 * @file Circle.h
 * @addtogroup Section
 * @{
 */

#ifndef CIRCLE_H
#define CIRCLE_H

#include <Section/Section.h>

class Circle : public Section {
    const double radius;
    const unsigned int_pt_num;

    struct IntegrationPoint {
        double coor, weight;
        unique_ptr<Material> s_material;
        IntegrationPoint(const double, const double, unique_ptr<Material>&&);
        IntegrationPoint(const IntegrationPoint&);
    };

    vector<IntegrationPoint> int_pt;

public:
    explicit Circle(const unsigned&, // tag
        const double&,               // radius
        const unsigned&,             // material tag
        const unsigned& = 6          // number of integration points
    );

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
