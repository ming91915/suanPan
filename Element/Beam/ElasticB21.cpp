#include "ElasticB21.h"

const unsigned ElasticB21::b_node = 2;
const unsigned ElasticB21::b_dof = 3;

ElasticB21::ElasticB21(const unsigned& T, const uvec& N, const double& A, const double& I, const unsigned& M, const bool& F)
    : Element(T, ET_ELASTICB21, b_node, b_dof, N, uvec{ M }, F)
    , area(A)
    , moment_inertia(I) {}

void ElasticB21::initialize(const shared_ptr<DomainBase>& D) {
    auto& coord_i = node_ptr.at(0).lock()->get_coordinate();
    auto& coord_j = node_ptr.at(1).lock()->get_coordinate();

    if(coord_i.size() != 2 || coord_j.size() != 2) {
        suanpan_error("initialize() finds incompatible nodes.\n");
        return;
    }

    const vec pos_diff = coord_j - coord_i;

    length = norm(pos_diff);

    direction_cosine = pos_diff / length;
    ini_angle = atan2(direction_cosine(1), direction_cosine(0));

    b_material = D->get_material(unsigned(material_tag(0)))->get_copy();

    const auto& t_modulus = b_material->get_initial_stiffness();
    const auto tmp_a = as_scalar(t_modulus) / length;
    const auto tmp_b = 2. * tmp_a * moment_inertia;
    const auto tmp_c = 2. * tmp_b;

    local_stiff.zeros(3, 3);
    local_stiff(0, 0) = tmp_a * area;
    local_stiff(1, 1) = tmp_c;
    local_stiff(1, 2) = tmp_b;
    local_stiff(2, 1) = tmp_b;
    local_stiff(2, 2) = tmp_c;

    const auto tmp_d = direction_cosine(0) / length;
    const auto tmp_e = direction_cosine(1) / length;

    strain_mat.zeros(3, 6);
    strain_mat(0, 0) = -direction_cosine(0);
    strain_mat(0, 1) = -direction_cosine(1);
    strain_mat(0, 3) = direction_cosine(0);
    strain_mat(0, 4) = direction_cosine(1);
    strain_mat(1, 0) = -tmp_e;
    strain_mat(1, 1) = tmp_d;
    strain_mat(1, 3) = tmp_e;
    strain_mat(1, 4) = -tmp_d;
    strain_mat(1, 2) = 1.;
    strain_mat(2, 0) = -tmp_e;
    strain_mat(2, 1) = tmp_d;
    strain_mat(2, 3) = tmp_e;
    strain_mat(2, 4) = -tmp_d;
    strain_mat(2, 5) = 1.;
}

int ElasticB21::update_status() {
    const auto& node_i = node_ptr.at(0).lock();
    const auto& node_j = node_ptr.at(1).lock();

    auto& disp_i = node_i->get_trial_displacement();
    auto& disp_j = node_j->get_trial_displacement();

    vec disp_diff(2);
    disp_diff(0) = disp_j(0) - disp_i(0);
    disp_diff(1) = disp_j(1) - disp_i(1);

    auto new_length = length;

    vec local_deformation(3);

    if(nlgeom) {
        disp_diff += node_j->get_coordinate() - node_i->get_coordinate();

        new_length = norm(disp_diff);

        direction_cosine = disp_diff / new_length;

        const auto tmp_d = direction_cosine(0) / new_length;
        const auto tmp_e = direction_cosine(1) / new_length;

        strain_mat(0, 0) = -direction_cosine(0);
        strain_mat(0, 1) = -direction_cosine(1);
        strain_mat(0, 3) = direction_cosine(0);
        strain_mat(0, 4) = direction_cosine(1);
        strain_mat(1, 0) = -tmp_e;
        strain_mat(1, 1) = tmp_d;
        strain_mat(1, 3) = tmp_e;
        strain_mat(1, 4) = -tmp_d;
        strain_mat(2, 0) = -tmp_e;
        strain_mat(2, 1) = tmp_d;
        strain_mat(2, 3) = tmp_e;
        strain_mat(2, 4) = -tmp_d;

        local_deformation(0) = new_length - length;

        auto tmp_angle = ini_angle + disp_i(2);
        auto tmp_a = sin(tmp_angle);
        auto tmp_b = cos(tmp_angle);

        local_deformation(1) = atan((direction_cosine(0) * tmp_a - direction_cosine(1) * tmp_b) / (direction_cosine(0) * tmp_b + direction_cosine(1) * tmp_a));

        tmp_angle = ini_angle + disp_j(2);
        tmp_a = sin(tmp_angle);
        tmp_b = cos(tmp_angle);

        local_deformation(2) = atan((direction_cosine(0) * tmp_a - direction_cosine(1) * tmp_b) / (direction_cosine(0) * tmp_b + direction_cosine(1) * tmp_a));
    } else {
        vec t_disp(6);
        for(auto I = 0; I < 3; ++I) {
            t_disp(I) = disp_i(I);
            t_disp(I + 3) = disp_j(I);
        }
        local_deformation = strain_mat * t_disp;
    }

    const vec local_force = local_stiff * local_deformation;

    stiffness = strain_mat.t() * local_stiff * strain_mat;
    resistance = strain_mat.t() * local_force;

    if(nlgeom) {
        vec R(6, fill::zeros);
        vec Z(6, fill::zeros);
        R(0) = -direction_cosine(0);
        R(1) = -direction_cosine(1);
        R(3) = direction_cosine(0);
        R(4) = direction_cosine(1);
        Z(0) = direction_cosine(1);
        Z(1) = -direction_cosine(0);
        Z(3) = -direction_cosine(1);
        Z(4) = direction_cosine(0);

        const mat tmp_a = R * Z.t();

        stiffness += local_force(0) / new_length * Z * Z.t() + (local_force(1) + local_force(2)) / new_length / new_length * (tmp_a + tmp_a.t());
    }

    return 0;
}

int ElasticB21::commit_status() { return b_material->commit_status(); }

int ElasticB21::clear_status() { return b_material->clear_status(); }

int ElasticB21::reset_status() { return b_material->reset_status(); }
