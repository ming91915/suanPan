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

    mass.zeros();

    mat n(2, 8, fill::zeros);

    const auto tmp_density = material_proto->get_parameter() * thickness;

    mat H(7, 7, fill::zeros);
    mat L(7, 8, fill::zeros);
    mat LI(7, 2, fill::zeros);
    for(const auto& I : int_pt) {
        const auto pn = shapeFunctionQuad(I->coor, 1);
        I->jacob = pn * ele_coor;
        I->jacob_det = det(I->jacob);
        if(!solve(I->pn_pxy, I->jacob, pn)) suanpan_warning("initialize() finds a badly shaped element.\n");

        disp_mode(1) = I->coor(0);
        disp_mode(2) = I->coor(1);
        disp_mode(3) = I->coor(0) * I->coor(1);

        I->P = shapeStress7(tmp_const * disp_mode);
        const mat tmp_mat = I->P.t() * I->jacob_det * I->weight * thickness;

        if(!solve(I->A, ini_stiffness, I->P)) suanpan_warning("initialize() finds a singular initial stiffness matrix.\n");
        H += tmp_mat * I->A;

        I->B = zeros(3, 8);
        for(unsigned K = 0; K < m_node; ++K) {
            I->B(2, 2 * K + 1) = I->pn_pxy(0, K);
            I->B(2, 2 * K) = I->pn_pxy(1, K);
            I->B(1, 2 * K + 1) = I->pn_pxy(1, K);
            I->B(0, 2 * K) = I->pn_pxy(0, K);
        }
        L += tmp_mat * I->B;

        const vec tmp_vec = I->coor / I->jacob_det;
        I->BI = zeros(3, 2);
        I->BI(0, 0) = -tmp_const(1, 2) * tmp_vec(0) - tmp_const(1, 1) * tmp_vec(1);
        I->BI(1, 1) = tmp_const(0, 2) * tmp_vec(0) + tmp_const(0, 1) * tmp_vec(1);
        I->BI(2, 0) = I->BI(1, 1);
        I->BI(2, 1) = I->BI(0, 0);
        LI += tmp_mat * I->BI;

        if(tmp_density != 0.) {
            const auto n_int = shapeFunctionQuad(I->coor, 0);
            for(unsigned K = 0; K < m_node; ++K) {
                n(0, 2 * K) = n_int(0, K);
                n(1, 2 * K + 1) = n_int(0, K);
            }
            mass += n.t() * n * I->jacob_det * I->weight * tmp_density;
        }
    }

    inv(HI, H);
    solve(HIL, H, L);
    solve(HILI, H, LI);
    HT = H;
    const mat tmp_mat = HILI.t() * HT;
    QT = tmp_mat * HILI;
    TT = tmp_mat * HIL;

    initial_stiffness = HIL.t() * HT * HIL - TT.t() * solve(QT, TT);
    stiffness = initial_stiffness;

    initial_qtitt = solve(QT, TT);
    current_qtitt = initial_qtitt;
    trial_qtitt = initial_qtitt;

    current_qtifi.zeros(2);
    trial_qtifi.zeros(2);

    FI.zeros(2);

    trial_disp.zeros(8);
    trial_lambda.zeros(2);
    trial_alpha.zeros(7);
    trial_beta.zeros(7);

    current_disp.zeros(8);
    current_lambda.zeros(2);
    current_alpha.zeros(7);
    current_beta.zeros(7);
}

int QE2::update_status() {
    current_disp = trial_disp;

    auto idx = 0;
    for(const auto& tmp_ptr : node_ptr) {
        auto& tmp_disp = tmp_ptr.lock()->get_trial_displacement();
        for(auto pos = 0; pos < m_dof; ++pos) trial_disp(idx++) = tmp_disp(pos);
    }

    if(norm(trial_disp) < 1E-20) return 0; // quick return

    const vec incre_disp = trial_disp - current_disp;                 // eq. 46
    const vec incre_lambda = -trial_qtitt * incre_disp - trial_qtifi; // eq. 65
    const vec incre_alpha = HIL * incre_disp + HILI * incre_lambda;   // eq. 57
    const vec incre_beta = HI * HT * incre_alpha;                     // eq. 58

    trial_lambda += incre_lambda; // eq. 66
    trial_alpha += incre_alpha;   // eq. 46
    trial_beta += incre_beta;     // eq. 46

    resistance.zeros();
    HT.zeros();
    FI.zeros();
    auto code = 0;
    for(const auto& tmp_pt : int_pt) {
        code += tmp_pt->m_material->update_trial_status(tmp_pt->A * trial_alpha);
        const auto tmp_factor = tmp_pt->jacob_det * tmp_pt->weight * thickness;
        const vec tmp_vector = tmp_pt->P * trial_beta * tmp_factor;
        HT += tmp_pt->A.t() * tmp_pt->m_material->get_stiffness() * tmp_pt->A * tmp_factor; // eq. 56
        FI += tmp_pt->BI.t() * tmp_vector;                                                  // eq. 54
        resistance += tmp_pt->B.t() * tmp_vector;                                           // eq. 54
    }

    QT = HILI.t() * HT * HILI;                             // eq. 60
    TT = HILI.t() * HT * HIL;                              // eq. 60
    solve(trial_qtitt, QT, TT);                            // eq. 65
    solve(trial_qtifi, QT, FI);                            // eq. 65
    resistance -= TT.t() * trial_qtifi;                    // eq. 64
    stiffness = HIL.t() * HT * HIL - TT.t() * trial_qtitt; // eq. 61

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

    current_qtifi.zeros(2);
    trial_qtifi.zeros(2);

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
