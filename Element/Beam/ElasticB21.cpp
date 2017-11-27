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

#include "ElasticB21.h"
#include <Domain/DomainBase.h>
#include <Domain/Node.h>
#include <Material/Material1D/Material1D.h>
#include <Toolbox/tensorToolbox.h>

const unsigned ElasticB21::b_node = 2;
const unsigned ElasticB21::b_dof = 3;

ElasticB21::ElasticB21(const unsigned& T, const uvec& N, const double& A, const double& I, const unsigned& M, const bool& F)
    : MaterialElement(T, ET_ELASTICB21, b_node, b_dof, N, uvec{ M }, F)
    , area(A)
    , moment_inertia(I) {}

void ElasticB21::initialize(const shared_ptr<DomainBase>& D) {
    auto& coord_i = node_ptr.at(0).lock()->get_coordinate();
    auto& coord_j = node_ptr.at(1).lock()->get_coordinate();

    // check size compatibility
    if(coord_i.size() != 2 || coord_j.size() != 2) {
        suanpan_error("initialize() finds incompatible nodes.\n");
        return;
    }

    // chord vector
    const vec pos_diff = coord_j - coord_i;

    length = norm(pos_diff);

    direction_cosine = pos_diff / length;
    inclination = atan2(direction_cosine(1), direction_cosine(0));

    b_material = D->get_material(unsigned(material_tag(0)))->get_copy();

    // stiffness
    const auto& t_modulus = b_material->get_initial_stiffness();
    const auto tmp_a = as_scalar(t_modulus) / length;
    const auto tmp_b = 2. * tmp_a * moment_inertia;
    const auto tmp_c = 2. * tmp_b;

    local_stiff.zeros(3, 3);
    local_stiff(0, 0) = tmp_a * area;
    local_stiff(1, 1) = tmp_c;
    local_stiff(1, 2) = tmp_b;
    local_stiff(2, 1) = tmp_b;
    local_stiff(2, 2) = tmp_c;

    strain_mat = transform::beam::global_to_local(direction_cosine, length);

    initial_stiffness = strain_mat.t() * local_stiff * strain_mat;

    // mass
    trial_mass.zeros();
    const auto density = b_material->get_parameter(ParameterType::DENSITY);
    if(density != 0.) {
        mat trans(6, 6, fill::zeros);
        trans(5, 5) = trans(2, 2) = 1.;
        trans(0, 0) = trans(1, 1) = trans(3, 3) = trans(4, 4) = direction_cosine(0);
        trans(1, 0) = trans(4, 3) = -(trans(0, 1) = trans(3, 4) = direction_cosine(1));

        trial_mass(1, 1) = trial_mass(4, 4) = 156.;
        trial_mass(1, 4) = trial_mass(4, 1) = 54.;
        trial_mass(4, 5) = trial_mass(5, 4) = -(trial_mass(2, 1) = trial_mass(1, 2) = 22. * length);
        trial_mass(2, 4) = trial_mass(4, 2) = -(trial_mass(5, 1) = trial_mass(1, 5) = -13. * length);
        trial_mass(5, 2) = trial_mass(2, 5) = -.75 * (trial_mass(5, 5) = trial_mass(2, 2) = 4. * length * length);
        trial_mass(3, 3) = trial_mass(0, 0) = 2. * (trial_mass(3, 0) = trial_mass(0, 3) = 140.);
        trial_mass *= density * area * length / 420.;

        trial_mass = trans.t() * trial_mass * trans;
    }
}

int ElasticB21::update_status() {
    const auto& node_i = node_ptr.at(0).lock();
    const auto& node_j = node_ptr.at(1).lock();

    auto& disp_i = node_i->get_trial_displacement();
    auto& disp_j = node_j->get_trial_displacement();

    auto new_length = length;

    vec local_deformation(3);

    if(nlgeom) {
        vec disp_diff(2);
        disp_diff(0) = disp_j(0) - disp_i(0);
        disp_diff(1) = disp_j(1) - disp_i(1);

        disp_diff += node_j->get_coordinate() - node_i->get_coordinate();

        new_length = norm(disp_diff);

        direction_cosine = disp_diff / new_length;

        strain_mat = transform::beam::global_to_local(direction_cosine, new_length);

        local_deformation(0) = new_length - length;

        auto tmp_angle = inclination + disp_i(2);
        auto tmp_a = sin(tmp_angle), tmp_b = cos(tmp_angle);

        local_deformation(1) = atan((direction_cosine(0) * tmp_a - direction_cosine(1) * tmp_b) / (direction_cosine(0) * tmp_b + direction_cosine(1) * tmp_a));

        tmp_angle = inclination + disp_j(2);
        tmp_a = sin(tmp_angle), tmp_b = cos(tmp_angle);

        local_deformation(2) = atan((direction_cosine(0) * tmp_a - direction_cosine(1) * tmp_b) / (direction_cosine(0) * tmp_b + direction_cosine(1) * tmp_a));
    } else {
        vec t_disp(6);
        for(auto I = 0; I < 3; ++I) t_disp(I) = disp_i(I), t_disp(I + 3) = disp_j(I);
        local_deformation = strain_mat * t_disp;
    }

    const vec local_force = local_stiff * local_deformation;

    trial_stiffness = strain_mat.t() * local_stiff * strain_mat;
    trial_resistance = strain_mat.t() * local_force;

    if(nlgeom) {
        vec R(6, fill::zeros), Z(6, fill::zeros);
        R(0) = Z(1) = -(R(3) = Z(4) = direction_cosine(0));
        R(1) = Z(3) = -(R(4) = Z(0) = direction_cosine(1));

        const mat tmp_a = R * Z.t();

        trial_geometry = local_force(0) / new_length * Z * Z.t() + (local_force(1) + local_force(2)) / new_length / new_length * (tmp_a + tmp_a.t());
        trial_stiffness += trial_geometry;
    }

    return 0;
}

int ElasticB21::commit_status() {
    current_stiffness = trial_stiffness;
    current_resistance = trial_resistance;
    return b_material->commit_status();
}

int ElasticB21::clear_status() {
    current_stiffness = trial_stiffness = initial_stiffness;
    current_resistance.zeros();
    trial_resistance.zeros();
    return b_material->clear_status();
}

int ElasticB21::reset_status() {
    trial_stiffness = current_stiffness;
    trial_resistance = current_resistance;
    return b_material->reset_status();
}

void ElasticB21::print() { suanpan_info("An elastic B21 element%s", nlgeom ? " with corotational formulation.\n" : ".\n"); }
