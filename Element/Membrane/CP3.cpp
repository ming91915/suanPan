#include "CP3.h"

const unsigned CP3::m_node = 3;
const unsigned CP3::m_dof = 2;

CP3::CP3(const unsigned& T, const uvec& NT, const unsigned& MT, const double& TH)
    : Element(T, ET_CP3, m_node, m_dof, NT, uvec{ MT }, false)
    , thickness(TH) {}

void CP3::initialize(const shared_ptr<Domain>& D) {
    m_material = D->get_material(static_cast<unsigned>(material_tag(0)))->get_copy();

    mat ele_coor(m_node, m_node, fill::ones);
    for(auto I = 0; I < m_node; ++I) {
        auto& tmp_coor = node_ptr[I].lock()->get_coordinate();
        for(auto J = 0; J < 2; ++J) ele_coor(I, J + 1) = tmp_coor(J);
    }

    area = det(ele_coor) / 2.;

    const mat inv_coor = inv(ele_coor);

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

    mass.zeros();
    auto tmp_density = m_material->get_parameter();
    if(tmp_density != 0.) {
        tmp_density *= area * thickness;
        const vec n = mean(ele_coor) * inv_coor;
        const auto tmp_a = n(0) * n(0) * tmp_density;
        const auto tmp_b = n(1) * n(1) * tmp_density;
        const auto tmp_c = n(2) * n(2) * tmp_density;
        const auto tmp_d = n(0) * n(1) * tmp_density;
        const auto tmp_e = n(1) * n(2) * tmp_density;
        const auto tmp_f = n(2) * n(0) * tmp_density;
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

int CP3::update_status() {
    vec trial_disp(m_node * m_dof);
    auto idx = 0;
    for(const auto& I : node_ptr) {
        auto& tmp_disp = I.lock()->get_trial_displacement();
        for(auto J = 0; J < m_dof; ++J) trial_disp(idx++) = tmp_disp(J);
    }

    m_material->update_trial_status(strain_mat * trial_disp);

    const mat tmp_factor = strain_mat.t() * area * thickness;
    stiffness = tmp_factor * m_material->get_stiffness() * strain_mat;
    resistance = tmp_factor * m_material->get_stress();

    return 0;
}

int CP3::commit_status() { return m_material->commit_status(); }

int CP3::clear_status() { return m_material->clear_status(); }

int CP3::reset_status() { return m_material->reset_status(); }
