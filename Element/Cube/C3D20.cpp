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

#include "C3D20.h"
#include <Domain/DomainBase.h>
#include <Domain/Node.h>
#include <Material/Material3D/Material3D.h>
#include <Toolbox/IntegrationPlan.h>
#include <Toolbox/shapeFunction.hpp>

const unsigned C3D20::c_node = 20;
const unsigned C3D20::c_dof = 3;
const unsigned C3D20::c_size = c_dof * c_node;

C3D20::IntegrationPoint::IntegrationPoint(const vec& C, const double W, const double J, unique_ptr<Material>&& M)
    : coor(C)
    , weight(W)
    , jacob_det(J)
    , c_material(move(M))
    , strain_mat(6, c_size, fill::zeros) {}

C3D20::C3D20(const unsigned& T, const uvec& N, const unsigned& M, const bool& R, const bool& F)
    : MaterialElement(T, ET_C3D20, c_node, c_dof, N, uvec{ M }, F)
    , reduced_scheme(R) {}

void C3D20::initialize(const shared_ptr<DomainBase>& D) {
    mat ele_coor(c_node, c_dof);
    for(unsigned I = 0; I < c_node; ++I) {
        auto& tmp_coor = node_ptr[I].lock()->get_coordinate();
        for(unsigned J = 0; J < c_dof; ++J) ele_coor(I, J) = tmp_coor(J);
    }

    const auto& material_proto = D->get_material(unsigned(material_tag(0)));

    const IntegrationPlan plan(3, reduced_scheme ? 2 : 3, IntegrationType::IRONS);

    int_pt.clear(), int_pt.reserve(plan.n_rows);
    for(unsigned I = 0; I < plan.n_rows; ++I) {
        const vec t_vec(std::initializer_list<double>{ plan(I, 0), plan(I, 1), plan(I, 2) });
        const auto pn = shape::cube(t_vec, 1, 20);
        const mat jacob = pn * ele_coor;
        const mat pn_pxy = solve(jacob, pn);
        int_pt.emplace_back(t_vec, plan(I, c_dof), det(jacob), material_proto->get_copy());
        for(auto J = 0; J < c_node; ++J) {
            int_pt[I].strain_mat(0, c_dof * J) = int_pt[I].strain_mat(3, c_dof * J + 1) = int_pt[I].strain_mat(5, c_dof * J + 2) = pn_pxy(0, J);
            int_pt[I].strain_mat(3, c_dof * J) = int_pt[I].strain_mat(1, c_dof * J + 1) = int_pt[I].strain_mat(4, c_dof * J + 2) = pn_pxy(1, J);
            int_pt[I].strain_mat(5, c_dof * J) = int_pt[I].strain_mat(4, c_dof * J + 1) = int_pt[I].strain_mat(2, c_dof * J + 2) = pn_pxy(2, J);
        }
    }

    initial_stiffness.zeros(c_size, c_size);
    for(const auto& I : int_pt) initial_stiffness += I.strain_mat.t() * I.c_material->get_stiffness() * I.strain_mat * I.jacob_det * I.weight;

    initial_mass.zeros(c_size, c_size);
    const auto tmp_density = material_proto->get_parameter();
    if(tmp_density != 0.) {
        for(const auto& I : int_pt) {
            const auto n_int = shape::cube(I.coor, 0, 20);
            const auto tmp_a = tmp_density * I.jacob_det * I.weight;
            for(auto J = 0; J < c_node; ++J)
                for(auto K = J; K < c_node; ++K) initial_mass(c_dof * J, c_dof * K) += tmp_a * n_int(J) * n_int(K);
        }
        for(auto I = 0; I < c_size; I += c_dof) {
            initial_mass(I + 1, I + 1) = initial_mass(I + 2, I + 2) = initial_mass(I, I);
            for(auto J = I + c_dof; J < c_node * c_dof; J += c_dof) initial_mass(J, I) = initial_mass(I + 1, J + 1) = initial_mass(I + 2, J + 2) = initial_mass(J + 1, I + 1) = initial_mass(J + 2, I + 2) = initial_mass(I, J);
        }
    }
    trial_mass = current_mass = initial_mass;
}

int C3D20::update_status() {
    auto code = 0, idx = 0;

    vec t_disp(c_size);
    for(const auto& I : node_ptr) {
        const auto& tmp_disp = I.lock()->get_trial_displacement();
        for(auto J = 0; J < c_dof; ++J) t_disp(idx++) = tmp_disp(J);
    }

    trial_stiffness.zeros(c_size, c_size);
    trial_resistance.zeros(c_size);
    for(const auto& I : int_pt) {
        code += I.c_material->update_trial_status(I.strain_mat * t_disp);
        const mat t_factor = I.strain_mat.t() * I.jacob_det * I.weight;
        trial_stiffness += t_factor * I.c_material->get_stiffness() * I.strain_mat;
        trial_resistance += t_factor * I.c_material->get_stress();
    }

    return code;
}

int C3D20::commit_status() {
    auto code = 0;
    for(const auto& I : int_pt) code += I.c_material->commit_status();
    return code;
}

int C3D20::clear_status() {
    auto code = 0;
    for(const auto& I : int_pt) code += I.c_material->clear_status();
    return code;
}

int C3D20::reset_status() {
    auto code = 0;
    for(const auto& I : int_pt) code += I.c_material->reset_status();
    return code;
}

void C3D20::print() { suanpan_info("C3D20(R) element.\n"); }
