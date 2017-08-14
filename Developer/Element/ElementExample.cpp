#include "ElementExample.h"

DLL_EXPORT void new_elementexample_(unique_ptr<Element>& return_obj,
    std::istringstream& command)
{
    unsigned tag;
    if(command.good())
        command >> tag;
    else
        suanpan_error("ElementExample needs a tag.\n");

    unsigned node;
    vector<uword> node_tag;
    for(auto I = 0; I < 3; ++I) {
        if(command.good())
            command >> node;
        else
            suanpan_error("ElementExample needs 3 ndoes.\n");
        node_tag.push_back(node);
    }

    unsigned material_tag;
    if(command.good())
        command >> material_tag;
    else
        suanpan_error("ElementExample needs a material tag.\n");

    auto thickness = 1.;
    if(command.good()) command >> thickness;

    return_obj =
        make_unique<ElementExample>(tag, uvec(node_tag), material_tag, thickness);
}

const unsigned ElementExample::m_node = 3;
const unsigned ElementExample::m_dof = 2;

#ifndef ET_ELEMENTTEMPLATE
#define ET_ELEMENTTEMPLATE 0
#endif

ElementExample::ElementExample(const unsigned& T,
    const uvec& NT,
    const unsigned& MT,
    const double& TH)
    : Element(T, ET_ELEMENTTEMPLATE, m_node, m_dof, NT, { MT }, false)
    , thickness(TH)
{
}

void ElementExample::initialize(const shared_ptr<Domain>& D)
{
    auto& material_proto = D->getMaterial(static_cast<unsigned>(material_tag(0)));
    m_material = material_proto->getCopy();

    mat ele_coor(m_node, m_node, fill::ones);
    for(auto I = 0; I < m_node; ++I) {
        auto& tmp_coor = node_ptr.at(I).lock()->getCoordinate();
        for(auto J = 0; J < 2; ++J) ele_coor(I, J + 1) = tmp_coor(J);
    }

    area = det(ele_coor) / 2.;

    mat inv_coor = inv(ele_coor);

    strain_mat.zeros(3, m_node * m_dof);
    for(auto I = 0; I < 3; ++I) {
        strain_mat(0, 2 * I) = inv_coor(1, I);
        strain_mat(1, 2 * I + 1) = inv_coor(2, I);
        strain_mat(2, 2 * I) = inv_coor(2, I);
        strain_mat(2, 2 * I + 1) = inv_coor(1, I);
    }

    auto tmp_density = m_material->getParameter();
    if(tmp_density != 0.) {
        vec n = mean(ele_coor) * inv_coor;
        mass = n * n.t() * tmp_density * area * thickness;
    }
}

int ElementExample::updateStatus()
{
    vec trial_disp(m_node * m_dof);
    auto idx = 0;
    for(auto I = 0; I < m_node; ++I) {
        auto& tmp_disp = node_ptr.at(I).lock()->getTrialDisplacement();
        for(const auto& J : tmp_disp) trial_disp(idx++) = J;
    }
    m_material->updateTrialStatus(strain_mat * trial_disp);

    stiffness =
        strain_mat.t() * m_material->getStiffness() * strain_mat * area * thickness;
    resistance = strain_mat.t() * m_material->getStress() * area * thickness;

    return 0;
}

int ElementExample::commitStatus() { return m_material->commitStatus(); }

int ElementExample::clearStatus() { return m_material->clearStatus(); }

int ElementExample::resetStatus() { return m_material->resetStatus(); }

void ElementExample::print() { printf("This is an element example.\n"); }
