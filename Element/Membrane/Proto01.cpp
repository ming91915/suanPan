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

#include "Proto01.h"
#include <Domain/DomainBase.h>
#include <Domain/Node.h>
#include <Material/Material2D/Material2D.h>
#include <Recorder/OutputType.h>
#include <Toolbox/IntegrationPlan.h>
#include <Toolbox/shapeFunction.hpp>
#include <Toolbox/tensorToolbox.h>
#include <Toolbox/utility.h>

const unsigned Proto01::m_node = 4;
const unsigned Proto01::m_dof = 3;
const unsigned Proto01::m_size = m_dof * m_node;
mat Proto01::mapping;
mat Proto01::converter;

Proto01::IntegrationPoint::IntegrationPoint(const vec& C, const double W, const double J, unique_ptr<Material>&& M)
    : coor(C)
    , weight(W)
    , jacob_det(J)
    , m_material(move(M))
    , B(3, m_size, fill::zeros)
    , BI(3, 3, fill::zeros) {}

Proto01::Proto01(const unsigned T, const uvec& N, const unsigned M, const double TH, const bool RS)
    : MaterialElement(T, ET_PROTO01, m_node, m_dof, N, uvec{ M })
    , thickness(TH)
    , reduced_scheme(RS) {}

void Proto01::initialize(const shared_ptr<DomainBase>& D) {
    if(mapping.is_empty()) {
        mapping.zeros(4, 4);
        mapping.fill(.25);
        mapping(1, 0) = mapping(1, 3) = mapping(2, 0) = mapping(2, 1) = mapping(3, 1) = mapping(3, 3) = -.25;
    }

    if(converter.is_empty()) {
        converter.zeros(12, 11);
        converter(0, 1) = converter(2, 2) = converter(2, 6) = converter(3, 0) = converter(3, 6) = converter(5, 2) = converter(6, 1) = converter(6, 5) = converter(8, 2) = converter(9, 0) = converter(11, 2) = converter(11, 5) = 2.;
        converter(0, 5) = converter(0, 10) = converter(2, 8) = converter(3, 9) = converter(5, 5) = converter(5, 7) = converter(6, 10) = converter(8, 6) = converter(8, 8) = converter(9, 6) = converter(9, 9) = converter(11, 7) = -2.;
        converter(0, 9) = converter(2, 7) = converter(3, 10) = converter(5, 8) = converter(6, 9) = converter(8, 7) = converter(9, 10) = converter(11, 8) = -2. / 3.;
        converter(1, 3) = converter(4, 4) = converter(7, 3) = converter(10, 4) = 2. / 3.;
        converter(1, 7) = converter(10, 8) = -(converter(4, 8) = converter(7, 7) = 4. / 3.);
    }

    mat ele_coor(m_node, 2);
    for(auto I = 0; I < m_node; ++I) {
        auto& t_coor = node_ptr[I].lock()->get_coordinate();
        for(auto J = 0; J < 2; ++J) ele_coor(I, J) = t_coor(J);
    }

    trans_mat = trans(mapping * ele_coor);

    const auto LX1 = ele_coor(1, 1) - ele_coor(0, 1);
    const auto LX2 = ele_coor(2, 1) - ele_coor(1, 1);
    const auto LX3 = ele_coor(3, 1) - ele_coor(2, 1);
    const auto LX4 = ele_coor(0, 1) - ele_coor(3, 1);
    const auto LY1 = ele_coor(0, 0) - ele_coor(1, 0);
    const auto LY2 = ele_coor(1, 0) - ele_coor(2, 0);
    const auto LY3 = ele_coor(2, 0) - ele_coor(3, 0);
    const auto LY4 = ele_coor(3, 0) - ele_coor(0, 0);

    auto& material_proto = D->get_material(unsigned(material_tag(0)));

    if(material_proto->material_type == MaterialType::D2 && std::dynamic_pointer_cast<Material2D>(material_proto)->plane_type == PlaneType::E) suanpan::hacker(thickness) = 1.;

    auto& ini_stiffness = material_proto->get_initial_stiffness();

    const IntegrationPlan plan(2, reduced_scheme ? 2 : 3, reduced_scheme ? IntegrationType::IRONS : IntegrationType::GAUSS);

    mat pnt(2, 8), t_array(2, 2, fill::zeros);

    vec disp_mode(4, fill::zeros);

    mat H(11, 11, fill::zeros);

    L.zeros(11, 12), LI.zeros(11, 3);

    int_pt.clear(), int_pt.reserve(plan.n_rows);
    for(unsigned I = 0; I < plan.n_rows; ++I) {
        vec t_vec{ plan(I, 0), plan(I, 1) };
        const auto pn = shape::quad(t_vec, 1);
        const mat jacob = pn * ele_coor;
        int_pt.emplace_back(t_vec, plan(I, 2), det(jacob), material_proto->get_copy());
        int_pt[I].factor = int_pt[I].jacob_det * int_pt[I].weight * thickness;

        const auto& X = int_pt[I].coor(0);
        const auto& Y = int_pt[I].coor(1);

        disp_mode(1) = X;
        disp_mode(2) = Y;
        disp_mode(3) = X * Y;

        int_pt[I].P = shape::stress11(trans_mat * disp_mode);

        int_pt[I].A = solve(ini_stiffness, int_pt[I].P);

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
            int_pt[I].B(0, m_dof * J) = int_pt[I].B(2, m_dof * J + 1) = pn_pxy(0, J);
            int_pt[I].B(2, m_dof * J) = int_pt[I].B(1, m_dof * J + 1) = pn_pxy(1, J);
            int_pt[I].B(0, m_dof * J + 2) = pnt_pxy(0, J);
            int_pt[I].B(1, m_dof * J + 2) = pnt_pxy(1, J + 4);
            int_pt[I].B(2, m_dof * J + 2) = pnt_pxy(0, J + 4) + pnt_pxy(1, J);
        }

        t_array(0, 0) = X * X - Y * Y + X;
        t_array(1, 0) = Y * Y - X * X + Y;
        t_array(0, 1) = 6. * X * Y + 3. * Y * Y - 1. + X;
        t_array(1, 1) = 6. * X * Y + 3. * X * X - 1. + Y;
        t_array = solve(jacob, t_array);
        int_pt[I].BI(2, 1) = int_pt[I].BI(0, 0) = t_array(0, 0);
        int_pt[I].BI(2, 0) = int_pt[I].BI(1, 1) = t_array(1, 0);
        int_pt[I].BI(0, 2) = t_array(0, 1);
        int_pt[I].BI(1, 2) = t_array(1, 1);
        int_pt[I].BI(2, 2) = t_array(0, 1) + t_array(1, 1);

        const mat t_mat = int_pt[I].P.t() * int_pt[I].factor;
        H += t_mat * int_pt[I].A;
        L += t_mat * int_pt[I].B;
        LI += t_mat * int_pt[I].BI;
    }

    initial_mass.zeros(m_size, m_size);
    const auto t_density = material_proto->get_parameter();
    if(t_density != 0.) {
        for(const auto& I : int_pt) {
            const auto n_int = shape::quad(I.coor, 0);
            const auto tmp_a = t_density * I.factor;
            for(auto J = 0; J < m_node; ++J)
                for(auto K = J; K < m_node; ++K) initial_mass(m_dof * J, m_dof * K) += tmp_a * n_int(J) * n_int(K);
        }
        for(auto I = 0; I < m_size; I += m_dof) {
            initial_mass(I + 1, I + 1) = initial_mass(I, I);
            for(auto J = I + m_dof; J < m_size; J += m_dof) initial_mass(J, I) = initial_mass(I + 1, J + 1) = initial_mass(J + 1, I + 1) = initial_mass(I, J);
        }
    }
    trial_mass = current_mass = initial_mass;

    inv(HI, H);
    solve(HIL, H, L);
    solve(HILI, H, LI);

    const mat QT = LI.t() * HILI;
    const mat TT = LI.t() * HIL;

    trial_qtitt = current_qtitt = initial_qtitt = solve(QT, TT);

    initial_stiffness = HIL.t() * H * HIL - TT.t() * initial_qtitt;

    current_qtifi.zeros(3);
    trial_qtifi.zeros(3);

    current_disp.zeros(12);
    current_lambda.zeros(3);
    current_alpha.zeros(11);
    current_beta.zeros(11);

    trial_disp.zeros(12);
    trial_lambda.zeros(3);
    trial_alpha.zeros(11);
    trial_beta.zeros(11);
}

int Proto01::update_status() {
    auto idx = 0;
    for(const auto& t_ptr : node_ptr) {
        auto& t_disp = t_ptr.lock()->get_trial_displacement();
        for(auto pos = 0; pos < m_dof; ++pos) trial_disp(idx++) = t_disp(pos);
    }

    const vec incre_disp = trial_disp - current_disp;
    const vec incre_lambda = -trial_qtitt * incre_disp - trial_qtifi; // eq. 65
    const vec incre_alpha = HIL * incre_disp + HILI * incre_lambda;   // eq. 57

    trial_lambda += incre_lambda; // eq. 66
    trial_alpha += incre_alpha;   // eq. 46

    auto code = 0;
    mat HT(11, 11, fill::zeros);
    for(const auto& t_pt : int_pt) {
        code += t_pt.m_material->update_trial_status(t_pt.A * trial_alpha);
        HT += t_pt.A.t() * t_pt.m_material->get_stiffness() * t_pt.A * t_pt.factor; // eq. 56
    }

    trial_beta += HI * HT * incre_alpha; // eq. 46

    const mat tmp_mat = HILI.t() * HT;
    const mat QT = tmp_mat * HILI;                                // eq. 60
    const mat TT = tmp_mat * HIL;                                 // eq. 60
    solve(trial_qtitt, QT, TT);                                   // eq. 65
    solve(trial_qtifi, QT, LI.t() * trial_beta);                  // eq. 65
    trial_resistance = L.t() * trial_beta - TT.t() * trial_qtifi; // eq. 64
    trial_stiffness = HIL.t() * HT * HIL - TT.t() * trial_qtitt;  // eq. 61

    current_disp = trial_disp;

    return code;
}

int Proto01::commit_status() {
    current_lambda = trial_lambda;
    current_alpha = trial_alpha;
    current_beta = trial_beta;
    current_qtifi = trial_qtifi;

    current_qtitt = trial_qtitt;

    auto code = 0;
    for(const auto& I : int_pt) code += I.m_material->commit_status();
    return code;
}

int Proto01::clear_status() {
    current_lambda.zeros();
    trial_lambda.zeros();
    current_alpha.zeros();
    trial_alpha.zeros();
    current_beta.zeros();
    trial_beta.zeros();
    current_qtifi.zeros();
    trial_qtifi.zeros();

    current_qtitt = trial_qtitt = initial_qtitt;

    current_disp.zeros();

    auto code = 0;
    for(const auto& I : int_pt) code += I.m_material->clear_status();
    return code;
}

int Proto01::reset_status() {
    trial_lambda = current_lambda;
    trial_alpha = current_alpha;
    trial_beta = current_beta;
    trial_qtifi = current_qtifi;

    trial_qtitt = current_qtitt;

    auto idx = 0;
    for(const auto& t_ptr : node_ptr) {
        auto& t_disp = t_ptr.lock()->get_current_displacement();
        for(auto pos = 0; pos < m_dof; ++pos) current_disp(idx++) = t_disp(pos);
    }

    auto code = 0;
    for(const auto& I : int_pt) code += I.m_material->reset_status();
    return code;
}

vector<vec> Proto01::record(const OutputType& T) {
    vector<vec> data;
    switch(T) {
    case OutputType::E:
        for(const auto& I : int_pt) data.emplace_back(I.A * current_alpha);
        break;
    case OutputType::S:
        for(const auto& I : int_pt) data.emplace_back(I.P * current_beta);
        break;
    default:
        break;
    }
    return data;
}

void Proto01::print() {
    suanpan_info("Prototype mixed quad element %u connects nodes:\n", get_tag());
    node_encoding.t().print();
    suanpan_info("\nMaterial model response:");
    for(auto I = 0; I < int_pt.size(); ++I) {
        suanpan_info("\nIntegration Point %u:\n", I + 1);
        int_pt[I].m_material->print();
    }
    suanpan_info("\nElement model response:");
    for(auto I = 0; I < int_pt.size(); ++I) {
        suanpan_info("\nIntegration Point %u:\n", I + 1);
        suanpan_info("Strain:\n");
        (int_pt[I].A * current_alpha).t().print();
        suanpan_info("Stress:\n");
        (int_pt[I].P * current_beta).t().print();
    }
}
