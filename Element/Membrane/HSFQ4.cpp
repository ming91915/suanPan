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

#include "HSFQ4.h"
#include <Toolbox/IntegrationPlan.h>
#include <Toolbox/shapeFunction.hpp>
#include <Toolbox/tensorToolbox.h>

const unsigned HSFQ4::m_node = 4;
const unsigned HSFQ4::m_dof = 3;
mat HSFQ4::mapping;

HSFQ4::HSFQ4(const unsigned& T, const uvec& N, const unsigned& M, const double& TH, const bool& R, const bool& F)
    : Element(T, ET_HSFQ4, m_node, m_dof, N, uvec{ M }, F)
    , thickness(TH)
    , reduced_scheme(R) {}

void HSFQ4::initialize(const shared_ptr<DomainBase>& D) {
    if (mapping.is_empty()) {
        mapping.zeros(4, 4);
        mapping.fill(.25);
        mapping(1, 0) = -.25;
        mapping(1, 3) = -.25;
        mapping(2, 0) = -.25;
        mapping(2, 1) = -.25;
        mapping(3, 1) = -.25;
        mapping(3, 3) = -.25;
    }

    const auto& material_proto = D->get_material(static_cast<unsigned>(material_tag(0)));

    const IntegrationPlan plan(2, 2, 1);

    int_pt.clear();
    for(unsigned I = 0; I < plan.n_rows; ++I) {
        int_pt.push_back(make_unique<IntegrationPoint>());
        int_pt[I]->coor.zeros(2);
        for(auto J = 0; J < 2; ++J) int_pt[I]->coor(J) = plan(I, J);
        int_pt[I]->weight = plan(I, 2);
        int_pt[I]->m_material = material_proto->get_copy();
    }

    mat ele_coor(m_node, m_dof);
    for(unsigned I = 0; I < m_node; ++I) {
        auto& tmp_coor = node_ptr[I].lock()->get_coordinate();
        for(unsigned J = 0; J < m_dof; ++J) ele_coor(I, J) = tmp_coor(J);
    }

    t_factor = trans(mapping * ele_coor);

    vec disp_mode(4, fill::zeros);

    for(const auto& I : int_pt) {
        const auto pn = shapeFunctionQuad(I->coor, 1);
        const mat jacob = pn * ele_coor;
        I->jacob_det = det(jacob);
        I->pn_pxy = solve(jacob, pn);

        disp_mode(1) = I->coor(0);
        disp_mode(2) = I->coor(1);
        disp_mode(3) = I->coor(0) * I->coor(1);

        I->S = shapeStress7(t_factor * disp_mode);

    }

    mass.zeros();
    const auto tmp_density = material_proto->get_parameter();
    if(tmp_density != 0.) {
        for(const auto& I : int_pt) {
            const auto n_int = shapeFunctionQuad(I->coor, 0);
            const auto tmp_a = tmp_density * I->jacob_det * I->weight * thickness;
            for(auto J = 0; J < m_node; ++J)
                for(auto K = J; K < m_node; ++K) mass(m_dof * J, m_dof * K) += tmp_a * n_int(J) * n_int(K);
        }
        for(auto I = 0; I < m_node * m_dof; I += m_dof) {
            mass(I + 1, I + 1) = mass(I, I);
            for(auto J = I + m_dof; J < m_node * m_dof; J += m_dof) {
                mass(J, I) = mass(I, J);
                mass(I + 1, J + 1) = mass(I, J);
                mass(J + 1, I + 1) = mass(I, J);
            }
        }
    }
}

int HSFQ4::update_status() {
    auto code = 0;

    vec t_strain(3);
    mat ele_disp(m_node, m_dof);
    mat sigma(4, 4, fill::zeros);

    stiffness.zeros();
    resistance.zeros();
    for (const auto& I : int_pt) {
            t_strain.zeros();
            for (auto J = 0; J < m_node; ++J) {
                const auto& t_disp = node_ptr[J].lock()->get_trial_displacement();
                t_strain(0) += t_disp(0) * I->pn_pxy(0, J);
                t_strain(1) += t_disp(1) * I->pn_pxy(1, J);
                t_strain(2) += t_disp(0) * I->pn_pxy(1, J) + t_disp(1) * I->pn_pxy(0, J);
            }

        code += I->m_material->update_trial_status(t_strain);

        const auto t_factor = I->jacob_det * I->weight * thickness;

        auto& t_stiff = I->m_material->get_stiffness();
        auto& t_stress = I->m_material->get_stress();
    }

    return code;
}

int HSFQ4::commit_status() {
    auto code = 0;
    for(const auto& I : int_pt) code += I->m_material->commit_status();
    return code;
}

int HSFQ4::clear_status() {
    auto code = 0;
    for(const auto& I : int_pt) code += I->m_material->clear_status();
    return code;
}

int HSFQ4::reset_status() {
    auto code = 0;
    for(const auto& I : int_pt) code += I->m_material->reset_status();
    return code;
}

void HSFQ4::print() {
    suanpan_info("Element %u is a four-node membrane element (HSFQ4)%s.\n", get_tag(), nlgeom ? " with nonlinear geomotry (TL formulation)" : "");
    suanpan_info("The nodes connected are:\n");
    node_encoding.t().print();
}
