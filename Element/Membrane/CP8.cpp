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

#include "CP8.h"
#include <Toolbox/IntegrationPlan.h>
#include <Toolbox/shapeFunction.hpp>

const unsigned CP8::m_node = 8;
const unsigned CP8::m_dof = 2;

CP8::CP8(const unsigned& T, const uvec& N, const unsigned& M, const double& TH, const bool& R, const bool& F)
    : Element(T, ET_CP8, m_node, m_dof, N, uvec{ M }, F)
    , thickness(TH)
    , reduced_scheme(R) {}

void CP8::initialize(const shared_ptr<DomainBase>& D) {
    mat ele_coor(m_node, m_dof);
    for(unsigned I = 0; I < m_node; ++I) {
        auto& tmp_coor = node_ptr[I].lock()->get_coordinate();
        for(unsigned J = 0; J < m_dof; ++J) ele_coor(I, J) = tmp_coor(J);
    }

    const auto& material_proto = D->get_material(unsigned(material_tag(0)));

    const unsigned order = reduced_scheme ? 2 : 3;
    const IntegrationPlan plan(2, order, IntegrationType::GAUSS);

    int_pt.clear();
    int_pt.reserve(plan.n_rows);
    for(unsigned I = 0; I < plan.n_rows; ++I) {
        const vec t_vec{ plan(I, 0), plan(I, 1) };
        const auto pn = shape::quad(t_vec, 1, 8);
        const mat jacob = pn * ele_coor;
        int_pt.emplace_back(t_vec, plan(I, 2), det(jacob), material_proto->get_copy(), solve(jacob, pn));
    }

    mass.zeros();
    const auto tmp_density = material_proto->get_parameter();
    if(tmp_density != 0.) {
        for(const auto& I : int_pt) {
            const auto n_int = shape::quad(I.coor, 0, 8);
            const auto tmp_a = tmp_density * I.jacob_det * I.weight * thickness;
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

int CP8::update_status() {
    auto code = 0;

    vec t_strain(3);

    stiffness.zeros();
    resistance.zeros();
    for(const auto& I : int_pt) {
        t_strain.zeros();
        for(auto J = 0; J < m_node; ++J) {
            const auto& t_disp = node_ptr[J].lock()->get_trial_displacement();
            t_strain(0) += t_disp(0) * I.pn_pxy(0, J);
            t_strain(1) += t_disp(1) * I.pn_pxy(1, J);
            t_strain(2) += t_disp(0) * I.pn_pxy(1, J) + t_disp(1) * I.pn_pxy(0, J);
        }
        code += I.m_material->update_trial_status(t_strain);

        const auto t_factor = I.jacob_det * I.weight * thickness;

        auto& t_stiff = I.m_material->get_stiffness();
        auto& t_stress = I.m_material->get_stress();

        const auto& NX1 = I.pn_pxy(0, 0);
        const auto& NY1 = I.pn_pxy(1, 0);
        const auto& NX2 = I.pn_pxy(0, 1);
        const auto& NY2 = I.pn_pxy(1, 1);
        const auto& NX3 = I.pn_pxy(0, 2);
        const auto& NY3 = I.pn_pxy(1, 2);
        const auto& NX4 = I.pn_pxy(0, 3);
        const auto& NY4 = I.pn_pxy(1, 3);
        const auto& NX5 = I.pn_pxy(0, 4);
        const auto& NY5 = I.pn_pxy(1, 4);
        const auto& NX6 = I.pn_pxy(0, 5);
        const auto& NY6 = I.pn_pxy(1, 5);
        const auto& NX7 = I.pn_pxy(0, 6);
        const auto& NY7 = I.pn_pxy(1, 6);
        const auto& NX8 = I.pn_pxy(0, 7);
        const auto& NY8 = I.pn_pxy(1, 7);

        const auto D11 = t_factor * t_stiff(0, 0);
        const auto D12 = t_factor * t_stiff(0, 1);
        const auto D13 = t_factor * t_stiff(0, 2);
        const auto D22 = t_factor * t_stiff(1, 1);
        const auto D23 = t_factor * t_stiff(1, 2);
        const auto D33 = t_factor * t_stiff(2, 2);

        const auto S1 = t_factor * t_stress(0);
        const auto S2 = t_factor * t_stress(1);
        const auto S3 = t_factor * t_stress(2);

        // 351+183 (912+608)
        const auto D11NX1 = D11 * NX1;
        const auto D11NX2 = D11 * NX2;
        const auto D11NX3 = D11 * NX3;
        const auto D11NX4 = D11 * NX4;
        const auto D11NX5 = D11 * NX5;
        const auto D11NX6 = D11 * NX6;
        const auto D11NX7 = D11 * NX7;
        const auto D11NX8 = D11 * NX8;
        const auto D12NX1 = D12 * NX1;
        const auto D12NX2 = D12 * NX2;
        const auto D12NX3 = D12 * NX3;
        const auto D12NX4 = D12 * NX4;
        const auto D12NX5 = D12 * NX5;
        const auto D12NX6 = D12 * NX6;
        const auto D12NX7 = D12 * NX7;
        const auto D12NX8 = D12 * NX8;
        const auto D13NX1 = D13 * NX1;
        const auto D13NX2 = D13 * NX2;
        const auto D13NX3 = D13 * NX3;
        const auto D13NX4 = D13 * NX4;
        const auto D13NX5 = D13 * NX5;
        const auto D13NX6 = D13 * NX6;
        const auto D13NX7 = D13 * NX7;
        const auto D13NX8 = D13 * NX8;
        const auto D23NX1 = D23 * NX1;
        const auto D23NX2 = D23 * NX2;
        const auto D23NX3 = D23 * NX3;
        const auto D23NX4 = D23 * NX4;
        const auto D23NX5 = D23 * NX5;
        const auto D23NX6 = D23 * NX6;
        const auto D23NX7 = D23 * NX7;
        const auto D23NX8 = D23 * NX8;
        const auto D33NX1 = D33 * NX1;
        const auto D33NX2 = D33 * NX2;
        const auto D33NX3 = D33 * NX3;
        const auto D33NX4 = D33 * NX4;
        const auto D33NX5 = D33 * NX5;
        const auto D33NX6 = D33 * NX6;
        const auto D33NX7 = D33 * NX7;
        const auto D33NX8 = D33 * NX8;
        const auto D12NY1 = D12 * NY1;
        const auto D12NY2 = D12 * NY2;
        const auto D12NY3 = D12 * NY3;
        const auto D12NY4 = D12 * NY4;
        const auto D12NY5 = D12 * NY5;
        const auto D12NY6 = D12 * NY6;
        const auto D12NY7 = D12 * NY7;
        const auto D13NY1 = D13 * NY1;
        const auto D13NY2 = D13 * NY2;
        const auto D13NY3 = D13 * NY3;
        const auto D13NY4 = D13 * NY4;
        const auto D13NY5 = D13 * NY5;
        const auto D13NY6 = D13 * NY6;
        const auto D13NY7 = D13 * NY7;
        const auto D13NY8 = D13 * NY8;
        const auto D22NY1 = D22 * NY1;
        const auto D22NY2 = D22 * NY2;
        const auto D22NY3 = D22 * NY3;
        const auto D22NY4 = D22 * NY4;
        const auto D22NY5 = D22 * NY5;
        const auto D22NY6 = D22 * NY6;
        const auto D22NY7 = D22 * NY7;
        const auto D22NY8 = D22 * NY8;
        const auto D23NY1 = D23 * NY1;
        const auto D23NY2 = D23 * NY2;
        const auto D23NY3 = D23 * NY3;
        const auto D23NY4 = D23 * NY4;
        const auto D23NY5 = D23 * NY5;
        const auto D23NY6 = D23 * NY6;
        const auto D23NY7 = D23 * NY7;
        const auto D23NY8 = D23 * NY8;
        const auto D33NY1 = D33 * NY1;
        const auto D33NY2 = D33 * NY2;
        const auto D33NY3 = D33 * NY3;
        const auto D33NY4 = D33 * NY4;
        const auto D33NY5 = D33 * NY5;
        const auto D33NY6 = D33 * NY6;
        const auto D33NY7 = D33 * NY7;
        const auto D33NY8 = D33 * NY8;

        const auto D11NX1D13NY1 = D11NX1 + D13NY1;
        const auto D13NX1D33NY1 = D13NX1 + D33NY1;
        const auto D33NX1D23NY1 = D33NX1 + D23NY1;
        const auto D13NX1D12NY1 = D13NX1 + D12NY1;
        const auto D11NX2D13NY2 = D11NX2 + D13NY2;
        const auto D13NX2D33NY2 = D13NX2 + D33NY2;
        const auto D33NX2D23NY2 = D33NX2 + D23NY2;
        const auto D13NX2D12NY2 = D13NX2 + D12NY2;
        const auto D11NX3D13NY3 = D11NX3 + D13NY3;
        const auto D13NX3D33NY3 = D13NX3 + D33NY3;
        const auto D33NX3D23NY3 = D33NX3 + D23NY3;
        const auto D13NX3D12NY3 = D13NX3 + D12NY3;
        const auto D11NX4D13NY4 = D11NX4 + D13NY4;
        const auto D13NX4D33NY4 = D13NX4 + D33NY4;
        const auto D33NX4D23NY4 = D33NX4 + D23NY4;
        const auto D13NX4D12NY4 = D13NX4 + D12NY4;
        const auto D11NX5D13NY5 = D11NX5 + D13NY5;
        const auto D13NX5D33NY5 = D13NX5 + D33NY5;
        const auto D33NX5D23NY5 = D33NX5 + D23NY5;
        const auto D13NX5D12NY5 = D13NX5 + D12NY5;
        const auto D11NX6D13NY6 = D11NX6 + D13NY6;
        const auto D13NX6D33NY6 = D13NX6 + D33NY6;
        const auto D33NX6D23NY6 = D33NX6 + D23NY6;
        const auto D13NX6D12NY6 = D13NX6 + D12NY6;
        const auto D11NX7D13NY7 = D11NX7 + D13NY7;
        const auto D13NX7D33NY7 = D13NX7 + D33NY7;
        const auto D33NX7D23NY7 = D33NX7 + D23NY7;
        const auto D13NX7D12NY7 = D13NX7 + D12NY7;
        const auto D11NX8D13NY8 = D11NX8 + D13NY8;
        const auto D13NX8D33NY8 = D13NX8 + D33NY8;
        const auto D33NX8D23NY8 = D33NX8 + D23NY8;
        const auto D12NX1D23NY1 = D12NX1 + D23NY1;
        const auto D23NX1D22NY1 = D23NX1 + D22NY1;
        const auto D12NX2D23NY2 = D12NX2 + D23NY2;
        const auto D23NX2D22NY2 = D23NX2 + D22NY2;
        const auto D12NX3D23NY3 = D12NX3 + D23NY3;
        const auto D23NX3D22NY3 = D23NX3 + D22NY3;
        const auto D12NX4D23NY4 = D12NX4 + D23NY4;
        const auto D23NX4D22NY4 = D23NX4 + D22NY4;
        const auto D12NX5D23NY5 = D12NX5 + D23NY5;
        const auto D23NX5D22NY5 = D23NX5 + D22NY5;
        const auto D12NX6D23NY6 = D12NX6 + D23NY6;
        const auto D23NX6D22NY6 = D23NX6 + D22NY6;
        const auto D12NX7D23NY7 = D12NX7 + D23NY7;
        const auto D23NX7D22NY7 = D23NX7 + D22NY7;
        const auto D12NX8D23NY8 = D12NX8 + D23NY8;
        const auto D23NX8D22NY8 = D23NX8 + D22NY8;

        stiffness(0, 0) += NX1 * D11NX1D13NY1 + NY1 * D13NX1D33NY1;
        stiffness(0, 1) += NX1 * D13NX1D33NY1 + NY1 * D12NX1D23NY1;
        stiffness(0, 2) += NX2 * D11NX1D13NY1 + NY2 * D13NX1D33NY1;
        stiffness(0, 3) += NX2 * D13NX1D33NY1 + NY2 * D12NX1D23NY1;
        stiffness(0, 4) += NX3 * D11NX1D13NY1 + NY3 * D13NX1D33NY1;
        stiffness(0, 5) += NX3 * D13NX1D33NY1 + NY3 * D12NX1D23NY1;
        stiffness(0, 6) += NX4 * D11NX1D13NY1 + NY4 * D13NX1D33NY1;
        stiffness(0, 7) += NX4 * D13NX1D33NY1 + NY4 * D12NX1D23NY1;
        stiffness(0, 8) += NX5 * D11NX1D13NY1 + NY5 * D13NX1D33NY1;
        stiffness(0, 9) += NX5 * D13NX1D33NY1 + NY5 * D12NX1D23NY1;
        stiffness(0, 10) += NX6 * D11NX1D13NY1 + NY6 * D13NX1D33NY1;
        stiffness(0, 11) += NX6 * D13NX1D33NY1 + NY6 * D12NX1D23NY1;
        stiffness(0, 12) += NX7 * D11NX1D13NY1 + NY7 * D13NX1D33NY1;
        stiffness(0, 13) += NX7 * D13NX1D33NY1 + NY7 * D12NX1D23NY1;
        stiffness(0, 14) += NX8 * D11NX1D13NY1 + NY8 * D13NX1D33NY1;
        stiffness(0, 15) += NX8 * D13NX1D33NY1 + NY8 * D12NX1D23NY1;
        stiffness(1, 1) += NX1 * D33NX1D23NY1 + NY1 * D23NX1D22NY1;
        stiffness(1, 2) += NX2 * D13NX1D12NY1 + NY2 * D33NX1D23NY1;
        stiffness(1, 3) += NX2 * D33NX1D23NY1 + NY2 * D23NX1D22NY1;
        stiffness(1, 4) += NX3 * D13NX1D12NY1 + NY3 * D33NX1D23NY1;
        stiffness(1, 5) += NX3 * D33NX1D23NY1 + NY3 * D23NX1D22NY1;
        stiffness(1, 6) += NX4 * D13NX1D12NY1 + NY4 * D33NX1D23NY1;
        stiffness(1, 7) += NX4 * D33NX1D23NY1 + NY4 * D23NX1D22NY1;
        stiffness(1, 8) += NX5 * D13NX1D12NY1 + NY5 * D33NX1D23NY1;
        stiffness(1, 9) += NX5 * D33NX1D23NY1 + NY5 * D23NX1D22NY1;
        stiffness(1, 10) += NX6 * D13NX1D12NY1 + NY6 * D33NX1D23NY1;
        stiffness(1, 11) += NX6 * D33NX1D23NY1 + NY6 * D23NX1D22NY1;
        stiffness(1, 12) += NX7 * D13NX1D12NY1 + NY7 * D33NX1D23NY1;
        stiffness(1, 13) += NX7 * D33NX1D23NY1 + NY7 * D23NX1D22NY1;
        stiffness(1, 14) += NX8 * D13NX1D12NY1 + NY8 * D33NX1D23NY1;
        stiffness(1, 15) += NX8 * D33NX1D23NY1 + NY8 * D23NX1D22NY1;
        stiffness(2, 2) += NX2 * D11NX2D13NY2 + NY2 * D13NX2D33NY2;
        stiffness(2, 3) += NX2 * D13NX2D33NY2 + NY2 * D12NX2D23NY2;
        stiffness(2, 4) += NX3 * D11NX2D13NY2 + NY3 * D13NX2D33NY2;
        stiffness(2, 5) += NX3 * D13NX2D33NY2 + NY3 * D12NX2D23NY2;
        stiffness(2, 6) += NX4 * D11NX2D13NY2 + NY4 * D13NX2D33NY2;
        stiffness(2, 7) += NX4 * D13NX2D33NY2 + NY4 * D12NX2D23NY2;
        stiffness(2, 8) += NX5 * D11NX2D13NY2 + NY5 * D13NX2D33NY2;
        stiffness(2, 9) += NX5 * D13NX2D33NY2 + NY5 * D12NX2D23NY2;
        stiffness(2, 10) += NX6 * D11NX2D13NY2 + NY6 * D13NX2D33NY2;
        stiffness(2, 11) += NX6 * D13NX2D33NY2 + NY6 * D12NX2D23NY2;
        stiffness(2, 12) += NX7 * D11NX2D13NY2 + NY7 * D13NX2D33NY2;
        stiffness(2, 13) += NX7 * D13NX2D33NY2 + NY7 * D12NX2D23NY2;
        stiffness(2, 14) += NX8 * D11NX2D13NY2 + NY8 * D13NX2D33NY2;
        stiffness(2, 15) += NX8 * D13NX2D33NY2 + NY8 * D12NX2D23NY2;
        stiffness(3, 3) += NX2 * D33NX2D23NY2 + NY2 * D23NX2D22NY2;
        stiffness(3, 4) += NX3 * D13NX2D12NY2 + NY3 * D33NX2D23NY2;
        stiffness(3, 5) += NX3 * D33NX2D23NY2 + NY3 * D23NX2D22NY2;
        stiffness(3, 6) += NX4 * D13NX2D12NY2 + NY4 * D33NX2D23NY2;
        stiffness(3, 7) += NX4 * D33NX2D23NY2 + NY4 * D23NX2D22NY2;
        stiffness(3, 8) += NX5 * D13NX2D12NY2 + NY5 * D33NX2D23NY2;
        stiffness(3, 9) += NX5 * D33NX2D23NY2 + NY5 * D23NX2D22NY2;
        stiffness(3, 10) += NX6 * D13NX2D12NY2 + NY6 * D33NX2D23NY2;
        stiffness(3, 11) += NX6 * D33NX2D23NY2 + NY6 * D23NX2D22NY2;
        stiffness(3, 12) += NX7 * D13NX2D12NY2 + NY7 * D33NX2D23NY2;
        stiffness(3, 13) += NX7 * D33NX2D23NY2 + NY7 * D23NX2D22NY2;
        stiffness(3, 14) += NX8 * D13NX2D12NY2 + NY8 * D33NX2D23NY2;
        stiffness(3, 15) += NX8 * D33NX2D23NY2 + NY8 * D23NX2D22NY2;
        stiffness(4, 4) += NX3 * D11NX3D13NY3 + NY3 * D13NX3D33NY3;
        stiffness(4, 5) += NX3 * D13NX3D33NY3 + NY3 * D12NX3D23NY3;
        stiffness(4, 6) += NX4 * D11NX3D13NY3 + NY4 * D13NX3D33NY3;
        stiffness(4, 7) += NX4 * D13NX3D33NY3 + NY4 * D12NX3D23NY3;
        stiffness(4, 8) += NX5 * D11NX3D13NY3 + NY5 * D13NX3D33NY3;
        stiffness(4, 9) += NX5 * D13NX3D33NY3 + NY5 * D12NX3D23NY3;
        stiffness(4, 10) += NX6 * D11NX3D13NY3 + NY6 * D13NX3D33NY3;
        stiffness(4, 11) += NX6 * D13NX3D33NY3 + NY6 * D12NX3D23NY3;
        stiffness(4, 12) += NX7 * D11NX3D13NY3 + NY7 * D13NX3D33NY3;
        stiffness(4, 13) += NX7 * D13NX3D33NY3 + NY7 * D12NX3D23NY3;
        stiffness(4, 14) += NX8 * D11NX3D13NY3 + NY8 * D13NX3D33NY3;
        stiffness(4, 15) += NX8 * D13NX3D33NY3 + NY8 * D12NX3D23NY3;
        stiffness(5, 5) += NX3 * D33NX3D23NY3 + NY3 * D23NX3D22NY3;
        stiffness(5, 6) += NX4 * D13NX3D12NY3 + NY4 * D33NX3D23NY3;
        stiffness(5, 7) += NX4 * D33NX3D23NY3 + NY4 * D23NX3D22NY3;
        stiffness(5, 8) += NX5 * D13NX3D12NY3 + NY5 * D33NX3D23NY3;
        stiffness(5, 9) += NX5 * D33NX3D23NY3 + NY5 * D23NX3D22NY3;
        stiffness(5, 10) += NX6 * D13NX3D12NY3 + NY6 * D33NX3D23NY3;
        stiffness(5, 11) += NX6 * D33NX3D23NY3 + NY6 * D23NX3D22NY3;
        stiffness(5, 12) += NX7 * D13NX3D12NY3 + NY7 * D33NX3D23NY3;
        stiffness(5, 13) += NX7 * D33NX3D23NY3 + NY7 * D23NX3D22NY3;
        stiffness(5, 14) += NX8 * D13NX3D12NY3 + NY8 * D33NX3D23NY3;
        stiffness(5, 15) += NX8 * D33NX3D23NY3 + NY8 * D23NX3D22NY3;
        stiffness(6, 6) += NX4 * D11NX4D13NY4 + NY4 * D13NX4D33NY4;
        stiffness(6, 7) += NX4 * D13NX4D33NY4 + NY4 * D12NX4D23NY4;
        stiffness(6, 8) += NX5 * D11NX4D13NY4 + NY5 * D13NX4D33NY4;
        stiffness(6, 9) += NX5 * D13NX4D33NY4 + NY5 * D12NX4D23NY4;
        stiffness(6, 10) += NX6 * D11NX4D13NY4 + NY6 * D13NX4D33NY4;
        stiffness(6, 11) += NX6 * D13NX4D33NY4 + NY6 * D12NX4D23NY4;
        stiffness(6, 12) += NX7 * D11NX4D13NY4 + NY7 * D13NX4D33NY4;
        stiffness(6, 13) += NX7 * D13NX4D33NY4 + NY7 * D12NX4D23NY4;
        stiffness(6, 14) += NX8 * D11NX4D13NY4 + NY8 * D13NX4D33NY4;
        stiffness(6, 15) += NX8 * D13NX4D33NY4 + NY8 * D12NX4D23NY4;
        stiffness(7, 7) += NX4 * D33NX4D23NY4 + NY4 * D23NX4D22NY4;
        stiffness(7, 8) += NX5 * D13NX4D12NY4 + NY5 * D33NX4D23NY4;
        stiffness(7, 9) += NX5 * D33NX4D23NY4 + NY5 * D23NX4D22NY4;
        stiffness(7, 10) += NX6 * D13NX4D12NY4 + NY6 * D33NX4D23NY4;
        stiffness(7, 11) += NX6 * D33NX4D23NY4 + NY6 * D23NX4D22NY4;
        stiffness(7, 12) += NX7 * D13NX4D12NY4 + NY7 * D33NX4D23NY4;
        stiffness(7, 13) += NX7 * D33NX4D23NY4 + NY7 * D23NX4D22NY4;
        stiffness(7, 14) += NX8 * D13NX4D12NY4 + NY8 * D33NX4D23NY4;
        stiffness(7, 15) += NX8 * D33NX4D23NY4 + NY8 * D23NX4D22NY4;
        stiffness(8, 8) += NX5 * D11NX5D13NY5 + NY5 * D13NX5D33NY5;
        stiffness(8, 9) += NX5 * D13NX5D33NY5 + NY5 * D12NX5D23NY5;
        stiffness(8, 10) += NX6 * D11NX5D13NY5 + NY6 * D13NX5D33NY5;
        stiffness(8, 11) += NX6 * D13NX5D33NY5 + NY6 * D12NX5D23NY5;
        stiffness(8, 12) += NX7 * D11NX5D13NY5 + NY7 * D13NX5D33NY5;
        stiffness(8, 13) += NX7 * D13NX5D33NY5 + NY7 * D12NX5D23NY5;
        stiffness(8, 14) += NX8 * D11NX5D13NY5 + NY8 * D13NX5D33NY5;
        stiffness(8, 15) += NX8 * D13NX5D33NY5 + NY8 * D12NX5D23NY5;
        stiffness(9, 9) += NX5 * D33NX5D23NY5 + NY5 * D23NX5D22NY5;
        stiffness(9, 10) += NX6 * D13NX5D12NY5 + NY6 * D33NX5D23NY5;
        stiffness(9, 11) += NX6 * D33NX5D23NY5 + NY6 * D23NX5D22NY5;
        stiffness(9, 12) += NX7 * D13NX5D12NY5 + NY7 * D33NX5D23NY5;
        stiffness(9, 13) += NX7 * D33NX5D23NY5 + NY7 * D23NX5D22NY5;
        stiffness(9, 14) += NX8 * D13NX5D12NY5 + NY8 * D33NX5D23NY5;
        stiffness(9, 15) += NX8 * D33NX5D23NY5 + NY8 * D23NX5D22NY5;
        stiffness(10, 10) += NX6 * D11NX6D13NY6 + NY6 * D13NX6D33NY6;
        stiffness(10, 11) += NX6 * D13NX6D33NY6 + NY6 * D12NX6D23NY6;
        stiffness(10, 12) += NX7 * D11NX6D13NY6 + NY7 * D13NX6D33NY6;
        stiffness(10, 13) += NX7 * D13NX6D33NY6 + NY7 * D12NX6D23NY6;
        stiffness(10, 14) += NX8 * D11NX6D13NY6 + NY8 * D13NX6D33NY6;
        stiffness(10, 15) += NX8 * D13NX6D33NY6 + NY8 * D12NX6D23NY6;
        stiffness(11, 11) += NX6 * D33NX6D23NY6 + NY6 * D23NX6D22NY6;
        stiffness(11, 12) += NX7 * D13NX6D12NY6 + NY7 * D33NX6D23NY6;
        stiffness(11, 13) += NX7 * D33NX6D23NY6 + NY7 * D23NX6D22NY6;
        stiffness(11, 14) += NX8 * D13NX6D12NY6 + NY8 * D33NX6D23NY6;
        stiffness(11, 15) += NX8 * D33NX6D23NY6 + NY8 * D23NX6D22NY6;
        stiffness(12, 12) += NX7 * D11NX7D13NY7 + NY7 * D13NX7D33NY7;
        stiffness(12, 13) += NX7 * D13NX7D33NY7 + NY7 * D12NX7D23NY7;
        stiffness(12, 14) += NX8 * D11NX7D13NY7 + NY8 * D13NX7D33NY7;
        stiffness(12, 15) += NX8 * D13NX7D33NY7 + NY8 * D12NX7D23NY7;
        stiffness(13, 13) += NX7 * D33NX7D23NY7 + NY7 * D23NX7D22NY7;
        stiffness(13, 14) += NX8 * D13NX7D12NY7 + NY8 * D33NX7D23NY7;
        stiffness(13, 15) += NX8 * D33NX7D23NY7 + NY8 * D23NX7D22NY7;
        stiffness(14, 14) += NX8 * D11NX8D13NY8 + NY8 * D13NX8D33NY8;
        stiffness(14, 15) += NX8 * D13NX8D33NY8 + NY8 * D12NX8D23NY8;
        stiffness(15, 15) += NX8 * D33NX8D23NY8 + NY8 * D23NX8D22NY8;

        resistance(0) += NX1 * S1 + NY1 * S3;
        resistance(1) += NX1 * S3 + NY1 * S2;
        resistance(2) += NX2 * S1 + NY2 * S3;
        resistance(3) += NX2 * S3 + NY2 * S2;
        resistance(4) += NX3 * S1 + NY3 * S3;
        resistance(5) += NX3 * S3 + NY3 * S2;
        resistance(6) += NX4 * S1 + NY4 * S3;
        resistance(7) += NX4 * S3 + NY4 * S2;
        resistance(8) += NX5 * S1 + NY5 * S3;
        resistance(9) += NX5 * S3 + NY5 * S2;
        resistance(10) += NX6 * S1 + NY6 * S3;
        resistance(11) += NX6 * S3 + NY6 * S2;
        resistance(12) += NX7 * S1 + NY7 * S3;
        resistance(13) += NX7 * S3 + NY7 * S2;
        resistance(14) += NX8 * S1 + NY8 * S3;
        resistance(15) += NX8 * S3 + NY8 * S2;
    }

    for(auto I = 0; I < 15; ++I)
        for(auto J = I + 1; J < 16; ++J) stiffness(J, I) = stiffness(I, J);

    return code;
}

int CP8::commit_status() {
    auto code = 0;
    for(const auto& I : int_pt) code += I.m_material->commit_status();
    return code;
}

int CP8::clear_status() {
    auto code = 0;
    for(const auto& I : int_pt) code += I.m_material->clear_status();
    return code;
}

int CP8::reset_status() {
    auto code = 0;
    for(const auto& I : int_pt) code += I.m_material->reset_status();
    return code;
}

void CP8::print() { suanpan_info("This is a CP8 element.\n"); }
