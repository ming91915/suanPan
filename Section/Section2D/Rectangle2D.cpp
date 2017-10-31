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

#include "Rectangle2D.h"
#include <Domain/DomainBase.h>
#include <Material/Material1D/Material1D.h>
#include <Toolbox/IntegrationPlan.h>

Rectangle2D::IntegrationPoint::IntegrationPoint(const double C, const double W, unique_ptr<Material>&& M)
    : coor(C)
    , weight(W)
    , s_material(move(M)) {}

Rectangle2D::IntegrationPoint::IntegrationPoint(const IntegrationPoint& old_obj)
    : coor(old_obj.coor)
    , weight(old_obj.weight)
    , s_material(old_obj.s_material->get_copy()) {}

Rectangle2D::Rectangle2D(const unsigned T, const double B, const double H, const unsigned M, const unsigned S)
    : Section2D(T, ST_RECTANGLE2D, M)
    , width(B)
    , height(H)
    , int_pt_num(S) {}

void Rectangle2D::initialize(const shared_ptr<DomainBase>& D) {
    area = width * height;

    auto& material_proto = D->get_material(material_tag);

    const IntegrationPlan plan(1, int_pt_num, IntegrationType::LOBATTO);

    int_pt.clear(), int_pt.reserve(int_pt_num);
    initial_stiffness.zeros(2, 2);
    for(unsigned I = 0; I < int_pt_num; ++I) {
        int_pt.emplace_back(.5 * height * plan(I, 0), .5 * width * height * plan(I, 1), material_proto->get_copy());
        const auto tmp_a = int_pt[I].s_material->get_initial_stiffness().at(0) * int_pt[I].weight;
        const auto tmp_b = int_pt[I].coor - eccentricity(0);
        initial_stiffness(0, 0) += tmp_a;
        initial_stiffness(1, 1) += tmp_a * tmp_b * tmp_b;
    }

    current_stiffness = initial_stiffness;
    trial_stiffness = initial_stiffness;
}

unique_ptr<Section> Rectangle2D::get_copy() { return make_unique<Rectangle2D>(*this); }

double Rectangle2D::get_parameter(const ParameterType& P) {
    switch(P) {
    case ParameterType::AREA:
        return area;
    case ParameterType::DENSITY:
        return int_pt.cbegin()->s_material->get_parameter(ParameterType::DENSITY);
    default:
        return 0.;
    }
}

int Rectangle2D::update_trial_status(const vec& t_deformation) {
    trial_deformation = t_deformation;

    trial_stiffness.zeros();
    trial_resistance.zeros();

    auto code = 0;

    for(const auto& I : int_pt) {
        const vec fibre_strain{ trial_deformation(0) - trial_deformation(1) * I.coor };
        code += I.s_material->update_trial_status(fibre_strain);
    }

    if(code != 0) return code;

    for(const auto& I : int_pt) {
        const auto arm = I.coor - eccentricity(0);
        const auto tmp_a = I.s_material->get_stiffness().at(0) * I.weight;
        trial_stiffness(0, 0) += tmp_a;
        trial_stiffness(1, 1) += tmp_a * arm * arm;
        const auto tmp_b = I.s_material->get_stress().at(0) * I.weight;
        trial_resistance(0) += tmp_b;
        trial_resistance(1) -= tmp_b * arm;
    }

    return 0;
}

int Rectangle2D::clear_status() {
    current_deformation.zeros();
    trial_deformation.zeros();
    current_resistance.zeros();
    trial_resistance.zeros();
    current_stiffness = initial_stiffness;
    trial_stiffness = initial_stiffness;
    auto code = 0;
    for(const auto& I : int_pt) code += I.s_material->clear_status();
    return code;
}

int Rectangle2D::commit_status() {
    current_deformation = trial_deformation;
    current_resistance = trial_resistance;
    current_stiffness = trial_stiffness;
    auto code = 0;
    for(const auto& I : int_pt) code += I.s_material->commit_status();
    return code;
}

int Rectangle2D::reset_status() {
    trial_deformation = current_deformation;
    trial_resistance = current_resistance;
    trial_stiffness = current_stiffness;
    auto code = 0;
    for(const auto& I : int_pt) code += I.s_material->reset_status();
    return code;
}

void Rectangle2D::print() { suanpan_info("A Rectangle2D Section.\n"); }
