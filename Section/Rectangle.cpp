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

#include "Rectangle.h"
#include <Toolbox/IntegrationPlan.h>

Rectangle::Rectangle(const unsigned& T, const double& B, const double& H, const unsigned& M, const unsigned& S)
    : Section(T, ST_RECTANGLE, M)
    , width(B)
    , height(H)
    , int_pt_num(S) {}

void Rectangle::initialize(const shared_ptr<DomainBase>& D) {
    auto& material_proto = D->get_material(material_tag);

    const IntegrationPlan plan(1, int_pt_num, IntegrationType::GAUSS);

    int_pt.clear();
    int_pt.reserve(int_pt_num);
    for(unsigned I = 0; I < int_pt_num; ++I) int_pt.emplace_back(.5 * height * plan(I, 0), .5 * width * height * plan(I, 1), material_proto->get_copy());

    resistance.zeros(2);
    stiffness.zeros(2, 2);
    initial_stiffness.zeros(2, 2);
}

unique_ptr<Section> Rectangle::get_copy() { return make_unique<Rectangle>(*this); }

int Rectangle::update_status(const vec& t_strain) {
    stiffness.zeros();

    auto code = 0;
    for(const auto& I : int_pt) {
        code += I.s_material->update_trial_status(vec{ t_strain(0) - t_strain(1) * I.coor });
        const auto tmp_a = I.s_material->get_stiffness().at(0) * I.weight;
        stiffness(0, 0) += tmp_a;
        stiffness(1, 1) += tmp_a * I.coor * I.coor;
        const auto tmp_b = I.s_material->get_stress().at(0) * I.weight;
        resistance(0) += tmp_b;
        resistance(1) += tmp_b * I.coor;
    }

    return code;
}

int Rectangle::clear_status() {
    auto code = 0;
    for(const auto& I : int_pt) code += I.s_material->clear_status();
    return code;
}

int Rectangle::commit_status() {
    auto code = 0;
    for(const auto& I : int_pt) code += I.s_material->commit_status();
    return code;
}

int Rectangle::reset_status() {
    auto code = 0;
    for(const auto& I : int_pt) code += I.s_material->reset_status();
    return code;
}

void Rectangle::print() { suanpan_info("A Rectangle Section.\n"); }
