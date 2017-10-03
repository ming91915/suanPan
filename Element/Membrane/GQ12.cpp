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

#include "GQ12.h"
#include <Toolbox/IntegrationPlan.h>
#include <Toolbox/shapeFunction.hpp>

const unsigned GQ12::m_node = 4;
const unsigned GQ12::m_dof = 3;

GQ12::GQ12(const unsigned& T, const uvec& N, const unsigned& M, const double& TH)
    : Element(T, ET_GQ12, m_node, m_dof, N, uvec{ M }, false)
    , thickness(TH) {}

void GQ12::initialize(const shared_ptr<DomainBase>& D) {
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

    mat ele_coor(m_node, 2);
    for(unsigned I = 0; I < m_node; ++I) {
        auto& tmp_coor = node_ptr[I].lock()->get_coordinate();
        for(unsigned J = 0; J < 2; ++J) ele_coor(I, J) = tmp_coor(J);
    }

    const auto LX1 = ele_coor(1, 1) - ele_coor(0, 1);
    const auto LX2 = ele_coor(2, 1) - ele_coor(1, 1);
    const auto LX3 = ele_coor(3, 1) - ele_coor(2, 1);
    const auto LX4 = ele_coor(0, 1) - ele_coor(3, 1);
    const auto LY1 = ele_coor(0, 0) - ele_coor(1, 0);
    const auto LY2 = ele_coor(1, 0) - ele_coor(2, 0);
    const auto LY3 = ele_coor(2, 0) - ele_coor(3, 0);
    const auto LY4 = ele_coor(3, 0) - ele_coor(0, 0);

    mat pnt(2, 8);

    for(const auto& I : int_pt) {
        const auto pn = shape::quad(I->coor, 1);
        const mat jacob = pn * ele_coor;
        I->jacob_det = det(jacob);
        const mat pn_pxy = solve(jacob, pn);

        const auto X = 2. * I->coor(0);
        const auto Y = 2. * I->coor(1);

        const auto AA = I->coor(0) + 1.;
        const auto BB = I->coor(0) - 1.;
        const auto CC = I->coor(1) + 1.;
        const auto DD = I->coor(1) - 1.;

        pnt(0, 0) = DD * (LX4 * CC - LX1 * X);
        pnt(0, 1) = DD * (LX2 * CC + LX1 * X);
        pnt(0, 2) = CC * (LX3 * X - LX2 * DD);
        pnt(0, 3) = -CC * (LX3 * X + LX4 * DD);
        pnt(0, 4) = DD * (LY4 * CC - LY1 * X);
        pnt(0, 5) = DD * (LY2 * CC + LY1 * X);
        pnt(0, 6) = CC * (LY3 * X - LY2 * DD);
        pnt(0, 7) = -CC * (LY3 * X + LY4 * DD);
        pnt(1, 0) = BB * (LX4 * Y - LX1 * AA);
        pnt(1, 1) = AA * (LX1 * BB + LX2 * Y);
        pnt(1, 2) = AA * (LX3 * BB - LX2 * Y);
        pnt(1, 3) = -BB * (LX3 * AA + LX4 * Y);
        pnt(1, 4) = BB * (LY4 * Y - LY1 * AA);
        pnt(1, 5) = AA * (LY1 * BB + LY2 * Y);
        pnt(1, 6) = AA * (LY3 * BB - LY2 * Y);
        pnt(1, 7) = -BB * (LY3 * AA + LY4 * Y);

        const mat pnt_pxy = solve(jacob, pnt / 16.);

        I->strain_mat.zeros(3, m_node * m_dof);
        for(auto J = 0; J < m_node; ++J) {
            I->strain_mat(0, m_dof * J) = pn_pxy(0, J);
            I->strain_mat(2, m_dof * J) = pn_pxy(1, J);
            I->strain_mat(1, m_dof * J + 1) = pn_pxy(1, J);
            I->strain_mat(2, m_dof * J + 1) = pn_pxy(0, J);
            I->strain_mat(0, m_dof * J + 2) = pnt_pxy(0, J);
            I->strain_mat(1, m_dof * J + 2) = pnt_pxy(1, J + 4);
            I->strain_mat(2, m_dof * J + 2) = pnt_pxy(0, J + 4) + pnt_pxy(1, J);
        }
    }
}

int GQ12::update_status() {
    auto code = 0, idx = 0;

    vec trial_disp(m_node * m_dof);
    for(const auto& I : node_ptr) {
        auto& tmp_disp = I.lock()->get_trial_displacement();
        for(auto J = 0; J < m_dof; ++J) trial_disp(idx++) = tmp_disp(J);
    }

    stiffness.zeros();
    resistance.zeros();
    for(const auto& I : int_pt) {
        code += I->m_material->update_trial_status(I->strain_mat * trial_disp);
        const mat tmp_mat = I->strain_mat.t() * I->jacob_det * I->weight * thickness;
        stiffness += tmp_mat * I->m_material->get_stiffness() * I->strain_mat;
        resistance += tmp_mat * I->m_material->get_stress();
    }

    return code;
}

int GQ12::commit_status() {
    auto code = 0;
    for(const auto& I : int_pt) code += I->m_material->commit_status();
    return code;
}

int GQ12::clear_status() {
    auto code = 0;
    for(const auto& I : int_pt) code += I->m_material->clear_status();
    return code;
}

int GQ12::reset_status() {
    auto code = 0;
    for(const auto& I : int_pt) code += I->m_material->reset_status();
    return code;
}

void GQ12::print() {
    suanpan_info("Material model response:\n");
    for(auto I = 0; I < int_pt.size(); ++I) {
        suanpan_info("Integration Point %u:\n", I + 1);
        int_pt[I]->m_material->print();
    }
}
