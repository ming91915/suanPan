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

#include "QE2.h"
#include <Domain/DomainBase.h>
#include <Domain/Node.h>
#include <Material/Material2D/Material2D.h>
#include <Recorder/OutputType.h>
#include <Toolbox/IntegrationPlan.h>
#include <Toolbox/shapeFunction.hpp>
#include <Toolbox/tensorToolbox.h>
#include <Toolbox/utility.h>

const unsigned QE2::m_node = 4;
const unsigned QE2::m_dof = 2;
mat QE2::mapping;

QE2::IntegrationPoint::IntegrationPoint(const vec& C, const double W, const double J, unique_ptr<Material>&& M)
    : coor(C)
    , weight(W)
    , jacob_det(J)
    , m_material(move(M))
    , B(3, m_node * m_dof, fill::zeros)
    , BI(3, 2, fill::zeros) {}

QE2::QE2(const unsigned& T, const uvec& N, const unsigned& M, const double& TH)
    : Element(T, ET_QE2, m_node, m_dof, N, uvec{ M })
    , thickness(TH) {}

void QE2::initialize(const shared_ptr<DomainBase>& D) {
    // ISOPARAMETRIC MAPPING
    if(mapping.is_empty()) {
        mapping.zeros(4, 4);
        mapping.fill(.25);
        mapping(1, 0) = mapping(1, 3) = mapping(2, 0) = mapping(2, 1) = mapping(3, 1) = mapping(3, 3) = -.25;
    }

    // ELEMENT COORDINATES
    mat ele_coor(m_node, 2);
    for(auto I = 0; I < m_node; ++I) {
        auto& tmp_coor = node_ptr[I].lock()->get_coordinate();
        for(auto J = 0; J < 2; ++J) ele_coor(I, J) = tmp_coor(J);
    }

    t_factor = trans(mapping * ele_coor);

    // MATERIAL MODEL PROTOTYPE
    auto& material_proto = D->get_material(unsigned(material_tag(0)));

    if(material_proto->material_type == MaterialType::D2 && std::dynamic_pointer_cast<Material2D>(material_proto)->plane_type == PlaneType::E) suanpan::hacker(thickness) = 1.;

    // INITIAL FLEXIBILITY
    auto& ini_stiffness = material_proto->get_initial_stiffness();

    // INTEGRATION POINTS INITIALIZATION
    const IntegrationPlan plan(2, 2, IntegrationType::GAUSS);

    vec disp_mode(4, fill::zeros);

    mat H(7, 7, fill::zeros), L(7, 8, fill::zeros), LI(7, 2, fill::zeros);

    int_pt.clear(), int_pt.reserve(plan.n_rows);
    for(unsigned I = 0; I < plan.n_rows; ++I) {
        const vec t_vec{ plan(I, 0), plan(I, 1) };
        const auto pn = shape::quad(t_vec, 1);
        const mat jacob = pn * ele_coor;
        int_pt.emplace_back(t_vec, plan(I, 2), det(jacob), material_proto->get_copy());

        disp_mode(1) = int_pt[I].coor(0);
        disp_mode(2) = int_pt[I].coor(1);
        disp_mode(3) = int_pt[I].coor(0) * int_pt[I].coor(1);

        int_pt[I].P = shape::stress7(t_factor * disp_mode);

        int_pt[I].A = solve(ini_stiffness, int_pt[I].P);

        const mat pn_pxy = solve(jacob, pn);
        for(unsigned K = 0; K < m_node; ++K) {
            int_pt[I].B(0, m_dof * K) = int_pt[I].B(2, m_dof * K + 1) = pn_pxy(0, K);
            int_pt[I].B(2, m_dof * K) = int_pt[I].B(1, m_dof * K + 1) = pn_pxy(1, K);
        }

        const vec tmp_vec = int_pt[I].coor / int_pt[I].jacob_det;
        int_pt[I].BI(2, 1) = int_pt[I].BI(0, 0) = -t_factor(1, 2) * tmp_vec(0) - t_factor(1, 1) * tmp_vec(1);
        int_pt[I].BI(2, 0) = int_pt[I].BI(1, 1) = t_factor(0, 2) * tmp_vec(0) + t_factor(0, 1) * tmp_vec(1);

        const mat tmp_mat = int_pt[I].P.t() * int_pt[I].jacob_det * int_pt[I].weight * thickness;
        H += tmp_mat * int_pt[I].A;
        L += tmp_mat * int_pt[I].B;
        LI += tmp_mat * int_pt[I].BI;
    }

    mass.zeros();
    const auto tmp_density = material_proto->get_parameter() * thickness;
    if(tmp_density != 0.) {
        for(const auto& I : int_pt) {
            const auto n_int = shape::quad(I.coor, 0);
            const auto tmp_a = tmp_density * I.jacob_det * I.weight;
            for(auto J = 0; J < m_node; ++J)
                for(auto K = J; K < m_node; ++K) mass(m_dof * J, m_dof * K) += tmp_a * n_int(J) * n_int(K);
        }
        for(auto I = 0; I < m_node * m_dof; I += m_dof) {
            mass(I + 1, I + 1) = mass(I, I);
            for(auto J = I + m_dof; J < m_node * m_dof; J += m_dof) mass(J, I) = mass(I + 1, J + 1) = mass(J + 1, I + 1) = mass(I, J);
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

    current_disp.zeros(8);
    current_lambda.zeros(2);
    current_alpha.zeros(7);
    current_beta.zeros(7);

    trial_disp.zeros(8);
    trial_lambda.zeros(2);
    trial_alpha.zeros(7);
    trial_beta.zeros(7);
}

int QE2::update_status() {
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
        code += t_pt.m_material->update_trial_status(t_pt.A * trial_alpha);
        const auto t_factor = t_pt.jacob_det * t_pt.weight * thickness;
        const vec t_vector = t_pt.P * trial_beta * t_factor;
        trial_ht += t_pt.A.t() * t_pt.m_material->get_stiffness() * t_pt.A * t_factor; // eq. 56
        resistance += t_pt.B.t() * t_vector;                                           // eq. 54
        FI += t_pt.BI.t() * t_vector;                                                  // eq. 54
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

int QE2::commit_status() {
    current_lambda = trial_lambda;
    current_alpha = trial_alpha;
    current_beta = trial_beta;

    current_qtitt = trial_qtitt;
    current_qtifi = trial_qtifi;

    current_ht = trial_ht;

    auto code = 0;
    for(const auto& I : int_pt) code += I.m_material->commit_status();
    return code;
}

int QE2::clear_status() {
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
    for(const auto& I : int_pt) code += I.m_material->clear_status();
    return code;
}

int QE2::reset_status() {
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
    for(const auto& I : int_pt) code += I.m_material->reset_status();
    return code;
}

vector<vec> QE2::record(const OutputType& T) {
    vector<vec> data;
    switch(T) {
    case OutputType::E:
        for(const auto& I : int_pt) data.push_back(I.A * current_alpha);
        break;
    case OutputType::S:
        for(const auto& I : int_pt) data.push_back(I.P * current_beta);
        break;
    default:
        break;
    }
    return data;
}

void QE2::print() {
    suanpan_info("Piltner's mixed quad element %u connects nodes:\n", get_tag());
    node_encoding.t().print();
    suanpan_info("Material model response:\n");
    for(auto I = 0; I < int_pt.size(); ++I) {
        suanpan_info("Integration Point %u:\n", I + 1);
        int_pt[I].m_material->print();
    }
    suanpan_info("Element model response:\n");
    for(auto I = 0; I < int_pt.size(); ++I) {
        suanpan_info("Integration Point %u:\n", I + 1);
        suanpan_info("Strain:\n");
        (int_pt[I].A * current_alpha).t().print();
        suanpan_info("Stress:\n");
        (int_pt[I].P * current_beta).t().print();
    }
}
