#include "GQ12.h"
#include <Toolbox/integrationPlan.h>
#include <Toolbox/shapeFunctionQuad.h>

const unsigned GQ12::m_node = 4;
const unsigned GQ12::m_dof = 3;

GQ12::GQ12(const unsigned& T, const uvec& N, const unsigned& M, const double& TH)
    : Element(T, ET_GQ12, m_node, m_dof, N, uvec({ M }), false)
{
}

void GQ12::initialize(const shared_ptr<Domain>& D)
{
    auto& material_proto = D->getMaterial(static_cast<unsigned>(material_tag(0)));

    integrationPlan plan(2, 2, 1);

    for(unsigned I = 0; I < plan.n_rows(); ++I) {
        int_pt.push_back(make_unique<IntegrationPoint>());
        int_pt.at(I)->coor.zeros(2);
        for(auto J = 0; J < 2; ++J) int_pt.at(I)->coor(J) = plan(I, J);
        int_pt.at(I)->weight = plan(I, 2);
        int_pt.at(I)->m_material = material_proto->getCopy();
    }

    mat ele_coor(m_node, 2);
    for(unsigned I = 0; I < m_node; ++I) {
        auto& tmp_coor = node_ptr.at(I).lock()->getCoordinate();
        for(unsigned J = 0; J < 2; ++J) ele_coor(I, J) = tmp_coor(J);
    }

    auto LX1 = ele_coor(1, 1) - ele_coor(0, 1);
    auto LX2 = ele_coor(2, 1) - ele_coor(1, 1);
    auto LX3 = ele_coor(3, 1) - ele_coor(2, 1);
    auto LX4 = ele_coor(0, 1) - ele_coor(3, 1);
    auto LY1 = ele_coor(0, 0) - ele_coor(1, 0);
    auto LY2 = ele_coor(1, 0) - ele_coor(2, 0);
    auto LY3 = ele_coor(2, 0) - ele_coor(3, 0);
    auto LY4 = ele_coor(3, 0) - ele_coor(0, 0);

    mat pnt(2, 8);

    for(const auto& I : int_pt) {
        auto pn = shapeFunctionQuad(I->coor, 1);
        mat jacob = pn * ele_coor;
        I->jacob_det = det(jacob);
        mat pn_pxy = solve(jacob, pn);

        auto X = 2. * I->coor(0);
        auto Y = 2. * I->coor(1);

        auto AA = I->coor(0) + 1.;
        auto BB = I->coor(0) - 1.;
        auto CC = I->coor(1) + 1.;
        auto DD = I->coor(1) - 1.;

        pnt(0, 0) = DD * (LX4 * CC - LX1 * X);
        pnt(0, 1) = DD * (LX2 * CC + LX1 * X);
        pnt(0, 2) = CC * (LX3 * X - LX2 * DD);
        pnt(0, 3) = -CC * (LX3 * X + LX4 * DD);
        pnt(0, 4) = DD * (LY4 * CC - LY1 * X);
        pnt(0, 5) = DD * (LY2 * CC + LY1 * X);
        pnt(0, 6) = CC * (LY3 * X - LY2 * DD);
        pnt(0, 7) = -CC * (LY3 * X + LY4 * DD);
        pnt(1, 0) = BB * (LX4 * Y - LX1 * AA);
        pnt(1, 1) = AA * (LX1 * BB + LX2 * Y);
        pnt(1, 2) = AA * (LX3 * BB - LX2 * Y);
        pnt(1, 3) = -BB * (LX3 * AA + LX4 * Y);
        pnt(1, 4) = BB * (LY4 * Y - LY1 * AA);
        pnt(1, 5) = AA * (LY1 * BB + LY2 * Y);
        pnt(1, 6) = AA * (LY3 * BB - LY2 * Y);
        pnt(1, 7) = -BB * (LY3 * AA + LY4 * Y);

        mat pnt_pxy = solve(jacob, pnt / 16.);

        I->strain_mat.zeros(3, m_node * m_dof);
        for(auto J = 0; J < m_node; ++J) {
            I->strain_mat(0, m_dof * J) = pn_pxy(0, J);
            I->strain_mat(2, m_dof * J) = pn_pxy(1, J);
            I->strain_mat(1, m_dof * J + 1) = pn_pxy(1, J);
            I->strain_mat(2, m_dof * J + 1) = pn_pxy(0, J);
            I->strain_mat(0, m_dof * J + 2) = pnt_pxy(0, J);
            I->strain_mat(1, m_dof * J + 2) = pnt_pxy(1, J + 4);
            I->strain_mat(2, m_dof * J + 2) = pnt_pxy(0, J + 4) + pnt_pxy(1, J);
        }
    }
}

int GQ12::updateStatus()
{
    auto code = 0, idx = 0;

    vec trial_disp(m_node * m_dof);
    for(const auto& I : node_ptr)
        for(const auto& J : I.lock()->getTrialDisplacement()) trial_disp(idx++) = J;

    stiffness.zeros();
    resistance.zeros();
    for(const auto& I : int_pt) {
        code += I->m_material->updateTrialStatus(I->strain_mat * trial_disp);
        mat tmp_factor = I->strain_mat.t() * I->jacob_det * I->weight * thickness;
        stiffness += tmp_factor * I->m_material->getStiffness() * I->strain_mat;
        resistance += tmp_factor * I->m_material->getStress();
    }

    return code;
}

int GQ12::commitStatus()
{
    auto code = 0;
    for(const auto& I : int_pt) code += I->m_material->commitStatus();
    return code;
}

int GQ12::clearStatus()
{
    auto code = 0;
    for(const auto& I : int_pt) code += I->m_material->clearStatus();
    return code;
}

int GQ12::resetStatus()
{
    auto code = 0;
    for(const auto& I : int_pt) code += I->m_material->resetStatus();
    return code;
}
