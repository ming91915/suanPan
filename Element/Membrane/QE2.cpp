#include "QE2.h"
#include <Toolbox/integrationPlan.h>
#include <Toolbox/shapeFunction.hpp>
#include <Toolbox/tensorToolbox.h>

const unsigned QE2::m_node = 4;
const unsigned QE2::m_dof = 2;
mat QE2::mapping;

QE2::QE2(const unsigned& T, const uvec& N, const unsigned& M, const double& TH)
    : Element(T, ET_QE2, m_node, m_dof, N, uvec{ M })
    , thickness(TH) {}

void QE2::initialize(const shared_ptr<Domain>& D) {
    // ISOPARAMETRIC MAPPING
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

    // MATERIAL MODEL PROTOTYPE
    const auto& material_proto = D->get_material(static_cast<unsigned>(material_tag(0)));

    // INITIAL FLEXIBILITY
    auto& ini_stiffness = material_proto->get_initial_stiffness();

    // INTEGRATION POINTS INITIALIZATION
    const integrationPlan plan(2, 2, 1);
    int_pt.clear();
    for(unsigned I = 0; I < plan.n_rows; ++I) {
        int_pt.push_back(make_unique<IntegrationPoint>());
        int_pt[I]->coor.zeros(2);
        for(unsigned J = 0; J < 2; ++J) int_pt[I]->coor(J) = plan(I, J);
        int_pt[I]->weight = plan(I, 2);
        int_pt[I]->m_material = material_proto->get_copy();
    }

    // ELEMENT COORDINATES
    mat ele_coor(m_node, 2);
    for(auto I = 0; I < m_node; ++I) {
        auto& tmp_coor = node_ptr[I].lock()->get_coordinate();
        for(auto J = 0; J < 2; ++J) ele_coor(I, J) = tmp_coor(J);
    }

    const mat tmp_const = trans(mapping * ele_coor);

    vec disp_mode(4, fill::zeros);

    mat H(7, 7, fill::zeros);
    mat L(7, 8, fill::zeros);
    mat LI(7, 2, fill::zeros);
    for(const auto& I : int_pt) {
        const auto pn = shapeFunctionQuad(I->coor, 1);

        const mat jacob = pn * ele_coor;

        I->jacob_det = det(jacob);

        disp_mode(1) = I->coor(0);
        disp_mode(2) = I->coor(1);
        disp_mode(3) = I->coor(0) * I->coor(1);

        I->P = shapeStress7(tmp_const * disp_mode);

        I->A = solve(ini_stiffness, I->P);

        I->B = zeros(3, m_node * m_dof);
        const mat pn_pxy = solve(jacob, pn);
        for(unsigned K = 0; K < m_node; ++K) {
            I->B(2, m_dof * K + 1) = pn_pxy(0, K);
            I->B(2, m_dof * K) = pn_pxy(1, K);
            I->B(1, m_dof * K + 1) = pn_pxy(1, K);
            I->B(0, m_dof * K) = pn_pxy(0, K);
        }

        I->BI = zeros(3, 2);
        const vec tmp_vec = I->coor / I->jacob_det;
        I->BI(0, 0) = -tmp_const(1, 2) * tmp_vec(0) - tmp_const(1, 1) * tmp_vec(1);
        I->BI(1, 1) = tmp_const(0, 2) * tmp_vec(0) + tmp_const(0, 1) * tmp_vec(1);
        I->BI(2, 0) = I->BI(1, 1);
        I->BI(2, 1) = I->BI(0, 0);

        const mat tmp_mat = I->P.t() * I->jacob_det * I->weight * thickness;
        H += tmp_mat * I->A;
        L += tmp_mat * I->B;
        LI += tmp_mat * I->BI;
    }

    mass.zeros();
    const auto tmp_density = material_proto->get_parameter() * thickness;
    if(tmp_density != 0.) {
        for(const auto& I : int_pt) {
            const auto n_int = shapeFunctionQuad(I->coor, 0);
            const auto tmp_a = tmp_density * I->jacob_det * I->weight;
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

int QE2::commit_status() {
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
    for(const auto& I : int_pt) code += I->m_material->clear_status();
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
    for(const auto& I : int_pt) code += I->m_material->reset_status();
    return code;
}

vector<vec> QE2::record(const OutputList& T) {
    vector<vec> data;
    switch(T) {
    case OutputList::E:
        for(const auto& I : int_pt) data.push_back(I->A * current_alpha);
        break;
    case OutputList::S:
        for(const auto& I : int_pt) data.push_back(I->P * current_beta);
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
        int_pt[I]->m_material->print();
    }
    suanpan_info("Element model response:\n");
    for(auto I = 0; I < int_pt.size(); ++I) {
        suanpan_info("Integration Point %u:\n", I + 1);
        suanpan_info("Strain:\n");
        (int_pt[I]->A * current_alpha).t().print();
        suanpan_info("Stress:\n");
        (int_pt[I]->P * current_beta).t().print();
    }
}
