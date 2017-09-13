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
    ele_coor.zeros(m_node, m_dof);
    for(auto I = 0; I < m_node; ++I) {
        auto& tmp_coor = node_ptr[I].lock()->get_coordinate();
        for(auto J = 0; J < m_dof; ++J) ele_coor(I, J) = tmp_coor(J);
    }

    const mat tmp_const = trans(mapping * ele_coor);

    vec disp_mode(4, fill::zeros);

    mat H(7, 7, fill::zeros);
    mat L(7, 8, fill::zeros);
    mat LI(7, 2, fill::zeros);
    for(const auto& I : int_pt) {
        const auto pn = shapeFunctionQuad(I->coor, 1);
        I->jacob = pn * ele_coor;
        I->jacob_det = det(I->jacob);
        solve(I->pn_pxy, I->jacob, pn);

        disp_mode(1) = I->coor(0);
        disp_mode(2) = I->coor(1);
        disp_mode(3) = I->coor(0) * I->coor(1);

        I->P = shapeStress7(tmp_const * disp_mode);
        const mat tmp_mat = I->P.t() * I->jacob_det * I->weight * thickness;

        solve(I->A, ini_stiffness, I->P);
        H += tmp_mat * I->A;

        I->B = zeros(3, 8);
        for(unsigned K = 0; K < m_node; ++K) {
            I->B(2, m_dof * K + 1) = I->pn_pxy(0, K);
            I->B(2, m_dof * K) = I->pn_pxy(1, K);
            I->B(1, m_dof * K + 1) = I->pn_pxy(1, K);
            I->B(0, m_dof * K) = I->pn_pxy(0, K);
        }
        L += tmp_mat * I->B;

        const vec tmp_vec = I->coor / I->jacob_det;
        I->BI = zeros(3, 2);
        I->BI(0, 0) = -tmp_const(1, 2) * tmp_vec(0) - tmp_const(1, 1) * tmp_vec(1);
        I->BI(1, 1) = tmp_const(0, 2) * tmp_vec(0) + tmp_const(0, 1) * tmp_vec(1);
        I->BI(2, 0) = I->BI(1, 1);
        I->BI(2, 1) = I->BI(0, 0);
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
    trial_ht = H;
    const mat tmp_mat = HILI.t() * trial_ht;
    QT = tmp_mat * HILI;
    TT = tmp_mat * HIL;

    initial_qtitt = solve(QT, TT);
    current_qtitt = initial_qtitt;
    trial_qtitt = initial_qtitt;

    initial_stiffness = HIL.t() * trial_ht * HIL - TT.t() * initial_qtitt;
    stiffness = initial_stiffness;

    current_qtifi.zeros(2);
    trial_qtifi.zeros(2);

    FI.zeros(2);

    trial_lambda.zeros(2);
    trial_alpha.zeros(7);
    trial_beta.zeros(7);

    current_lambda.zeros(2);
    current_alpha.zeros(7);
    current_beta.zeros(7);
}

int QE2::update_status() {
    vec incre_disp(m_node * m_dof);

    auto idx = 0;
    for(const auto& t_ptr : node_ptr) {
        auto& t_disp = t_ptr.lock()->get_incre_displacement();
        for(auto pos = 0; pos < m_dof; ++pos) incre_disp(idx++) = t_disp(pos);
    }

    if(norm(incre_disp) < 1E-10) return 0; // quick return

    const vec incre_lambda = -trial_qtitt * incre_disp - trial_qtifi; // eq. 65
    const vec incre_alpha = HIL * incre_disp + HILI * incre_lambda;   // eq. 57
    const vec incre_beta = HI * trial_ht * incre_alpha;               // eq. 58

    trial_lambda += incre_lambda; // eq. 66
    trial_alpha += incre_alpha;   // eq. 46
    trial_beta += incre_beta;     // eq. 46

    resistance.zeros();
    trial_ht.zeros();
    FI.zeros();
    auto code = 0;
    for(const auto& t_pt : int_pt) {
        code += t_pt->m_material->update_trial_status(t_pt->A * trial_alpha);
        const auto t_factor = t_pt->jacob_det * t_pt->weight * thickness;
        const vec t_vector = t_pt->P * trial_beta * t_factor;
        trial_ht += t_pt->A.t() * t_pt->m_material->get_stiffness() * t_pt->A * t_factor; // eq. 56
        FI += t_pt->BI.t() * t_vector;                                                    // eq. 54
        resistance += t_pt->B.t() * t_vector;                                             // eq. 54
    }

    QT = HILI.t() * trial_ht * HILI;                             // eq. 60
    TT = HILI.t() * trial_ht * HIL;                              // eq. 60
    solve(trial_qtitt, QT, TT);                                  // eq. 65
    solve(trial_qtifi, QT, FI);                                  // eq. 65
    resistance -= TT.t() * trial_qtifi;                          // eq. 64
    stiffness = HIL.t() * trial_ht * HIL - TT.t() * trial_qtitt; // eq. 61

    return code;
}

int QE2::commit_status() {
    current_lambda = trial_lambda;
    current_alpha = trial_alpha;
    current_beta = trial_beta;

    current_qtitt = trial_qtitt;
    current_qtifi = trial_qtifi;

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

    stiffness = initial_stiffness;

    current_qtitt = initial_qtitt;
    trial_qtitt = initial_qtitt;

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

    auto code = 0;
    for(const auto& I : int_pt) code += I->m_material->reset_status();
    return code;
}

vector<vec> QE2::record(const OutputList& T) {
    vector<vec> data;
    switch(T) {
    case OutputList::E:
        for(const auto& I : int_pt) data.push_back(I->m_material->get_strain());
        break;
    case OutputList::S:
        for(const auto& I : int_pt) data.push_back(I->m_material->get_stress());
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
