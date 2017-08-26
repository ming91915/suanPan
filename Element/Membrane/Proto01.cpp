#include "Proto01.h"
#include "Toolbox/integrationPlan.h"
#include "Toolbox/shapeFunctionQuad.h"
#include "Toolbox/tensorToolbox.h"

const unsigned Proto01::m_node = 4;
const unsigned Proto01::m_dof = 2;
mat Proto01::mapping;

Proto01::Proto01(const unsigned& T, const uvec& N, const unsigned& M, const double& TH)
    : Element(T, ET_PROTO01, m_node, m_dof, N, { M })
    , thickness(TH)
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
}

void Proto01::initialize(const shared_ptr<Domain>& D)
{
    // MATERIAL MODEL PROTOTYPE
    auto& material_proto = D->get_material(static_cast<unsigned>(material_tag(0)));
    // INITIAL FLEXIBILITY
    auto& ini_stiffness = material_proto->get_initial_stiffness();

    // INTEGRATION POINTS INITIALIZATION
    const integrationPlan plan(2, 2, 1);
    int_pt.clear();
    for(unsigned I = 0; I < 4; ++I) {
        int_pt.push_back(make_unique<IntegrationPoint>());
        int_pt[I]->coor.zeros(2);
        for(unsigned J = 0; J < 2; ++J) int_pt[I]->coor(J) = plan(I, J);
        int_pt[I]->weight = plan(I, 2);
        int_pt[I]->m_material = material_proto->get_copy();
    }

    // ELEMENT COORDINATES
    ele_coor.zeros(m_node, m_dof);
    for(unsigned I = 0; I < m_node; ++I) {
        auto& tmp_coor = node_ptr[I].lock()->get_coordinate();
        for(unsigned J = 0; J < m_dof; ++J) ele_coor(I, J) = tmp_coor(J);
    }

    const mat tmp_const = trans(mapping * ele_coor);

    vec disp_mode(4, fill::zeros);

    mass.zeros();

    mat n(2, m_node * m_dof, fill::zeros);

    mat H(7, 7, fill::zeros);
    mat E(7, m_node * m_dof, fill::zeros);
    for(const auto& I : int_pt) {
        auto pn = shapeFunctionQuad(I->coor, 1);
        I->jacob = pn * ele_coor;
        I->jacob_det = det(I->jacob);
        if(!solve(I->pn_pxy, I->jacob, pn))
            suanpan_warning("initialize() finds a badly shaped element.\n");

        disp_mode(1) = I->coor(0);
        disp_mode(2) = I->coor(1);
        disp_mode(3) = I->coor(0) * I->coor(1);

        I->P = shapeStress11(tmp_const * disp_mode);
        const mat tmp_mat = I->P.t() * I->jacob_det * I->weight * thickness;

        solve(I->A, ini_stiffness, I->P);
        H += tmp_mat * I->A;

        I->B = zeros(3, m_node * m_dof);
        for(unsigned K = 0; K < m_node; ++K) {
            I->B(2, 2 * K + 1) = I->pn_pxy(0, K);
            I->B(2, 2 * K) = I->pn_pxy(1, K);
            I->B(1, 2 * K + 1) = I->pn_pxy(1, K);
            I->B(0, 2 * K) = I->pn_pxy(0, K);
        }
        E += tmp_mat * I->B;
    }
}

int Proto01::update_status()
{
    throw logic_error("The method or operation is not implemented.");
}

int Proto01::commit_status()
{
    throw logic_error("The method or operation is not implemented.");
}

int Proto01::clear_status()
{
    throw logic_error("The method or operation is not implemented.");
}

int Proto01::reset_status()
{
    throw logic_error("The method or operation is not implemented.");
}
