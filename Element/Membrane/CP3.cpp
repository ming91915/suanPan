#include "CP3.h"

const unsigned CP3::m_node = 3;
const unsigned CP3::m_dof = 2;

CP3::CP3(const unsigned& T, const uvec& NT, const unsigned& MT, const double& TH)
    : Element(T, ET_CP3, m_node, m_dof, NT, { MT }, false)
    , thickness(TH)
{
}

void CP3::initialize(const shared_ptr<Domain>& D)
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
    strain_mat(0, 0) = inv_coor(1, 0);
    strain_mat(1, 1) = inv_coor(2, 0);
    strain_mat(2, 0) = inv_coor(2, 0);
    strain_mat(2, 1) = inv_coor(1, 0);

    strain_mat(0, 2) = inv_coor(1, 1);
    strain_mat(1, 3) = inv_coor(2, 1);
    strain_mat(2, 2) = inv_coor(2, 1);
    strain_mat(2, 3) = inv_coor(1, 1);

    strain_mat(0, 4) = inv_coor(1, 2);
    strain_mat(1, 5) = inv_coor(2, 2);
    strain_mat(2, 4) = inv_coor(2, 2);
    strain_mat(2, 5) = inv_coor(1, 2);

    auto tmp_density = m_material->getParameter();
    if(tmp_density != 0.) {
        tmp_density *= area * thickness;
        vec n = mean(ele_coor) * inv_coor;
        auto tmp_a = n(0) * n(0) * tmp_density;
        auto tmp_b = n(1) * n(1) * tmp_density;
        auto tmp_c = n(2) * n(2) * tmp_density;
        auto tmp_d = n(0) * n(1) * tmp_density;
        auto tmp_e = n(1) * n(2) * tmp_density;
        auto tmp_f = n(2) * n(0) * tmp_density;
        mass(0, 0) = tmp_a;
        mass(1, 1) = tmp_a;
        mass(2, 2) = tmp_b;
        mass(3, 3) = tmp_b;
        mass(4, 4) = tmp_c;
        mass(5, 5) = tmp_c;
        mass(0, 2) = tmp_d;
        mass(1, 3) = tmp_d;
        mass(2, 0) = tmp_d;
        mass(3, 1) = tmp_d;
        mass(0, 4) = tmp_f;
        mass(1, 5) = tmp_f;
        mass(4, 0) = tmp_f;
        mass(5, 1) = tmp_f;
        mass(2, 4) = tmp_e;
        mass(3, 5) = tmp_e;
        mass(4, 2) = tmp_e;
        mass(5, 3) = tmp_e;
    }
}

int CP3::updateStatus()
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

int CP3::commitStatus() { return m_material->commitStatus(); }

int CP3::clearStatus() { return m_material->clearStatus(); }

int CP3::resetStatus() { return m_material->resetStatus(); }
