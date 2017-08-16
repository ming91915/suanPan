#include "QE2.h"
#include <Toolbox/integrationPlan.h>
#include <Toolbox/shapeFunctionQuad.h>
#include <Toolbox/tensorToolbox.h>

const unsigned QE2::m_node = 4;
const unsigned QE2::m_dof = 2;
mat QE2::mapping;

QE2::QE2(const unsigned& T, const uvec& N, const unsigned& M, const double& TH)
    : Element(T, ET_QE2, m_node, m_dof, N, uvec({ M }), false)
    , thickness(TH)
{
}

void QE2::initialize(const shared_ptr<Domain>& D)
{
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
    auto& material_proto = D->getMaterial(static_cast<unsigned>(material_tag(0)));

    // INITIAL FLEXIBILITY
    auto& ini_stiffness = material_proto->getInitialStiffness();

    // INTEGRATION POINTS INITIALIZATION
    integrationPlan plan(2, 2, 1);
    for(unsigned I = 0; I < 4; ++I) {
        int_pt[I] = make_unique<IntegrationPoint>();
        int_pt[I]->coor.zeros(2);
        for(unsigned J = 0; J < 2; ++J) int_pt[I]->coor(J) = plan(I, J);
        int_pt[I]->weight = plan(I, 2);
        int_pt[I]->m_material = material_proto->getCopy();
    }

    // ELEMENT COORDINATES
    ele_coor.zeros(m_node, m_dof);
    for(unsigned I = 0; I < m_node; ++I) {
        auto& tmp_coor = node_ptr[I].lock()->getCoordinate();
        for(unsigned J = 0; J < m_dof; ++J) ele_coor(I, J) = tmp_coor(J);
    }

    mat tmp_const = trans(mapping * ele_coor);

    vec disp_mode(4, fill::ones);

    mass.zeros();

    mat n(2, m_node * m_dof, fill::zeros);

    auto tmp_density = material_proto->getParameter() * thickness;

    mat H(7, 7, fill::zeros);
    mat L(7, m_node * m_dof, fill::zeros);
    mat LI(7, 2, fill::zeros);
    for(const auto& I : int_pt) {
        auto pn = shapeFunctionQuad(I->coor, 1);
        I->jacob = pn * ele_coor;
        I->jacob_det = det(I->jacob);
        if(!solve(I->pn_pxy, I->jacob, pn))
            suanpan_warning("initialize() finds a badly shaped element.\n");

        disp_mode(1) = I->coor(0);
        disp_mode(2) = I->coor(1);
        disp_mode(3) = I->coor(0) * I->coor(1);

        I->P = shapeStress7(disp_mode * tmp_const);
        mat tmp_mat = I->P.t() * I->jacob_det * I->weight * thickness;

        solve(I->A, ini_stiffness, I->P);
        H += tmp_mat * I->A;

        I->B = zeros(3, m_node * m_dof);
        for(unsigned K = 0; K < m_node; ++K) {
            I->B(2, 2 * K + 1) = I->pn_pxy(0, K);
            I->B(2, 2 * K) = I->pn_pxy(1, K);
            I->B(1, 2 * K + 1) = I->pn_pxy(1, K);
            I->B(0, 2 * K) = I->pn_pxy(0, K);
        }
        L += tmp_mat * I->B;

        I->BI = zeros(3, 2);
        I->BI(0, 0) =
            (-tmp_const(1, 2) * I->coor(0) - tmp_const(1, 1) * I->coor(1)) / I->jacob_det;
        I->BI(1, 1) =
            (tmp_const(0, 2) * I->coor(0) + tmp_const(0, 1) * I->coor(1)) / I->jacob_det;
        I->BI(2, 0) = I->BI(1, 1);
        I->BI(2, 1) = I->BI(0, 0);
        LI += tmp_mat * I->BI;

        if(tmp_density != 0.) {
            auto n_int = shapeFunctionQuad(I->coor, 0);
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
    mat tmp_mat = HILI.t() * HT;
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
}

int QE2::updateStatus()
{
    auto idx = 0;
    for(const auto& I : node_ptr)
        for(const auto& J : I.lock()->getCoordinate()) trial_disp(idx++) = J;

    auto incre_disp = trial_disp - current_disp;                 // eq. 46
    auto incre_lambda = -trial_qtitt * incre_disp - trial_qtifi; // eq. 65
    auto incre_alpha = HIL * incre_disp + HILI * incre_lambda;   // eq. 57
    auto incre_beta = HI * HT * incre_alpha;                     // eq. 58

    trial_lambda += incre_lambda; // eq. 66
    trial_alpha += incre_alpha;   // eq. 46
    trial_beta += incre_beta;     // eq. 46

    resistance.zeros();
    HT.zeros();
    FI.zeros();
    auto code = 0;
    for(const auto& I : int_pt) {
        code += I->m_material->updateTrialStatus(I->A * trial_alpha);
        auto tmp_factor = I->jacob_det * I->weight * thickness;
        vec tmp_vector = I->P * trial_beta * tmp_factor;
        HT += I->A.t() * I->m_material->getStiffness() * I->A * tmp_factor; // eq. 56
        FI += I->BI.t() * tmp_vector;                                       // eq. 54
        resistance += I->B.t() * tmp_vector;                                // eq. 54
    }
    QT = HILI.t() * HT * HILI;                             // eq. 60
    TT = HILI.t() * HT * HIL;                              // eq. 60
    trial_qtitt = solve(QT, TT);                           // eq. 65
    trial_qtifi = solve(QT, FI);                           // eq. 65
    resistance -= TT.t() * trial_qtifi;                    // eq. 64
    stiffness = HIL.t() * HT * HIL - TT.t() * trial_qtitt; // eq. 61

    return code;
}

int QE2::commitStatus()
{
    current_disp = trial_disp;
    current_lambda = trial_lambda;
    current_alpha = trial_alpha;
    current_beta = trial_beta;

    current_qtitt = trial_qtitt;
    current_qtifi = trial_qtifi;

    auto code = 0;
    for(const auto& I : int_pt) code += I->m_material->commitStatus();
    return code;
}

int QE2::clearStatus()
{
    current_disp.zeros();
    current_lambda.zeros();
    current_alpha.zeros();
    current_beta.zeros();

    trial_disp.zeros();
    trial_lambda.zeros();
    trial_alpha.zeros();
    trial_beta.zeros();

    current_qtifi.zeros(2);
    trial_qtifi.zeros(2);

    current_qtitt = initial_qtitt;
    trial_qtitt = initial_qtitt;

    auto code = 0;
    for(const auto& I : int_pt) code += I->m_material->clearStatus();
    return code;
}

int QE2::resetStatus()
{
    trial_disp = current_disp;
    trial_lambda = current_lambda;
    trial_alpha = current_alpha;
    trial_beta = current_beta;

    trial_qtitt = current_qtitt;
    trial_qtifi = current_qtifi;

    auto code = 0;
    for(const auto& I : int_pt) code += I->m_material->resetStatus();
    return code;
}
