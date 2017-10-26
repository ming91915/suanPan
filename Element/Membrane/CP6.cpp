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

#include "CP6.h"
#include <Domain/DomainBase.h>
#include <Domain/Node.h>
#include <Material/Material2D/Material2D.h>
#include <Toolbox/shapeFunction.hpp>
#include <Toolbox/utility.h>

const unsigned CP6::m_node = 6;
const unsigned CP6::m_dof = 2;

CP6::IntegrationPoint::IntegrationPoint(const vec& C, const double W, unique_ptr<Material>&& M, const mat& PNPXY)
    : coor(C)
    , weight(W)
    , m_material(move(M))
    , pn_pxy(PNPXY) {}

CP6::CP6(const unsigned T, const uvec& NT, const unsigned MT, const double TH)
    : Element(T, ET_CP6, m_node, m_dof, NT, uvec{ MT }, false)
    , thickness(TH) {}

void CP6::initialize(const shared_ptr<DomainBase>& D) {
    mat ele_coor(m_node, m_node);
    for(auto I = 0; I < m_node; ++I) {
        auto& t_coor = node_ptr[I].lock()->get_coordinate();
        ele_coor(I, 0) = 1.;
        ele_coor(I, 1) = t_coor(0);
        ele_coor(I, 2) = t_coor(1);
        ele_coor(I, 3) = t_coor(0) * t_coor(1);
        ele_coor(I, 4) = t_coor(0) * t_coor(0);
        ele_coor(I, 5) = t_coor(1) * t_coor(1);
    }

    const mat inv_coor = inv(ele_coor);

    area = .5 * det(ele_coor(span(0, 2), span(0, 2)));

    auto& material_proto = D->get_material(unsigned(material_tag(0)));

    if(material_proto->material_type == MaterialType::D2 && std::dynamic_pointer_cast<Material2D>(material_proto)->plane_type == PlaneType::E) suanpan::modifier(thickness) = 1.;

    int_pt.clear(), int_pt.reserve(3);
    for(auto I = 0; I < 3; ++I) {
        const vec coor{ ele_coor(I + 3, 1), ele_coor(I + 3, 2) };
        int_pt.emplace_back(coor, 1. / 3., material_proto->get_copy(), shape::triangle(coor, 1) * inv_coor);
    }

    mass.zeros();
    auto t_density = material_proto->get_parameter();
    if(t_density != 0.) {
        t_density *= area * thickness;
        for(const auto& I : int_pt) {
            const vec n_int = shape::triangle(I.coor, 0) * inv_coor;
            const auto t_factor = t_density * I.weight;
            for(auto J = 0; J < m_node; ++J)
                for(auto K = J; K < m_node; ++K) mass(m_dof * J, m_dof * K) += t_factor * n_int(J) * n_int(K);
        }
        for(auto I = 0; I < m_node * m_dof; I += m_dof) {
            mass(I + 1, I + 1) = mass(I, I);
            for(auto J = I + m_dof; J < m_node * m_dof; J += m_dof) mass(J, I) = mass(I + 1, J + 1) = mass(J + 1, I + 1) = mass(I, J);
        }
    }
}

int CP6::update_status() {
    auto code = 0;

    vec t_strain(3);
    for(const auto& I : int_pt) {
        t_strain.zeros();
        for(auto J = 0; J < m_node; ++J) {
            const auto& t_disp = node_ptr[J].lock()->get_trial_displacement();
            t_strain(0) += t_disp(0) * I.pn_pxy(0, J);
            t_strain(1) += t_disp(1) * I.pn_pxy(1, J);
            t_strain(2) += t_disp(0) * I.pn_pxy(1, J) + t_disp(1) * I.pn_pxy(0, J);
        }
        code += I.m_material->update_trial_status(t_strain);

        if(code != 0) return code;

        const auto t_factor = area * thickness * I.weight;

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

        const auto D11 = t_factor * t_stiff(0, 0);
        const auto D12 = t_factor * t_stiff(0, 1);
        const auto D13 = t_factor * t_stiff(0, 2);
        const auto D22 = t_factor * t_stiff(1, 1);
        const auto D23 = t_factor * t_stiff(1, 2);
        const auto D33 = t_factor * t_stiff(2, 2);

        const auto S1 = t_factor * t_stress(0);
        const auto S2 = t_factor * t_stress(1);
        const auto S3 = t_factor * t_stress(2);

        const auto D11NX1 = D11 * NX1;
        const auto D11NX2 = D11 * NX2;
        const auto D11NX3 = D11 * NX3;
        const auto D11NX4 = D11 * NX4;
        const auto D11NX5 = D11 * NX5;
        const auto D11NX6 = D11 * NX6;
        const auto D12NX1 = D12 * NX1;
        const auto D12NX2 = D12 * NX2;
        const auto D12NX3 = D12 * NX3;
        const auto D12NX4 = D12 * NX4;
        const auto D12NX5 = D12 * NX5;
        const auto D12NX6 = D12 * NX6;
        const auto D12NY1 = D12 * NY1;
        const auto D12NY2 = D12 * NY2;
        const auto D12NY3 = D12 * NY3;
        const auto D12NY4 = D12 * NY4;
        const auto D12NY5 = D12 * NY5;
        const auto D13NX1 = D13 * NX1;
        const auto D13NX2 = D13 * NX2;
        const auto D13NX3 = D13 * NX3;
        const auto D13NX4 = D13 * NX4;
        const auto D13NX5 = D13 * NX5;
        const auto D13NX6 = D13 * NX6;
        const auto D13NY1 = D13 * NY1;
        const auto D13NY2 = D13 * NY2;
        const auto D13NY3 = D13 * NY3;
        const auto D13NY4 = D13 * NY4;
        const auto D13NY5 = D13 * NY5;
        const auto D13NY6 = D13 * NY6;
        const auto D22NY1 = D22 * NY1;
        const auto D22NY2 = D22 * NY2;
        const auto D22NY3 = D22 * NY3;
        const auto D22NY4 = D22 * NY4;
        const auto D22NY5 = D22 * NY5;
        const auto D22NY6 = D22 * NY6;
        const auto D23NX1 = D23 * NX1;
        const auto D23NX2 = D23 * NX2;
        const auto D23NX3 = D23 * NX3;
        const auto D23NX4 = D23 * NX4;
        const auto D23NX5 = D23 * NX5;
        const auto D23NX6 = D23 * NX6;
        const auto D23NY1 = D23 * NY1;
        const auto D23NY2 = D23 * NY2;
        const auto D23NY3 = D23 * NY3;
        const auto D23NY4 = D23 * NY4;
        const auto D23NY5 = D23 * NY5;
        const auto D23NY6 = D23 * NY6;
        const auto D33NX1 = D33 * NX1;
        const auto D33NX2 = D33 * NX2;
        const auto D33NX3 = D33 * NX3;
        const auto D33NX4 = D33 * NX4;
        const auto D33NX5 = D33 * NX5;
        const auto D33NX6 = D33 * NX6;
        const auto D33NY1 = D33 * NY1;
        const auto D33NY2 = D33 * NY2;
        const auto D33NY3 = D33 * NY3;
        const auto D33NY4 = D33 * NY4;
        const auto D33NY5 = D33 * NY5;
        const auto D33NY6 = D33 * NY6;

        const auto D11NX1D13NY1 = D11NX1 + D13NY1;
        const auto D11NX2D13NY2 = D11NX2 + D13NY2;
        const auto D11NX3D13NY3 = D11NX3 + D13NY3;
        const auto D11NX4D13NY4 = D11NX4 + D13NY4;
        const auto D11NX5D13NY5 = D11NX5 + D13NY5;
        const auto D11NX6D13NY6 = D11NX6 + D13NY6;
        const auto D12NX1D23NY1 = D12NX1 + D23NY1;
        const auto D12NX2D23NY2 = D12NX2 + D23NY2;
        const auto D12NX3D23NY3 = D12NX3 + D23NY3;
        const auto D12NX4D23NY4 = D12NX4 + D23NY4;
        const auto D12NX5D23NY5 = D12NX5 + D23NY5;
        const auto D12NX6D23NY6 = D12NX6 + D23NY6;
        const auto D13NX1D12NY1 = D13NX1 + D12NY1;
        const auto D13NX1D33NY1 = D13NX1 + D33NY1;
        const auto D13NX2D12NY2 = D13NX2 + D12NY2;
        const auto D13NX2D33NY2 = D13NX2 + D33NY2;
        const auto D13NX3D12NY3 = D13NX3 + D12NY3;
        const auto D13NX3D33NY3 = D13NX3 + D33NY3;
        const auto D13NX4D12NY4 = D13NX4 + D12NY4;
        const auto D13NX4D33NY4 = D13NX4 + D33NY4;
        const auto D13NX5D12NY5 = D13NX5 + D12NY5;
        const auto D13NX5D33NY5 = D13NX5 + D33NY5;
        const auto D13NX6D33NY6 = D13NX6 + D33NY6;
        const auto D23NX1D22NY1 = D23NX1 + D22NY1;
        const auto D23NX2D22NY2 = D23NX2 + D22NY2;
        const auto D23NX3D22NY3 = D23NX3 + D22NY3;
        const auto D23NX4D22NY4 = D23NX4 + D22NY4;
        const auto D23NX5D22NY5 = D23NX5 + D22NY5;
        const auto D23NX6D22NY6 = D23NX6 + D22NY6;
        const auto D33NX1D23NY1 = D33NX1 + D23NY1;
        const auto D33NX2D23NY2 = D33NX2 + D23NY2;
        const auto D33NX3D23NY3 = D33NX3 + D23NY3;
        const auto D33NX4D23NY4 = D33NX4 + D23NY4;
        const auto D33NX5D23NY5 = D33NX5 + D23NY5;
        const auto D33NX6D23NY6 = D33NX6 + D23NY6;

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
        stiffness(3, 3) += NX2 * D33NX2D23NY2 + NY2 * D23NX2D22NY2;
        stiffness(3, 4) += NX3 * D13NX2D12NY2 + NY3 * D33NX2D23NY2;
        stiffness(3, 5) += NX3 * D33NX2D23NY2 + NY3 * D23NX2D22NY2;
        stiffness(3, 6) += NX4 * D13NX2D12NY2 + NY4 * D33NX2D23NY2;
        stiffness(3, 7) += NX4 * D33NX2D23NY2 + NY4 * D23NX2D22NY2;
        stiffness(3, 8) += NX5 * D13NX2D12NY2 + NY5 * D33NX2D23NY2;
        stiffness(3, 9) += NX5 * D33NX2D23NY2 + NY5 * D23NX2D22NY2;
        stiffness(3, 10) += NX6 * D13NX2D12NY2 + NY6 * D33NX2D23NY2;
        stiffness(3, 11) += NX6 * D33NX2D23NY2 + NY6 * D23NX2D22NY2;
        stiffness(4, 4) += NX3 * D11NX3D13NY3 + NY3 * D13NX3D33NY3;
        stiffness(4, 5) += NX3 * D13NX3D33NY3 + NY3 * D12NX3D23NY3;
        stiffness(4, 6) += NX4 * D11NX3D13NY3 + NY4 * D13NX3D33NY3;
        stiffness(4, 7) += NX4 * D13NX3D33NY3 + NY4 * D12NX3D23NY3;
        stiffness(4, 8) += NX5 * D11NX3D13NY3 + NY5 * D13NX3D33NY3;
        stiffness(4, 9) += NX5 * D13NX3D33NY3 + NY5 * D12NX3D23NY3;
        stiffness(4, 10) += NX6 * D11NX3D13NY3 + NY6 * D13NX3D33NY3;
        stiffness(4, 11) += NX6 * D13NX3D33NY3 + NY6 * D12NX3D23NY3;
        stiffness(5, 5) += NX3 * D33NX3D23NY3 + NY3 * D23NX3D22NY3;
        stiffness(5, 6) += NX4 * D13NX3D12NY3 + NY4 * D33NX3D23NY3;
        stiffness(5, 7) += NX4 * D33NX3D23NY3 + NY4 * D23NX3D22NY3;
        stiffness(5, 8) += NX5 * D13NX3D12NY3 + NY5 * D33NX3D23NY3;
        stiffness(5, 9) += NX5 * D33NX3D23NY3 + NY5 * D23NX3D22NY3;
        stiffness(5, 10) += NX6 * D13NX3D12NY3 + NY6 * D33NX3D23NY3;
        stiffness(5, 11) += NX6 * D33NX3D23NY3 + NY6 * D23NX3D22NY3;
        stiffness(6, 6) += NX4 * D11NX4D13NY4 + NY4 * D13NX4D33NY4;
        stiffness(6, 7) += NX4 * D13NX4D33NY4 + NY4 * D12NX4D23NY4;
        stiffness(6, 8) += NX5 * D11NX4D13NY4 + NY5 * D13NX4D33NY4;
        stiffness(6, 9) += NX5 * D13NX4D33NY4 + NY5 * D12NX4D23NY4;
        stiffness(6, 10) += NX6 * D11NX4D13NY4 + NY6 * D13NX4D33NY4;
        stiffness(6, 11) += NX6 * D13NX4D33NY4 + NY6 * D12NX4D23NY4;
        stiffness(7, 7) += NX4 * D33NX4D23NY4 + NY4 * D23NX4D22NY4;
        stiffness(7, 8) += NX5 * D13NX4D12NY4 + NY5 * D33NX4D23NY4;
        stiffness(7, 9) += NX5 * D33NX4D23NY4 + NY5 * D23NX4D22NY4;
        stiffness(7, 10) += NX6 * D13NX4D12NY4 + NY6 * D33NX4D23NY4;
        stiffness(7, 11) += NX6 * D33NX4D23NY4 + NY6 * D23NX4D22NY4;
        stiffness(8, 8) += NX5 * D11NX5D13NY5 + NY5 * D13NX5D33NY5;
        stiffness(8, 9) += NX5 * D13NX5D33NY5 + NY5 * D12NX5D23NY5;
        stiffness(8, 10) += NX6 * D11NX5D13NY5 + NY6 * D13NX5D33NY5;
        stiffness(8, 11) += NX6 * D13NX5D33NY5 + NY6 * D12NX5D23NY5;
        stiffness(9, 9) += NX5 * D33NX5D23NY5 + NY5 * D23NX5D22NY5;
        stiffness(9, 10) += NX6 * D13NX5D12NY5 + NY6 * D33NX5D23NY5;
        stiffness(9, 11) += NX6 * D33NX5D23NY5 + NY6 * D23NX5D22NY5;
        stiffness(10, 10) += NX6 * D11NX6D13NY6 + NY6 * D13NX6D33NY6;
        stiffness(10, 11) += NX6 * D13NX6D33NY6 + NY6 * D12NX6D23NY6;
        stiffness(11, 11) += NX6 * D33NX6D23NY6 + NY6 * D23NX6D22NY6;

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
    }

    for(auto I = 0; I < 11; ++I)
        for(auto J = I + 1; J < 12; ++J) stiffness(J, I) = stiffness(I, J);

    return 0;
}

int CP6::commit_status() {
    auto code = 0;
    for(const auto& I : int_pt) code += I.m_material->commit_status();
    return code;
}

int CP6::clear_status() {
    auto code = 0;
    for(const auto& I : int_pt) code += I.m_material->clear_status();
    return code;
}

int CP6::reset_status() {
    auto code = 0;
    for(const auto& I : int_pt) code += I.m_material->reset_status();
    return code;
}

void CP6::print() { suanpan_info("CP6 element.\n"); }
