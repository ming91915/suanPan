#include "QE2.h"
#include "Toolbox/integrationPlan.h"
#include "Toolbox/shapeFunctionQuad.h"

const unsigned QE2::m_node = 4;
const unsigned QE2::m_dof = 2;

QE2::QE2(const unsigned& T, const uvec& N, const unsigned& M, const double& TH)
    : Element(T, ET_QE2, m_node, m_dof, N, uvec({ M }), false)
    , thickness(TH)
{
}

void QE2::initialize(const shared_ptr<Domain>& D)
{
    auto& material_proto = D->getMaterial(static_cast<unsigned>(material_tag(0)));
    inv_stiffness = material_proto->getInitialStiffness();

    integrationPlan plan(2, 2, 1);

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
        for(unsigned J = 0; J < m_dof; ++J) ele_coor(I, J) = tmp_coor(J);
    }

    mass.zeros();

    mat n(2, m_node * m_dof, fill::zeros);

    for(const auto& I : int_pt) {
        auto pn = shapeFunctionQuad(I->coor, 1);
        I->jacob = pn * ele_coor;
        I->jacob_det = det(I->jacob);
        I->pn_pxy = solve(I->jacob, pn);

        auto n_int = shapeFunctionQuad(I->coor, 0);
        for(unsigned K = 0; K < m_node; ++K) {
            n(0, 2 * K) = n_int(0, K);
            n(1, 2 * K + 1) = n_int(0, K);
        }
        mass += n.t() * n * I->jacob_det * I->weight * I->m_material->getParameter();

        I->strain_mat.zeros(3, m_node * m_dof);
        for(unsigned K = 0; K < m_node; ++K) {
            I->strain_mat(2, 2 * K + 1) = I->pn_pxy(0, K);
            I->strain_mat(2, 2 * K) = I->pn_pxy(1, K);
            I->strain_mat(1, 2 * K + 1) = I->pn_pxy(1, K);
            I->strain_mat(0, 2 * K) = I->pn_pxy(0, K);
        }
    }
    mass *= thickness;
}

int QE2::updateStatus() { return 0; }

int QE2::commitStatus()
{
    auto code = 0;
    for(const auto& I : int_pt) code += I->m_material->commitStatus();
    return code;
}

int QE2::clearStatus()
{
    auto code = 0;
    for(const auto& I : int_pt) code += I->m_material->clearStatus();
    return code;
}

int QE2::resetStatus()
{
    auto code = 0;
    for(const auto& I : int_pt) code += I->m_material->resetStatus();
    return code;
}
