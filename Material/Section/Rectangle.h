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

#include <Material/Section/Section.h>

class Rectangle : public Section {
    const double width, height;
    const unsigned int_pt_num;

    struct IntegrationPoint {
        double coor, weight;
        unique_ptr<Material> s_material;
        IntegrationPoint(const double C, const double W, unique_ptr<Material>&& M)
            : coor(C)
            , weight(W)
            , s_material(move(M)) {}
        IntegrationPoint(const IntegrationPoint& old_obj)
            : coor(old_obj.coor)
            , weight(old_obj.weight)
            , s_material(old_obj.s_material->get_copy()) {}
    };

    vector<IntegrationPoint> int_pt;

public:
    explicit Rectangle(const unsigned&, const double&, const double&, const unsigned&, const unsigned&);

    void initialize(const shared_ptr<DomainBase>&) override;

    unique_ptr<Section> get_copy() override;

    int update_status(const vec&) override;

    int clear_status() override;
    int commit_status() override;
    int reset_status() override;

    void print() override;
};

#endif
