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

#include "CP4.h"
#include <Domain/DomainBase.h>
#include <Domain/Node.h>
#include <Material/Material2D/Material2D.h>
#include <Toolbox/IntegrationPlan.h>
#include <Toolbox/shapeFunction.hpp>
#include <Toolbox/utility.h>

const unsigned CP4::m_node = 4;
const unsigned CP4::m_dof = 2;
const unsigned CP4::m_size = m_dof * m_node;

CP4::IntegrationPoint::IntegrationPoint(const vec& C, const double W, const double J, unique_ptr<Material>&& M, const mat& PNPXY)
    : coor(C)
    , weight(W)
    , jacob_det(J)
    , m_material(move(M))
    , pn_pxy(PNPXY) {}

CP4::CP4(const unsigned T, const uvec& N, const unsigned M, const double TH, const bool R, const bool F)
    : MaterialElement(T, ET_CP4, m_node, m_dof, N, uvec{ M }, F)
    , thickness(TH)
    , reduced_scheme(R) {}

void CP4::initialize(const shared_ptr<DomainBase>& D) {
    mat ele_coor(m_node, m_dof);
    for(unsigned I = 0; I < m_node; ++I) {
        auto& tmp_coor = node_ptr[I].lock()->get_coordinate();
        for(unsigned J = 0; J < m_dof; ++J) ele_coor(I, J) = tmp_coor(J);
    }

    if(reduced_scheme) {
        hourglassing.zeros(m_size);
        const auto area = .5 * ((ele_coor(2, 0) - ele_coor(0, 0)) * (ele_coor(3, 1) - ele_coor(1, 1)) + (ele_coor(1, 0) - ele_coor(3, 0)) * (ele_coor(2, 1) - ele_coor(0, 1)));
        vec b1(4), b2(4);
        b1(0) = ele_coor(1, 1) - ele_coor(3, 1);
        b1(1) = ele_coor(2, 1) - ele_coor(0, 1);
        b1(2) = ele_coor(3, 1) - ele_coor(1, 1);
        b1(3) = ele_coor(0, 1) - ele_coor(2, 1);
        b2(0) = ele_coor(3, 0) - ele_coor(1, 0);
        b2(1) = ele_coor(0, 0) - ele_coor(2, 0);
        b2(2) = ele_coor(1, 0) - ele_coor(3, 0);
        b2(3) = ele_coor(2, 0) - ele_coor(0, 0);
        const vec h{ std::initializer_list<double>{ 1., -1., 1., -1. } };
        vec gamma = 2. * area * h - dot(h, ele_coor.col(0)) * b1 - dot(h, ele_coor.col(1)) * b2;
        mat t_hourglassing = gamma * gamma.t();
        for(auto I = 0; I < m_node; ++I)
            for(auto J = 0; J < m_node; ++J) hourglassing(m_dof * I + 1, m_dof * J + 1) = hourglassing(m_dof * I, m_dof * J) = t_hourglassing(I, J);
    }

    auto& material_proto = D->get_material(unsigned(material_tag(0)));

    if(material_proto->material_type == MaterialType::D2 && std::dynamic_pointer_cast<Material2D>(material_proto)->plane_type == PlaneType::E) suanpan::hacker(thickness) = 1.;

    const IntegrationPlan plan(2, reduced_scheme ? 1 : 2, IntegrationType::GAUSS);

    int_pt.clear(), int_pt.reserve(plan.n_rows);
    for(unsigned I = 0; I < plan.n_rows; ++I) {
        const vec t_vec{ plan(I, 0), plan(I, 1) };
        const auto pn = shape::quad(t_vec, 1);
        const mat jacob = pn * ele_coor;
        int_pt.emplace_back(t_vec, plan(I, 2), det(jacob), material_proto->get_copy(), solve(jacob, pn));
    }

    if(nlgeom)
        for(auto&& I : int_pt) {
            I.BN.zeros(3, m_size);
            I.BG.zeros(m_dof * m_dof, m_size);
            for(auto J = 0; J < m_node; ++J) {
                I.BG(0, m_dof * J) = I.BG(2, m_dof * J + 1) = I.pn_pxy(0, J);
                I.BG(1, m_dof * J) = I.BG(3, m_dof * J + 1) = I.pn_pxy(1, J);
            }
        }

    initial_mass.zeros(m_size, m_size);
    const auto tmp_density = material_proto->get_parameter();
    if(tmp_density != 0.) {
        for(const auto& I : int_pt) {
            const auto n_int = shape::quad(I.coor, 0);
            const auto tmp_a = tmp_density * I.jacob_det * I.weight * thickness;
            for(auto J = 0; J < m_node; ++J)
                for(auto K = J; K < m_node; ++K) initial_mass(m_dof * J, m_dof * K) += tmp_a * n_int(J) * n_int(K);
        }
        for(auto I = 0; I < m_node * m_dof; I += m_dof) {
            initial_mass(I + 1, I + 1) = initial_mass(I, I);
            for(auto J = I + m_dof; J < m_size; J += m_dof) initial_mass(J, I) = initial_mass(I + 1, J + 1) = initial_mass(J + 1, I + 1) = initial_mass(I, J);
        }
    }
    trial_mass = current_mass = initial_mass;
}

int CP4::update_status() {
    auto code = 0;

    vec t_strain(3);
    mat ele_disp(m_node, m_dof);
    for(auto I = 0; I < m_node; ++I) {
        auto& t_disp = node_ptr[I].lock()->get_trial_displacement();
        for(auto J = 0; J < m_dof; ++J) ele_disp(I, J) = t_disp(J);
    }

    if(nlgeom) trial_geometry.zeros(m_size, m_size);

    trial_stiffness.zeros(m_size, m_size);
    trial_resistance.zeros(m_size);
    for(auto& I : int_pt) {
        if(nlgeom) {
            mat gradient = I.pn_pxy * ele_disp;
            gradient(0, 0) += 1., gradient(1, 1) += 1.;
            mat t_mat = .5 * gradient * gradient.t();
            t_strain(0) = t_mat(0, 0) - .5;
            t_strain(1) = t_mat(1, 1) - .5;
            t_strain(2) = t_mat(0, 1) + t_mat(1, 0);
            for(auto J = 0; J < m_node; ++J) {
                const auto tmp_a = m_dof * J;
                const auto tmp_b = tmp_a + 1;
                I.BN(0, tmp_a) = I.pn_pxy(0, J) * gradient(0, 0);
                I.BN(1, tmp_a) = I.pn_pxy(1, J) * gradient(1, 0);
                I.BN(2, tmp_a) = I.pn_pxy(0, J) * gradient(1, 0) + I.pn_pxy(1, J) * gradient(0, 0);
                I.BN(0, tmp_b) = I.pn_pxy(0, J) * gradient(0, 1);
                I.BN(1, tmp_b) = I.pn_pxy(1, J) * gradient(1, 1);
                I.BN(2, tmp_b) = I.pn_pxy(0, J) * gradient(1, 1) + I.pn_pxy(1, J) * gradient(0, 1);
            }
        } else {
            t_strain.zeros();
            for(auto J = 0; J < m_node; ++J) {
                const auto& t_disp = node_ptr[J].lock()->get_trial_displacement();
                t_strain(0) += t_disp(0) * I.pn_pxy(0, J);
                t_strain(1) += t_disp(1) * I.pn_pxy(1, J);
                t_strain(2) += t_disp(0) * I.pn_pxy(1, J) + t_disp(1) * I.pn_pxy(0, J);
            }
        }

        code += I.m_material->update_trial_status(t_strain);

        const auto t_factor = I.jacob_det * I.weight * thickness;

        auto& t_stiff = I.m_material->get_stiffness();
        auto& t_stress = I.m_material->get_stress();

        if(nlgeom) {
            mat sigma(4, 4, fill::zeros);
            sigma(0, 0) = sigma(2, 2) = t_stress(0);
            sigma(1, 1) = sigma(3, 3) = t_stress(1);
            sigma(0, 1) = sigma(1, 0) = sigma(2, 3) = sigma(3, 2) = t_stress(2);
            trial_geometry += t_factor * I.BG.t() * sigma * I.BG;
            trial_stiffness += t_factor * I.BN.t() * t_stiff * I.BN;
            trial_resistance += t_factor * I.BN.t() * t_stress;
        } else {
            const auto& NX1 = I.pn_pxy(0, 0);
            const auto& NY1 = I.pn_pxy(1, 0);
            const auto& NX2 = I.pn_pxy(0, 1);
            const auto& NY2 = I.pn_pxy(1, 1);
            const auto& NX3 = I.pn_pxy(0, 2);
            const auto& NY3 = I.pn_pxy(1, 2);
            const auto& NX4 = I.pn_pxy(0, 3);
            const auto& NY4 = I.pn_pxy(1, 3);

            const auto D11 = t_factor * t_stiff(0, 0);
            const auto D12 = t_factor * t_stiff(0, 1);
            const auto D13 = t_factor * t_stiff(0, 2);
            const auto D22 = t_factor * t_stiff(1, 1);
            const auto D23 = t_factor * t_stiff(1, 2);
            const auto D33 = t_factor * t_stiff(2, 2);

            const auto S1 = t_factor * t_stress(0);
            const auto S2 = t_factor * t_stress(1);
            const auto S3 = t_factor * t_stress(2);

            // 111+59 (264+176)
            const auto D11NX1 = D11 * NX1;
            const auto D11NX2 = D11 * NX2;
            const auto D11NX3 = D11 * NX3;
            const auto D11NX4 = D11 * NX4;
            const auto D12NX1 = D12 * NX1;
            const auto D12NX2 = D12 * NX2;
            const auto D12NX3 = D12 * NX3;
            const auto D12NX4 = D12 * NX4;
            const auto D13NX1 = D13 * NX1;
            const auto D13NX2 = D13 * NX2;
            const auto D13NX3 = D13 * NX3;
            const auto D13NX4 = D13 * NX4;
            const auto D23NX1 = D23 * NX1;
            const auto D23NX2 = D23 * NX2;
            const auto D23NX3 = D23 * NX3;
            const auto D23NX4 = D23 * NX4;
            const auto D33NX1 = D33 * NX1;
            const auto D33NX2 = D33 * NX2;
            const auto D33NX3 = D33 * NX3;
            const auto D33NX4 = D33 * NX4;
            const auto D12NY1 = D12 * NY1;
            const auto D12NY2 = D12 * NY2;
            const auto D12NY3 = D12 * NY3;
            const auto D13NY1 = D13 * NY1;
            const auto D13NY2 = D13 * NY2;
            const auto D13NY3 = D13 * NY3;
            const auto D13NY4 = D13 * NY4;
            const auto D22NY1 = D22 * NY1;
            const auto D22NY2 = D22 * NY2;
            const auto D22NY3 = D22 * NY3;
            const auto D22NY4 = D22 * NY4;
            const auto D23NY1 = D23 * NY1;
            const auto D23NY2 = D23 * NY2;
            const auto D23NY3 = D23 * NY3;
            const auto D23NY4 = D23 * NY4;
            const auto D33NY1 = D33 * NY1;
            const auto D33NY2 = D33 * NY2;
            const auto D33NY3 = D33 * NY3;
            const auto D33NY4 = D33 * NY4;

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
            const auto D12NX1D23NY1 = D12NX1 + D23NY1;
            const auto D23NX1D22NY1 = D23NX1 + D22NY1;
            const auto D12NX2D23NY2 = D12NX2 + D23NY2;
            const auto D23NX2D22NY2 = D23NX2 + D22NY2;
            const auto D12NX3D23NY3 = D12NX3 + D23NY3;
            const auto D23NX3D22NY3 = D23NX3 + D22NY3;
            const auto D12NX4D23NY4 = D12NX4 + D23NY4;
            const auto D23NX4D22NY4 = D23NX4 + D22NY4;

            trial_stiffness(0, 0) += NX1 * D11NX1D13NY1 + NY1 * D13NX1D33NY1;
            trial_stiffness(0, 1) += NX1 * D13NX1D33NY1 + NY1 * D12NX1D23NY1;
            trial_stiffness(0, 2) += NX2 * D11NX1D13NY1 + NY2 * D13NX1D33NY1;
            trial_stiffness(0, 3) += NX2 * D13NX1D33NY1 + NY2 * D12NX1D23NY1;
            trial_stiffness(0, 4) += NX3 * D11NX1D13NY1 + NY3 * D13NX1D33NY1;
            trial_stiffness(0, 5) += NX3 * D13NX1D33NY1 + NY3 * D12NX1D23NY1;
            trial_stiffness(0, 6) += NX4 * D11NX1D13NY1 + NY4 * D13NX1D33NY1;
            trial_stiffness(0, 7) += NX4 * D13NX1D33NY1 + NY4 * D12NX1D23NY1;
            trial_stiffness(1, 1) += NX1 * D33NX1D23NY1 + NY1 * D23NX1D22NY1;
            trial_stiffness(1, 2) += NX2 * D13NX1D12NY1 + NY2 * D33NX1D23NY1;
            trial_stiffness(1, 3) += NX2 * D33NX1D23NY1 + NY2 * D23NX1D22NY1;
            trial_stiffness(1, 4) += NX3 * D13NX1D12NY1 + NY3 * D33NX1D23NY1;
            trial_stiffness(1, 5) += NX3 * D33NX1D23NY1 + NY3 * D23NX1D22NY1;
            trial_stiffness(1, 6) += NX4 * D13NX1D12NY1 + NY4 * D33NX1D23NY1;
            trial_stiffness(1, 7) += NX4 * D33NX1D23NY1 + NY4 * D23NX1D22NY1;
            trial_stiffness(2, 2) += NX2 * D11NX2D13NY2 + NY2 * D13NX2D33NY2;
            trial_stiffness(2, 3) += NX2 * D13NX2D33NY2 + NY2 * D12NX2D23NY2;
            trial_stiffness(2, 4) += NX3 * D11NX2D13NY2 + NY3 * D13NX2D33NY2;
            trial_stiffness(2, 5) += NX3 * D13NX2D33NY2 + NY3 * D12NX2D23NY2;
            trial_stiffness(2, 6) += NX4 * D11NX2D13NY2 + NY4 * D13NX2D33NY2;
            trial_stiffness(2, 7) += NX4 * D13NX2D33NY2 + NY4 * D12NX2D23NY2;
            trial_stiffness(3, 3) += NX2 * D33NX2D23NY2 + NY2 * D23NX2D22NY2;
            trial_stiffness(3, 4) += NX3 * D13NX2D12NY2 + NY3 * D33NX2D23NY2;
            trial_stiffness(3, 5) += NX3 * D33NX2D23NY2 + NY3 * D23NX2D22NY2;
            trial_stiffness(3, 6) += NX4 * D13NX2D12NY2 + NY4 * D33NX2D23NY2;
            trial_stiffness(3, 7) += NX4 * D33NX2D23NY2 + NY4 * D23NX2D22NY2;
            trial_stiffness(4, 4) += NX3 * D11NX3D13NY3 + NY3 * D13NX3D33NY3;
            trial_stiffness(4, 5) += NX3 * D13NX3D33NY3 + NY3 * D12NX3D23NY3;
            trial_stiffness(4, 6) += NX4 * D11NX3D13NY3 + NY4 * D13NX3D33NY3;
            trial_stiffness(4, 7) += NX4 * D13NX3D33NY3 + NY4 * D12NX3D23NY3;
            trial_stiffness(5, 5) += NX3 * D33NX3D23NY3 + NY3 * D23NX3D22NY3;
            trial_stiffness(5, 6) += NX4 * D13NX3D12NY3 + NY4 * D33NX3D23NY3;
            trial_stiffness(5, 7) += NX4 * D33NX3D23NY3 + NY4 * D23NX3D22NY3;
            trial_stiffness(6, 6) += NX4 * D11NX4D13NY4 + NY4 * D13NX4D33NY4;
            trial_stiffness(6, 7) += NX4 * D13NX4D33NY4 + NY4 * D12NX4D23NY4;
            trial_stiffness(7, 7) += NX4 * D33NX4D23NY4 + NY4 * D23NX4D22NY4;

            trial_resistance(0) += NX1 * S1 + NY1 * S3;
            trial_resistance(1) += NX1 * S3 + NY1 * S2;
            trial_resistance(2) += NX2 * S1 + NY2 * S3;
            trial_resistance(3) += NX2 * S3 + NY2 * S2;
            trial_resistance(4) += NX3 * S1 + NY3 * S3;
            trial_resistance(5) += NX3 * S3 + NY3 * S2;
            trial_resistance(6) += NX4 * S1 + NY4 * S3;
            trial_resistance(7) += NX4 * S3 + NY4 * S2;
        }
    }

    if(nlgeom)
        trial_stiffness += trial_geometry;
    else
        for(auto I = 0; I < 7; ++I)
            for(auto J = I + 1; J < 8; ++J) trial_stiffness(J, I) = trial_stiffness(I, J);

    if(reduced_scheme) {
        trial_stiffness += hourglassing;
        trial_resistance += hourglassing * vectorise(ele_disp.t());
    }

    if(initial_stiffness.is_empty()) initial_stiffness = trial_stiffness;

    return code;
}

int CP4::commit_status() {
    auto code = 0;
    for(const auto& I : int_pt) code += I.m_material->commit_status();
    return code;
}

int CP4::clear_status() {
    auto code = 0;
    for(const auto& I : int_pt) code += I.m_material->clear_status();
    return code;
}

int CP4::reset_status() {
    auto code = 0;
    for(const auto& I : int_pt) code += I.m_material->reset_status();
    return code;
}

vector<vec> CP4::record(const OutputType& P) {
    vector<vec> output;
    output.reserve(int_pt.size());

    for(const auto& I : int_pt)
        for(const auto& J : I.m_material->record(P)) output.emplace_back(J);

    return output;
}

void CP4::print() {
    suanpan_info("Element %u is a four-node membrane element (CP4)%s.\n", get_tag(), nlgeom ? " with nonlinear geomotry (TL formulation)" : "");
    suanpan_info("The nodes connected are:\n");
    node_encoding.t().print();
}
