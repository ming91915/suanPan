#include "CP4.h"
#include <Toolbox/integrationPlan.h>
#include <Toolbox/shapeFunction.hpp>

const unsigned CP4::m_node = 4;
const unsigned CP4::m_dof = 2;

CP4::CP4(const unsigned& T, const uvec& N, const unsigned& M, const double& TH, const bool& R, const bool& F)
    : Element(T, ET_CP4, m_node, m_dof, N, uvec{ M }, F)
    , thickness(TH)
    , reduced_scheme(R) {}

void CP4::initialize(const shared_ptr<Domain>& D) {
    const auto& material_proto = D->get_material(static_cast<unsigned>(material_tag(0)));

    const unsigned order = reduced_scheme ? 1 : 2;
    const integrationPlan plan(2, order, 1);

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

    for(const auto& I : int_pt) {
        const auto pn = shapeFunctionQuad(I->coor, 1);
        const mat jacob = pn * ele_coor;
        I->jacob_det = det(jacob);
        I->pn_pxy = solve(jacob, pn);
    }

    mass.zeros();
    const auto tmp_density = material_proto->get_parameter();
    if(tmp_density != 0.) {
        for(const auto& I : int_pt) {
            const auto n_int = shapeFunctionQuad(I->coor, 0);
            const auto tmp_a = tmp_density * I->jacob_det * I->weight * thickness;
            for(auto J = 0; J < m_node * m_dof; ++J)
                for(auto K = J; K < m_node * m_dof; ++K) mass(m_dof * J, m_dof * K) += tmp_a * n_int(J) * n_int(K);
        }

        for(auto I = 0; I < 8; I += m_dof) {
            mass(I + 1, I + 1) = mass(I, I);
            for(auto J = I + m_dof; J < 8; J += m_dof) {
                mass(J, I) = mass(I, J);
                mass(I + 1, J + 1) = mass(I, J);
                mass(J + 1, I + 1) = mass(I, J);
            }
        }
    }
}

int CP4::update_status() {
    auto code = 0;

    vec t_strain(3);

    stiffness.zeros();
    resistance.zeros();
    for(const auto& I : int_pt) {
        t_strain.zeros();
        for(auto J = 0; J < m_node; ++J) {
            const auto& t_disp = node_ptr[J].lock()->get_trial_displacement();
            t_strain(0) += t_disp(0) * I->pn_pxy(0, J);
            t_strain(1) += t_disp(1) * I->pn_pxy(1, J);
            t_strain(2) += t_disp(0) * I->pn_pxy(1, J) + t_disp(1) * I->pn_pxy(0, J);
        }
        code += I->m_material->update_trial_status(t_strain);

        const auto tmp_factor = I->jacob_det * I->weight * thickness;

        auto& tmp_stiff = I->m_material->get_stiffness();
        auto& tmp_stress = I->m_material->get_stress();

        const auto& NX1 = I->pn_pxy(0, 0);
        const auto& NY1 = I->pn_pxy(1, 0);
        const auto& NX2 = I->pn_pxy(0, 1);
        const auto& NY2 = I->pn_pxy(1, 1);
        const auto& NX3 = I->pn_pxy(0, 2);
        const auto& NY3 = I->pn_pxy(1, 2);
        const auto& NX4 = I->pn_pxy(0, 3);
        const auto& NY4 = I->pn_pxy(1, 3);

        const auto D11 = tmp_factor * tmp_stiff(0, 0);
        const auto D12 = tmp_factor * tmp_stiff(0, 1);
        const auto D13 = tmp_factor * tmp_stiff(0, 2);
        const auto D22 = tmp_factor * tmp_stiff(1, 1);
        const auto D23 = tmp_factor * tmp_stiff(1, 2);
        const auto D33 = tmp_factor * tmp_stiff(2, 2);

        const auto S1 = tmp_factor * tmp_stress(0);
        const auto S2 = tmp_factor * tmp_stress(1);
        const auto S3 = tmp_factor * tmp_stress(2);

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

        stiffness(0, 0) += NX1 * D11NX1D13NY1 + NY1 * D13NX1D33NY1;
        stiffness(0, 1) += NX1 * D13NX1D33NY1 + NY1 * D12NX1D23NY1;
        stiffness(0, 2) += NX2 * D11NX1D13NY1 + NY2 * D13NX1D33NY1;
        stiffness(0, 3) += NX2 * D13NX1D33NY1 + NY2 * D12NX1D23NY1;
        stiffness(0, 4) += NX3 * D11NX1D13NY1 + NY3 * D13NX1D33NY1;
        stiffness(0, 5) += NX3 * D13NX1D33NY1 + NY3 * D12NX1D23NY1;
        stiffness(0, 6) += NX4 * D11NX1D13NY1 + NY4 * D13NX1D33NY1;
        stiffness(0, 7) += NX4 * D13NX1D33NY1 + NY4 * D12NX1D23NY1;

        stiffness(1, 1) += NX1 * D33NX1D23NY1 + NY1 * D23NX1D22NY1;
        stiffness(1, 2) += NX2 * D13NX1D12NY1 + NY2 * D33NX1D23NY1;
        stiffness(1, 3) += NX2 * D33NX1D23NY1 + NY2 * D23NX1D22NY1;
        stiffness(1, 4) += NX3 * D13NX1D12NY1 + NY3 * D33NX1D23NY1;
        stiffness(1, 5) += NX3 * D33NX1D23NY1 + NY3 * D23NX1D22NY1;
        stiffness(1, 6) += NX4 * D13NX1D12NY1 + NY4 * D33NX1D23NY1;
        stiffness(1, 7) += NX4 * D33NX1D23NY1 + NY4 * D23NX1D22NY1;

        stiffness(2, 2) += NX2 * D11NX2D13NY2 + NY2 * D13NX2D33NY2;
        stiffness(2, 3) += NX2 * D13NX2D33NY2 + NY2 * D12NX2D23NY2;
        stiffness(2, 4) += NX3 * D11NX2D13NY2 + NY3 * D13NX2D33NY2;
        stiffness(2, 5) += NX3 * D13NX2D33NY2 + NY3 * D12NX2D23NY2;
        stiffness(2, 6) += NX4 * D11NX2D13NY2 + NY4 * D13NX2D33NY2;
        stiffness(2, 7) += NX4 * D13NX2D33NY2 + NY4 * D12NX2D23NY2;

        stiffness(3, 3) += NX2 * D33NX2D23NY2 + NY2 * D23NX2D22NY2;
        stiffness(3, 4) += NX3 * D13NX2D12NY2 + NY3 * D33NX2D23NY2;
        stiffness(3, 5) += NX3 * D33NX2D23NY2 + NY3 * D23NX2D22NY2;
        stiffness(3, 6) += NX4 * D13NX2D12NY2 + NY4 * D33NX2D23NY2;
        stiffness(3, 7) += NX4 * D33NX2D23NY2 + NY4 * D23NX2D22NY2;

        stiffness(4, 4) += NX3 * D11NX3D13NY3 + NY3 * D13NX3D33NY3;
        stiffness(4, 5) += NX3 * D13NX3D33NY3 + NY3 * D12NX3D23NY3;
        stiffness(4, 6) += NX4 * D11NX3D13NY3 + NY4 * D13NX3D33NY3;
        stiffness(4, 7) += NX4 * D13NX3D33NY3 + NY4 * D12NX3D23NY3;

        stiffness(5, 5) += NX3 * D33NX3D23NY3 + NY3 * D23NX3D22NY3;
        stiffness(5, 6) += NX4 * D13NX3D12NY3 + NY4 * D33NX3D23NY3;
        stiffness(5, 7) += NX4 * D33NX3D23NY3 + NY4 * D23NX3D22NY3;

        stiffness(6, 6) += NX4 * D11NX4D13NY4 + NY4 * D13NX4D33NY4;
        stiffness(6, 7) += NX4 * D13NX4D33NY4 + NY4 * D12NX4D23NY4;

        stiffness(7, 7) += NX4 * D33NX4D23NY4 + NY4 * D23NX4D22NY4;

        resistance(0) += NX1 * S1 + NY1 * S3;
        resistance(1) += NX1 * S3 + NY1 * S2;
        resistance(2) += NX2 * S1 + NY2 * S3;
        resistance(3) += NX2 * S3 + NY2 * S2;
        resistance(4) += NX3 * S1 + NY3 * S3;
        resistance(5) += NX3 * S3 + NY3 * S2;
        resistance(6) += NX4 * S1 + NY4 * S3;
        resistance(7) += NX4 * S3 + NY4 * S2;
    }

    for(auto I = 0; I < 7; ++I)
        for(auto J = I + 1; J < 8; ++J) stiffness(J, I) = stiffness(I, J);

    return code;
}

int CP4::commit_status() {
    auto code = 0;
    for(const auto& I : int_pt) code += I->m_material->commit_status();
    return code;
}

int CP4::clear_status() {
    auto code = 0;
    for(const auto& I : int_pt) code += I->m_material->clear_status();
    return code;
}

int CP4::reset_status() {
    auto code = 0;
    for(const auto& I : int_pt) code += I->m_material->reset_status();
    return code;
}

void CP4::print() { suanpan_info("This is a CP4 element.\n"); }
