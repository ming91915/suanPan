////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2017 Theodore Chang
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////

#include "Circle.h"
#include <Toolbox/IntegrationPlan.h>

Circle::Circle(const unsigned& T, const double& R, const unsigned& M, const unsigned& S)
    : Section(T, ST_CIRCLE, M)
    , radius(R)
    , int_pt_num(S) {}

void Circle::initialize(const shared_ptr<DomainBase>& D) {
    auto& material_proto = D->get_material(material_tag);

    const IntegrationPlan plan(1, int_pt_num, IntegrationType::GAUSS);

    int_pt.clear();
    int_pt.reserve(int_pt_num);
    for(unsigned I = 0; I < int_pt_num; ++I) int_pt.emplace_back(radius * plan(I, 0), plan(I, 1), material_proto->get_copy());

    resistance.zeros(2);
    stiffness.zeros(2, 2);
    initial_stiffness.zeros(2, 2);
}

unique_ptr<Section> Circle::get_copy() { return make_unique<Circle>(*this); }

int Circle::update_status(const vec& t_strain) {
    stiffness.zeros();

    auto code = 0;
    for(const auto& I : int_pt) {
        code += I.s_material->update_trial_status(vec{ t_strain(0) - t_strain(1) * I.coor });
        const auto tmp_a = I.weight * 2. * sqrt(radius * radius - I.coor * I.coor) * radius;
        const auto tmp_b = I.s_material->get_stiffness().at(0) * tmp_a;
        stiffness(0, 0) += tmp_b;
        stiffness(1, 1) += tmp_b * I.coor * I.coor;
        const auto tmp_c = I.s_material->get_stress().at(0) * tmp_a;
        resistance(0) += tmp_c;
        resistance(1) -= tmp_c * I.coor;
    }

    stiffness.print("\n");

    return code;
}

int Circle::clear_status() {
    auto code = 0;
    for(const auto& I : int_pt) code += I.s_material->clear_status();
    return code;
}

int Circle::commit_status() {
    auto code = 0;
    for(const auto& I : int_pt) code += I.s_material->commit_status();
    return code;
}

int Circle::reset_status() {
    auto code = 0;
    for(const auto& I : int_pt) code += I.s_material->reset_status();
    return code;
}

void Circle::print() { suanpan_info("A Circle Section.\n"); }
