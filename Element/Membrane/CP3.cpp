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

#include "CP3.h"
#include <Domain/DomainBase.h>
#include <Domain/Node.h>
#include <Material/Material2D/Material2D.h>
#include <Toolbox/utility.h>

const unsigned CP3::m_node = 3;
const unsigned CP3::m_dof = 2;

CP3::CP3(const unsigned& T, const uvec& NT, const unsigned& MT, const double& TH)
    : Element(T, ET_CP3, m_node, m_dof, NT, uvec{ MT }, false)
    , thickness(TH) {}

void CP3::initialize(const shared_ptr<DomainBase>& D) {
    auto& material_proto = D->get_material(unsigned(material_tag(0)));

    if(material_proto->material_type == MaterialType::D2 && std::dynamic_pointer_cast<Material2D>(material_proto)->plane_type == PlaneType::E) modifier(thickness) = 1.;

    m_material = material_proto->get_copy();

    mat ele_coor(m_node, m_node, fill::ones);
    for(auto I = 0; I < m_node; ++I) {
        auto& tmp_coor = node_ptr[I].lock()->get_coordinate();
        for(auto J = 0; J < 2; ++J) ele_coor(I, J + 1) = tmp_coor(J);
    }

    area = det(ele_coor) / 2.;

    const mat inv_coor = inv(ele_coor);
    pn_pxy = inv_coor.rows(1, 2);

    mass.zeros();
    auto t_density = m_material->get_parameter();
    if(t_density != 0.) {
        t_density *= area * thickness;
        const vec n = mean(ele_coor) * inv_coor;
        for(auto I = 0; I < m_node; ++I)
            for(auto J = I; J < m_node; ++J) mass(m_dof * I, m_dof * J) += t_density * n(I) * n(J);
        for(auto I = 0; I < m_node * m_dof; I += m_dof) {
            mass(I + 1, I + 1) = mass(I, I);
            for(auto J = I + m_dof; J < m_node * m_dof; J += m_dof) mass(J, I) = mass(I + 1, J + 1) = mass(J + 1, I + 1) = mass(I, J);
        }
    }
}

int CP3::update_status() {
    vec t_strain(3, fill::zeros);
    for(auto J = 0; J < m_node; ++J) {
        const auto& t_disp = node_ptr[J].lock()->get_trial_displacement();
        t_strain(0) += t_disp(0) * pn_pxy(0, J);
        t_strain(1) += t_disp(1) * pn_pxy(1, J);
        t_strain(2) += t_disp(0) * pn_pxy(1, J) + t_disp(1) * pn_pxy(0, J);
    }
    m_material->update_trial_status(t_strain);

    const auto t_factor = area * thickness;

    auto& t_stiff = m_material->get_stiffness();
    auto& t_stress = m_material->get_stress();

    const auto& NX1 = pn_pxy(0, 0);
    const auto& NY1 = pn_pxy(1, 0);
    const auto& NX2 = pn_pxy(0, 1);
    const auto& NY2 = pn_pxy(1, 1);
    const auto& NX3 = pn_pxy(0, 2);
    const auto& NY3 = pn_pxy(1, 2);

    const auto D11 = t_factor * t_stiff(0, 0);
    const auto D12 = t_factor * t_stiff(0, 1);
    const auto D13 = t_factor * t_stiff(0, 2);
    const auto D22 = t_factor * t_stiff(1, 1);
    const auto D23 = t_factor * t_stiff(1, 2);
    const auto D33 = t_factor * t_stiff(2, 2);

    const auto S1 = t_factor * t_stress(0);
    const auto S2 = t_factor * t_stress(1);
    const auto S3 = t_factor * t_stress(2);

    // 71+38 (162+108)
    const auto D11NX1 = D11 * NX1;
    const auto D11NX2 = D11 * NX2;
    const auto D11NX3 = D11 * NX3;
    const auto D12NX1 = D12 * NX1;
    const auto D12NX2 = D12 * NX2;
    const auto D12NX3 = D12 * NX3;
    const auto D12NY1 = D12 * NY1;
    const auto D12NY2 = D12 * NY2;
    const auto D13NX1 = D13 * NX1;
    const auto D13NX2 = D13 * NX2;
    const auto D13NX3 = D13 * NX3;
    const auto D13NY1 = D13 * NY1;
    const auto D13NY2 = D13 * NY2;
    const auto D13NY3 = D13 * NY3;
    const auto D22NY1 = D22 * NY1;
    const auto D22NY2 = D22 * NY2;
    const auto D22NY3 = D22 * NY3;
    const auto D23NX1 = D23 * NX1;
    const auto D23NX2 = D23 * NX2;
    const auto D23NX3 = D23 * NX3;
    const auto D23NY1 = D23 * NY1;
    const auto D23NY2 = D23 * NY2;
    const auto D23NY3 = D23 * NY3;
    const auto D33NX1 = D33 * NX1;
    const auto D33NX2 = D33 * NX2;
    const auto D33NX3 = D33 * NX3;
    const auto D33NY1 = D33 * NY1;
    const auto D33NY2 = D33 * NY2;
    const auto D33NY3 = D33 * NY3;

    const auto D11NX1D13NY1 = D11NX1 + D13NY1;
    const auto D11NX2D13NY2 = D11NX2 + D13NY2;
    const auto D11NX3D13NY3 = D11NX3 + D13NY3;
    const auto D12NX1D23NY1 = D12NX1 + D23NY1;
    const auto D12NX2D23NY2 = D12NX2 + D23NY2;
    const auto D12NX3D23NY3 = D12NX3 + D23NY3;
    const auto D13NX1D12NY1 = D13NX1 + D12NY1;
    const auto D13NX1D33NY1 = D13NX1 + D33NY1;
    const auto D13NX2D12NY2 = D13NX2 + D12NY2;
    const auto D13NX2D33NY2 = D13NX2 + D33NY2;
    const auto D13NX3D33NY3 = D13NX3 + D33NY3;
    const auto D23NX1D22NY1 = D23NX1 + D22NY1;
    const auto D23NX2D22NY2 = D23NX2 + D22NY2;
    const auto D23NX3D22NY3 = D23NX3 + D22NY3;
    const auto D33NX1D23NY1 = D33NX1 + D23NY1;
    const auto D33NX2D23NY2 = D33NX2 + D23NY2;
    const auto D33NX3D23NY3 = D33NX3 + D23NY3;

    stiffness(0, 0) = NX1 * D11NX1D13NY1 + NY1 * D13NX1D33NY1;
    stiffness(0, 1) = NX1 * D13NX1D33NY1 + NY1 * D12NX1D23NY1;
    stiffness(0, 2) = NX2 * D11NX1D13NY1 + NY2 * D13NX1D33NY1;
    stiffness(0, 3) = NX2 * D13NX1D33NY1 + NY2 * D12NX1D23NY1;
    stiffness(0, 4) = NX3 * D11NX1D13NY1 + NY3 * D13NX1D33NY1;
    stiffness(0, 5) = NX3 * D13NX1D33NY1 + NY3 * D12NX1D23NY1;
    stiffness(1, 1) = NX1 * D33NX1D23NY1 + NY1 * D23NX1D22NY1;
    stiffness(1, 2) = NX2 * D13NX1D12NY1 + NY2 * D33NX1D23NY1;
    stiffness(1, 3) = NX2 * D33NX1D23NY1 + NY2 * D23NX1D22NY1;
    stiffness(1, 4) = NX3 * D13NX1D12NY1 + NY3 * D33NX1D23NY1;
    stiffness(1, 5) = NX3 * D33NX1D23NY1 + NY3 * D23NX1D22NY1;
    stiffness(2, 2) = NX2 * D11NX2D13NY2 + NY2 * D13NX2D33NY2;
    stiffness(2, 3) = NX2 * D13NX2D33NY2 + NY2 * D12NX2D23NY2;
    stiffness(2, 4) = NX3 * D11NX2D13NY2 + NY3 * D13NX2D33NY2;
    stiffness(2, 5) = NX3 * D13NX2D33NY2 + NY3 * D12NX2D23NY2;
    stiffness(3, 3) = NX2 * D33NX2D23NY2 + NY2 * D23NX2D22NY2;
    stiffness(3, 4) = NX3 * D13NX2D12NY2 + NY3 * D33NX2D23NY2;
    stiffness(3, 5) = NX3 * D33NX2D23NY2 + NY3 * D23NX2D22NY2;
    stiffness(4, 4) = NX3 * D11NX3D13NY3 + NY3 * D13NX3D33NY3;
    stiffness(4, 5) = NX3 * D13NX3D33NY3 + NY3 * D12NX3D23NY3;
    stiffness(5, 5) = NX3 * D33NX3D23NY3 + NY3 * D23NX3D22NY3;

    resistance(0) = NX1 * S1 + NY1 * S3;
    resistance(1) = NX1 * S3 + NY1 * S2;
    resistance(2) = NX2 * S1 + NY2 * S3;
    resistance(3) = NX2 * S3 + NY2 * S2;
    resistance(4) = NX3 * S1 + NY3 * S3;
    resistance(5) = NX3 * S3 + NY3 * S2;

    for(auto I = 0; I < 5; ++I)
        for(auto J = I + 1; J < 6; ++J) stiffness(J, I) = stiffness(I, J);

    return 0;
}

int CP3::commit_status() { return m_material->commit_status(); }

int CP3::clear_status() { return m_material->clear_status(); }

int CP3::reset_status() { return m_material->reset_status(); }

void CP3::print() { suanpan_info("CPS3 element.\n"); }
