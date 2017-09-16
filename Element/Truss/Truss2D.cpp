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
#include "Truss2D.h"

const unsigned Truss2D::t_node = 2;
const unsigned Truss2D::t_dof = 2;

Truss2D::Truss2D(const unsigned& T)
    : Element(T, ET_TRUSS2D)
    , update_area(false)
    , log_strain(false) {}

Truss2D::Truss2D(const unsigned& T, const uvec& N, const unsigned& M, const double& A, const bool& F, const bool& UA, const bool& LS)
    : Element(T, ET_TRUSS2D, t_node, t_dof, N, uvec{ M }, F)
    , area(A)
    , direction_cosine(2)
    , update_area(UA)
    , log_strain(LS) {}

void Truss2D::initialize(const shared_ptr<DomainBase>& D) {
    auto& coord_i = node_ptr.at(0).lock()->get_coordinate();
    auto& coord_j = node_ptr.at(1).lock()->get_coordinate();

    if(coord_i.size() != t_dof || coord_j.size() != t_dof) {
        suanpan_error("initialize() finds incompatible nodes.\n");
        return;
    }

    const vec pos_diff = coord_i - coord_j;

    length = norm(pos_diff);

    t_material = D->get_material(static_cast<unsigned>(material_tag(0)))->get_copy();

    direction_cosine = pos_diff / length;

    const auto tmp_d = area / length * as_scalar(t_material->get_initial_stiffness());
    const auto tmp_a = tmp_d * direction_cosine(0) * direction_cosine(0);
    const auto tmp_b = tmp_d * direction_cosine(1) * direction_cosine(1);
    const auto tmp_c = tmp_d * direction_cosine(0) * direction_cosine(1);

    initial_stiffness(0, 0) = tmp_a;
    initial_stiffness(0, 1) = tmp_c;
    initial_stiffness(0, 2) = -tmp_a;
    initial_stiffness(0, 3) = -tmp_c;

    initial_stiffness(1, 0) = tmp_c;
    initial_stiffness(1, 1) = tmp_b;
    initial_stiffness(1, 2) = -tmp_c;
    initial_stiffness(1, 3) = -tmp_b;

    initial_stiffness(2, 0) = -tmp_a;
    initial_stiffness(2, 1) = -tmp_c;
    initial_stiffness(2, 2) = tmp_a;
    initial_stiffness(2, 3) = tmp_c;

    initial_stiffness(3, 0) = -tmp_c;
    initial_stiffness(3, 1) = -tmp_b;
    initial_stiffness(3, 2) = tmp_c;
    initial_stiffness(3, 3) = tmp_b;
}

int Truss2D::update_status() {
    const auto node_i = node_ptr.at(0).lock();
    const auto node_j = node_ptr.at(1).lock();

    // in a truss-beam system a node may have either 2 or 3 dofs depends on the type of elements connected
    // resize the displacement vectors to make sure they are compatiable with the truss formulation
    auto& disp_i = node_i->get_trial_displacement();
    auto& disp_j = node_j->get_trial_displacement();
    vec disp_diff(2);
    disp_diff(0) = disp_j(0) - disp_i(0);
    disp_diff(1) = disp_j(1) - disp_i(1);

    double trial_strain;

    auto new_area = area;
    auto new_length = length;

    if(nlgeom) {
        disp_diff += node_j->get_coordinate() - node_i->get_coordinate();

        new_length = norm(disp_diff);

        direction_cosine = disp_diff / new_length;

        if(update_area) new_area *= length / new_length;

        trial_strain = log_strain ? log(new_length / length) : new_length / length - 1.;
    } else
        trial_strain = dot(disp_diff, direction_cosine) / new_length;

    t_material->update_trial_status(vec{ trial_strain });

    const auto tmp_d = new_area / new_length * as_scalar(t_material->get_stiffness());
    const auto tmp_a = tmp_d * direction_cosine(0) * direction_cosine(0);
    const auto tmp_b = tmp_d * direction_cosine(1) * direction_cosine(1);
    const auto tmp_c = tmp_d * direction_cosine(0) * direction_cosine(1);

    stiffness(0, 0) = tmp_a;
    stiffness(0, 1) = tmp_c;
    stiffness(0, 2) = -tmp_a;
    stiffness(0, 3) = -tmp_c;
    stiffness(1, 1) = tmp_b;
    stiffness(1, 2) = -tmp_c;
    stiffness(1, 3) = -tmp_b;
    stiffness(2, 2) = tmp_a;
    stiffness(2, 3) = tmp_c;
    stiffness(3, 3) = tmp_b;

    if(nlgeom) {
        const auto tmp_e = new_area / new_length * as_scalar(t_material->get_stress());
        stiffness(0, 0) += tmp_e;
        stiffness(1, 1) += tmp_e;
        stiffness(2, 2) += tmp_e;
        stiffness(3, 3) += tmp_e;
        stiffness(0, 2) -= tmp_e;
        stiffness(1, 3) -= tmp_e;
    }

    for(auto I = 0; I < 3; ++I)
        for(auto J = I + 1; J < 4; ++J) stiffness(J, I) = stiffness(I, J);

    const auto tmp_f = new_area * as_scalar(t_material->get_stress());
    resistance(2) = tmp_f * direction_cosine(0);
    resistance(3) = tmp_f * direction_cosine(1);
    resistance(0) = -resistance(2);
    resistance(1) = -resistance(3);

    return 0;
}

int Truss2D::commit_status() { return t_material->commit_status(); }

int Truss2D::clear_status() { return t_material->clear_status(); }

int Truss2D::reset_status() { return t_material->reset_status(); }

void Truss2D::print() {
    suanpan_info("2-D truss element with ");
    if(nlgeom)
        suanpan_info("corotational formulation, assuming constant %s and %s strain. ", update_area ? "volume" : "area", log_strain ? "logarithmic" : "engineering");
    else
        suanpan_info("linear formulation. ");
    suanpan_info("The nodes connected are\n");
    node_encoding.t().print();
    suanpan_info("The area is %.4E. The initial element length is %.4E.\n", area, length);
    suanpan_info("Material Model: ");
    t_material->print();
}
