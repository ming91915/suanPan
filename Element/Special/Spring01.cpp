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

#include "Spring01.h"
#include <Domain/DomainBase.h>
#include <Domain/Node.h>
#include <Material/Material.h>

const unsigned Spring01::s_node = 2;
const unsigned Spring01::s_dof = 2;
const unsigned Spring01::s_size = s_dof * s_node;

Spring01::Spring01(const unsigned T, const uvec& NT, const unsigned MT)
    : Element(T, ET_SPRING01, s_node, s_dof, NT, uvec{ MT }) {}

void Spring01::initialize(const shared_ptr<DomainBase>& D) {
    auto& coord_i = node_ptr.at(0).lock()->get_coordinate();
    auto& coord_j = node_ptr.at(1).lock()->get_coordinate();

    length = norm(coord_j - coord_i);

    s_material = suanpan::make_copy(D->get_material(unsigned(material_tag(0))));
}

int Spring01::update_status() {
    const auto& node_i = node_ptr.at(0).lock();
    const auto& node_j = node_ptr.at(1).lock();

    auto& disp_i = node_i->get_trial_displacement();
    auto& disp_j = node_j->get_trial_displacement();

    vec new_position(2);
    new_position(0) = disp_j(0) - disp_i(0);
    new_position(1) = disp_j(1) - disp_i(1);

    new_position += node_j->get_coordinate() - node_i->get_coordinate();

    const auto new_length = norm(new_position);

    const auto elongation = new_length - length;

    // TODO: NEED FURTHER WORK

    if(elongation == 0.) return 0;

    const vec direction_cosine = new_position / new_length;

    s_material->update_trial_status(elongation);

    const auto tmp_a = s_material->get_stiffness().at(0);
    trial_stiffness.zeros(s_size, s_size);
    trial_stiffness(0, 2) = -(trial_stiffness(0, 0) = trial_stiffness(2, 2) = tmp_a * direction_cosine(0) * direction_cosine(0));
    trial_stiffness(1, 3) = -(trial_stiffness(1, 1) = trial_stiffness(3, 3) = tmp_a * direction_cosine(1) * direction_cosine(1));
    trial_stiffness(0, 3) = trial_stiffness(1, 2) = -(trial_stiffness(0, 1) = trial_stiffness(2, 3) = tmp_a * direction_cosine(0) * direction_cosine(1));

    for(auto I = 0; I < 3; ++I)
        for(auto J = I + 1; J < 4; ++J) trial_stiffness(J, I) = trial_stiffness(I, J);

    const auto tmp_b = s_material->get_stress().at(0);
    trial_resistance.zeros(s_size);
    trial_resistance(0) = -(trial_resistance(2) = tmp_b * direction_cosine(0));
    trial_resistance(1) = -(trial_resistance(3) = tmp_b * direction_cosine(1));

    return 0;
}

int Spring01::commit_status() { return 0; }

int Spring01::clear_status() { return 0; }

int Spring01::reset_status() { return 0; }
