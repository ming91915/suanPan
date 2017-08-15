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

    auto& material_proto = D->getMaterial(static_cast<unsigned>(material_tag(0)));

    if(!inv(inv_stiffness, material_proto->getInitialStiffness())) {
        suanpan_fatal("initialize() fails to get initial stiffness.\n");
        return;
    }

    integrationPlan plan(2, 2, 1);

    for(unsigned I = 0; I < 4; ++I) {
        int_pt[I] = make_unique<IntegrationPoint>();
        int_pt[I]->coor.zeros(2);
        for(unsigned J = 0; J < 2; ++J) int_pt[I]->coor(J) = plan(I, J);
        int_pt[I]->weight = plan(I, 2);
        int_pt[I]->m_material = material_proto->getCopy();
    }

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

        I->A = inv_stiffness * I->P;

        I->B.zeros(3, m_node * m_dof);
        for(unsigned K = 0; K < m_node; ++K) {
            I->B(2, 2 * K + 1) = I->pn_pxy(0, K);
            I->B(2, 2 * K) = I->pn_pxy(1, K);
            I->B(1, 2 * K + 1) = I->pn_pxy(1, K);
            I->B(0, 2 * K) = I->pn_pxy(0, K);
        }

        if(tmp_density != 0.) {
            auto n_int = shapeFunctionQuad(I->coor, 0);
            for(unsigned K = 0; K < m_node; ++K) {
                n(0, 2 * K) = n_int(0, K);
                n(1, 2 * K + 1) = n_int(0, K);
            }
            mass += n.t() * n * I->jacob_det * I->weight * tmp_density;
        }
    }
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
