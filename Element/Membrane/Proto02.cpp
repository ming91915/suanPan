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

#include "Proto02.h"
#include <Domain/DomainBase.h>
#include <Domain/Node.h>
#include <Material/Material2D/Material2D.h>
#include <Recorder/OutputList.h>
#include <Toolbox/IntegrationPlan.h>
#include <Toolbox/shapeFunction.hpp>
#include <Toolbox/tensorToolbox.h>

const unsigned Proto02::m_node = 4;
const unsigned Proto02::m_dof = 3;
mat Proto02::mapping;

Proto02::Proto02(const unsigned& T, const uvec& N, const unsigned& M, const double& TH)
    : Element(T, ET_PROTO02, m_node, m_dof, N, uvec{ M })
    , thickness(TH) {}

void Proto02::initialize(const shared_ptr<DomainBase>& D) {
    if(mapping.is_empty()) {
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

    auto& ini_stiffness = material_proto->get_initial_stiffness();

    const IntegrationPlan plan(2, 3, IntegrationType::GAUSS);
    int_pt.clear();
    for(unsigned I = 0; I < plan.n_rows; ++I) {
        int_pt.emplace_back(make_unique<IntegrationPoint>());
        int_pt[I]->coor.zeros(2);
        for(unsigned J = 0; J < 2; ++J) int_pt[I]->coor(J) = plan(I, J);
        int_pt[I]->weight = plan(I, 2);
        int_pt[I]->m_material = material_proto->get_copy();
    }

    mat ele_coor(m_node, 2);
    for(auto I = 0; I < m_node; ++I) {
        auto& t_coor = node_ptr[I].lock()->get_coordinate();
        for(auto J = 0; J < 2; ++J) ele_coor(I, J) = t_coor(J);
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

    const mat t_factor = trans(mapping * ele_coor);

    vec disp_mode(4, fill::zeros);

    mat H(11, 11, fill::zeros);
    mat L(11, 12, fill::zeros);
    mat LI(11, 2, fill::zeros);
    for(const auto& I : int_pt) {
        const auto pn = shape::quad(I->coor, 1);

        const mat jacob = pn * ele_coor;

        I->jacob_det = det(jacob);

        const auto& X = I->coor(0);
        const auto& Y = I->coor(1);

        disp_mode(1) = X;
        disp_mode(2) = Y;
        disp_mode(3) = X * Y;

        I->P = shape::stress11(t_factor * disp_mode);

        I->A = solve(ini_stiffness, I->P);

        I->B = zeros(3, m_node * m_dof);

        const auto X2 = 2. * X;
        const auto Y2 = 2. * Y;
        const auto XP = X + 1.;
        const auto XM = X - 1.;
        const auto YP = Y + 1.;
        const auto YM = Y - 1.;

        pnt(0, 0) = YM * (LX4 * YP - LX1 * X2);
        pnt(0, 1) = YM * (LX2 * YP + LX1 * X2);
        pnt(0, 2) = YP * (LX3 * X2 - LX2 * YM);
        pnt(0, 3) = -YP * (LX3 * X2 + LX4 * YM);
        pnt(0, 4) = YM * (LY4 * YP - LY1 * X2);
        pnt(0, 5) = YM * (LY2 * YP + LY1 * X2);
        pnt(0, 6) = YP * (LY3 * X2 - LY2 * YM);
        pnt(0, 7) = -YP * (LY3 * X2 + LY4 * YM);
        pnt(1, 0) = XM * (LX4 * Y2 - LX1 * XP);
        pnt(1, 1) = XP * (LX1 * XM + LX2 * Y2);
        pnt(1, 2) = XP * (LX3 * XM - LX2 * Y2);
        pnt(1, 3) = -XM * (LX3 * XP + LX4 * Y2);
        pnt(1, 4) = XM * (LY4 * Y2 - LY1 * XP);
        pnt(1, 5) = XP * (LY1 * XM + LY2 * Y2);
        pnt(1, 6) = XP * (LY3 * XM - LY2 * Y2);
        pnt(1, 7) = -XM * (LY3 * XP + LY4 * Y2);

        const mat pnt_pxy = solve(jacob, pnt / 16.);

        const mat pn_pxy = solve(jacob, pn);
        for(unsigned J = 0; J < m_node; ++J) {
            I->B(2, m_dof * J + 1) = pn_pxy(0, J);
            I->B(2, m_dof * J) = pn_pxy(1, J);
            I->B(1, m_dof * J + 1) = pn_pxy(1, J);
            I->B(0, m_dof * J) = pn_pxy(0, J);
            I->B(0, m_dof * J + 2) = pnt_pxy(0, J);
            I->B(1, m_dof * J + 2) = pnt_pxy(1, J + 4);
            I->B(2, m_dof * J + 2) = pnt_pxy(0, J + 4) + pnt_pxy(1, J);
        }

        I->BI = zeros(3, 2);
        vec t_vec(2);
        t_vec(0) = 3. * X * X - 1.;
        t_vec(1) = 3. * Y * Y - 1.;
        t_vec = solve(jacob, t_vec);
        I->BI(0, 0) = t_vec(0);
        I->BI(1, 1) = t_vec(1);
        I->BI(2, 0) = I->BI(1, 1);
        I->BI(2, 1) = I->BI(0, 0);

        const mat t_mat = I->P.t() * I->jacob_det * I->weight * thickness;
        H += t_mat * I->A;
        L += t_mat * I->B;
        LI += t_mat * I->BI;
    }

    mass.zeros();
    const auto t_density = material_proto->get_parameter() * thickness;
    if(t_density != 0.) {
        for(const auto& I : int_pt) {
            const auto n_int = shape::quad(I->coor, 0);
            const auto tmp_a = t_density * I->jacob_det * I->weight;
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

    inv(HI, H);
    solve(HIL, H, L);
    solve(HILI, H, LI);

    const mat QT = HILI.t() * H * HILI;
    const mat TT = HILI.t() * H * HIL;

    initial_qtitt = solve(QT, TT);
    current_qtitt = initial_qtitt;
    trial_qtitt = initial_qtitt;

    current_ht = H;
    trial_ht = H;

    initial_stiffness = HIL.t() * H * HIL - TT.t() * initial_qtitt;

    current_qtifi.zeros(2);
    trial_qtifi.zeros(2);

    current_disp.zeros(12);
    current_lambda.zeros(2);
    current_alpha.zeros(11);
    current_beta.zeros(11);

    trial_disp.zeros(12);
    trial_lambda.zeros(2);
    trial_alpha.zeros(11);
    trial_beta.zeros(11);
}

int Proto02::update_status() {
    auto idx = 0;
    for(const auto& t_ptr : node_ptr) {
        auto& t_disp = t_ptr.lock()->get_trial_displacement();
        for(auto pos = 0; pos < m_dof; ++pos) trial_disp(idx++) = t_disp(pos);
    }

    const vec incre_disp = trial_disp - current_disp;
    const vec incre_lambda = -trial_qtitt * incre_disp - trial_qtifi; // eq. 65
    const vec incre_alpha = HIL * incre_disp + HILI * incre_lambda;   // eq. 57
    const vec incre_beta = HI * trial_ht * incre_alpha;               // eq. 58

    trial_lambda += incre_lambda; // eq. 66
    trial_alpha += incre_alpha;   // eq. 46
    trial_beta += incre_beta;     // eq. 46

    trial_ht.zeros();
    resistance.zeros();
    vec FI(2, fill::zeros);
    auto code = 0;
    for(const auto& t_pt : int_pt) {
        code += t_pt->m_material->update_trial_status(t_pt->A * trial_alpha);
        const auto t_factor = t_pt->jacob_det * t_pt->weight * thickness;
        const vec t_vector = t_pt->P * trial_beta * t_factor;
        trial_ht += t_pt->A.t() * t_pt->m_material->get_stiffness() * t_pt->A * t_factor; // eq. 56
        resistance += t_pt->B.t() * t_vector;                                             // eq. 54
        FI += t_pt->BI.t() * t_vector;                                                    // eq. 54
    }

    const mat QT = HILI.t() * trial_ht * HILI;                   // eq. 60
    const mat TT = HILI.t() * trial_ht * HIL;                    // eq. 60
    solve(trial_qtitt, QT, TT);                                  // eq. 65
    solve(trial_qtifi, QT, FI);                                  // eq. 65
    resistance -= TT.t() * trial_qtifi;                          // eq. 64
    stiffness = HIL.t() * trial_ht * HIL - TT.t() * trial_qtitt; // eq. 61

    current_disp = trial_disp;

    return code;
}

int Proto02::commit_status() {
    current_lambda = trial_lambda;
    current_alpha = trial_alpha;
    current_beta = trial_beta;

    current_qtitt = trial_qtitt;
    current_qtifi = trial_qtifi;

    current_ht = trial_ht;

    auto code = 0;
    for(const auto& I : int_pt) code += I->m_material->commit_status();
    return code;
}

int Proto02::clear_status() {
    current_lambda.zeros();
    current_alpha.zeros();
    current_beta.zeros();

    trial_lambda.zeros();
    trial_alpha.zeros();
    trial_beta.zeros();

    current_qtifi.zeros();
    trial_qtifi.zeros();

    current_qtitt = initial_qtitt;
    trial_qtitt = initial_qtitt;

    current_ht = inv(HI);
    trial_ht = current_ht;

    current_disp.zeros();

    auto code = 0;
    for(const auto& I : int_pt) code += I->m_material->clear_status();
    return code;
}

int Proto02::reset_status() {
    trial_lambda = current_lambda;
    trial_alpha = current_alpha;
    trial_beta = current_beta;

    trial_qtitt = current_qtitt;
    trial_qtifi = current_qtifi;

    trial_ht = current_ht;

    auto idx = 0;
    for(const auto& t_ptr : node_ptr) {
        auto& t_disp = t_ptr.lock()->get_current_displacement();
        for(auto pos = 0; pos < m_dof; ++pos) current_disp(idx++) = t_disp(pos);
    }

    auto code = 0;
    for(const auto& I : int_pt) code += I->m_material->reset_status();
    return code;
}

vector<vec> Proto02::record(const OutputList& T) {
    vector<vec> data;
    switch(T) {
    case OutputList::E:
        for(const auto& I : int_pt) data.emplace_back(I->A * current_alpha);
        break;
    case OutputList::S:
        for(const auto& I : int_pt) data.emplace_back(I->P * current_beta);
        break;
    default:
        break;
    }
    return data;
}

void Proto02::print() {
    suanpan_info("Prototype mixed quad element %u connects nodes:\n", get_tag());
    node_encoding.t().print();
    suanpan_info("\nMaterial model response:");
    for(auto I = 0; I < int_pt.size(); ++I) {
        suanpan_info("\nIntegration Point %u:\n", I + 1);
        int_pt[I]->m_material->print();
    }
    suanpan_info("\nElement model response:");
    for(auto I = 0; I < int_pt.size(); ++I) {
        suanpan_info("\nIntegration Point %u:\n", I + 1);
        suanpan_info("Strain:\n");
        (int_pt[I]->A * current_alpha).t().print();
        suanpan_info("Stress:\n");
        (int_pt[I]->P * current_beta).t().print();
    }
}
