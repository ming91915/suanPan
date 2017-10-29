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

#include "Rectangle1D.h"
#include <Domain/DomainBase.h>
#include <Material/Material1D/Material1D.h>

Rectangle1D::Rectangle1D(const unsigned T, const double B, const double H, const unsigned M)
    : Section1D(T, ST_RECTANGLE1D, M)
    , width(B)
    , height(H) {}

Rectangle1D::Rectangle1D(const Rectangle1D& old_obj)
    : Section1D(old_obj.get_tag(), ST_RECTANGLE1D, old_obj.material_tag)
    , width(old_obj.width)
    , height(old_obj.height)
    , s_material(old_obj.s_material->get_copy()) {}

void Rectangle1D::initialize(const shared_ptr<DomainBase>& D) {
    area = width * height;

    auto& material_proto = D->get_material(material_tag);

    if(material_proto->material_type != MaterialType::D1) {
        suanpan_error("section needs uniaxial material.\n");
        D->disable_section(get_tag());
        return;
    }

    s_material = material_proto->get_copy();

    initial_stiffness = area * s_material->get_initial_stiffness().at(0);

    current_stiffness = initial_stiffness;
    trial_stiffness = initial_stiffness;
}

unique_ptr<Section> Rectangle1D::get_copy() { return make_unique<Rectangle1D>(*this); }

double Rectangle1D::get_parameter(const ParameterType& P) {
    switch(P) {
    case ParameterType::AREA:
        return area;
    case ParameterType::DENSITY:
        return s_material->get_parameter(ParameterType::DENSITY);
    default:
        return 0.;
    }
}

int Rectangle1D::update_trial_status(const vec& t_deformation) {
    trial_deformation = t_deformation;

    trial_stiffness.zeros();
    trial_resistance.zeros();

    const auto code = s_material->update_trial_status(t_deformation);

    if(code != 0) return code;

    trial_stiffness = s_material->get_stiffness() * area;

    trial_resistance = s_material->get_stress();

    return 0;
}

int Rectangle1D::clear_status() {
    current_deformation.zeros();
    trial_deformation.zeros();
    current_resistance.zeros();
    trial_resistance.zeros();
    current_stiffness = initial_stiffness;
    trial_stiffness = initial_stiffness;
    return s_material->clear_status();
}

int Rectangle1D::commit_status() {
    current_deformation = trial_deformation;
    current_resistance = trial_resistance;
    current_stiffness = trial_stiffness;
    return s_material->commit_status();
}

int Rectangle1D::reset_status() {
    trial_deformation = current_deformation;
    trial_resistance = current_resistance;
    trial_stiffness = current_stiffness;
    return s_material->reset_status();
}

void Rectangle1D::print() { suanpan_info("A Rectangle1D Section.\n"); }
