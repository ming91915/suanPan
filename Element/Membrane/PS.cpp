#include "PS.h"
#include <Toolbox/integrationPlan.h>
#include <Toolbox/shapeFunctionQuad.h>

const unsigned PS::m_node = 4;
const unsigned PS::m_dof = 2;

PS::PS(const unsigned& T,
    const uvec& N,
    const unsigned& M,
    const double& TH,
    const unsigned& TY,
    const bool& F)
    : Element(T, ET_PS, m_node, m_dof, N, uvec({ M }), F)
    , thickness(TH)
    , element_type(TY)
    , tmp_a(5, 5)
    , tmp_c(5, 8)
{
}

void PS::initialize(const shared_ptr<Domain>& D)
{
    auto& material_proto = D->getMaterial(static_cast<unsigned>(material_tag(0)));

    inv_stiffness = inv(material_proto->getInitialStiffness());

    integrationPlan plan(2, 2, 1);

    if(element_type == PLANE_STRAIN) thickness = 1.;

    for(unsigned I = 0; I < 4; ++I) {
        int_pt.at(I) = make_unique<IntegrationPoint>();
        int_pt.at(I)->coor.zeros(2);
        for(unsigned J = 0; J < 2; ++J) int_pt.at(I)->coor(J) = plan(I, J);
        int_pt.at(I)->weight = plan(I, 2);
        int_pt.at(I)->m_material = material_proto->getCopy();
    }

    ele_coor.zeros(m_node, m_dof);
    for(unsigned I = 0; I < m_node; ++I) {
        auto& tmp_coor = node_ptr.at(I).lock()->getCoordinate();
        for(unsigned J = 0; J < 2; ++J) ele_coor(I, J) = tmp_coor(J);
    }

    mass.zeros();
    tmp_a.zeros();
    tmp_c.zeros();

    mat n(2, m_node * m_dof, fill::zeros);
    mat jacob_center = shapeFunctionQuad({ 0, 0 }, 1) * ele_coor;

    for(const auto& I : int_pt) {
        auto pn = shapeFunctionQuad(I->coor, 1);
        mat jacob = pn * ele_coor;
        mat pn_pxy = solve(jacob, pn);
        auto tmp_factor = thickness * det(jacob) * I->weight;

        auto n_int = shapeFunctionQuad(I->coor, 0);
        for(unsigned K = 0; K < m_node; ++K) {
            n(0, 2 * K) = n_int(0, K);
            n(1, 2 * K + 1) = n_int(0, K);
        }
        mass += n.t() * n * tmp_factor * I->m_material->getParameter();

        I->strain_mat.zeros(3, m_node * m_dof);
        for(unsigned K = 0; K < m_node; ++K) {
            I->strain_mat(2, 2 * K + 1) = pn_pxy(0, K);
            I->strain_mat(2, 2 * K) = pn_pxy(1, K);
            I->strain_mat(1, 2 * K + 1) = pn_pxy(1, K);
            I->strain_mat(0, 2 * K) = pn_pxy(0, K);
        }

        I->n_stress.zeros(3, 5);
        I->n_stress(0, 0) = 1.;
        I->n_stress(1, 1) = 1.;
        I->n_stress(2, 2) = 1.;
        I->n_stress(0, 3) = jacob_center(0, 0) * jacob_center(0, 0) * I->coor(1);
        I->n_stress(0, 4) = jacob_center(1, 0) * jacob_center(1, 0) * I->coor(0);
        I->n_stress(1, 3) = jacob_center(0, 1) * jacob_center(0, 1) * I->coor(1);
        I->n_stress(1, 4) = jacob_center(1, 1) * jacob_center(1, 1) * I->coor(0);
        I->n_stress(2, 3) = jacob_center(0, 0) * jacob_center(0, 1) * I->coor(1);
        I->n_stress(2, 4) = jacob_center(1, 0) * jacob_center(1, 1) * I->coor(0);

        tmp_c += I->n_stress.t() * I->strain_mat * tmp_factor;
        tmp_a += I->n_stress.t() * inv_stiffness * I->n_stress * tmp_factor;
    }

    stiffness = tmp_c.t() * solve(tmp_a, tmp_c);
}

int PS::updateStatus()
{
    auto idx = 0;

    vec trial_disp(m_node * m_dof);
    for(const auto& I : node_ptr)
        for(const auto& J : I.lock()->getTrialDisplacement()) trial_disp(idx++) = J;

    resistance = stiffness * trial_disp;

    return 0;
}

int PS::commitStatus()
{
    auto code = 0;
    for(const auto& I : int_pt) code += I->m_material->commitStatus();
    return code;
}

int PS::clearStatus()
{
    auto code = 0;
    for(const auto& I : int_pt) code += I->m_material->clearStatus();
    return code;
}

int PS::resetStatus()
{
    auto code = 0;
    for(const auto& I : int_pt) code += I->m_material->resetStatus();
    return code;
}
