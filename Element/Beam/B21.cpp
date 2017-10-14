#include "B21.h"
#include <Toolbox/IntegrationPlan.h>
#include <Toolbox/shapeFunction.hpp>
#include <Toolbox/tensorToolbox.h>

const unsigned B21::b_node = 2;
const unsigned B21::b_dof = 3;

B21::IntegrationPoint::IntegrationPoint(const double C, const double W, unique_ptr<Section>&& M)
    : coor(C)
    , weight(W)
    , b_section(move(M))
    , strain_mat(2, 3, fill::zeros) {}

B21::B21(const unsigned& T, const uvec& N, const unsigned& S, const unsigned& P, const bool& F)
    : Element(T, ET_B21, b_node, b_dof, N, uvec{ S }, F)
    , int_pt_num(P) {}

void B21::initialize(const shared_ptr<DomainBase>& D) {
    auto& coord_i = node_ptr.at(0).lock()->get_coordinate();
    auto& coord_j = node_ptr.at(1).lock()->get_coordinate();

    // chord vector
    const vec pos_diff = coord_j - coord_i;

    length = norm(pos_diff);

    direction_cosine = pos_diff / length;

    inclination = atan2(direction_cosine(1), direction_cosine(0));

    const auto& section_proto = D->get_section(unsigned(material_tag(0)));

    const IntegrationPlan plan(1, int_pt_num, IntegrationType::LOBATTO);

    int_pt.clear();
    int_pt.reserve(int_pt_num);
    for(unsigned I = 0; I < int_pt_num; ++I) {
        int_pt.emplace_back(plan(I, 0), plan(I, 1), section_proto->get_copy());
        int_pt[I].strain_mat(0, 0) = 1. / length;
        int_pt[I].strain_mat(1, 1) = (3. * plan(I, 0) - 1.) / length;
        int_pt[I].strain_mat(1, 2) = (3. * plan(I, 0) + 1.) / length;
    }

    trans_mat = transform::beam::global_to_local(direction_cosine, length);
}

int B21::update_status() {
    const auto& node_i = node_ptr.at(0).lock();
    const auto& node_j = node_ptr.at(1).lock();

    auto& disp_i = node_i->get_trial_displacement();
    auto& disp_j = node_j->get_trial_displacement();

    const auto new_length = length;

    // transform global deformation to local one (remove rigid body motion)
    vec t_disp(6);
    for(auto I = 0; I < 3; ++I) {
        t_disp(I) = disp_i(I);
        t_disp(I + 3) = disp_j(I);
    }
    const vec local_deformation = trans_mat * t_disp;

    mat local_stiffness(3, 3, fill::zeros);
    vec local_resistance(3, fill::zeros);
    for(const auto& I : int_pt) {
        I.b_section->update_trial_status(I.strain_mat * local_deformation);
        const mat tmp_a = I.strain_mat.t() * I.weight * new_length / 2.;
        local_stiffness += tmp_a * I.b_section->get_stiffness() * I.strain_mat;
        local_resistance += tmp_a * I.b_section->get_resistance();
    }

    stiffness = trans_mat.t() * local_stiffness * trans_mat;
    resistance = trans_mat.t() * local_resistance;

    return 0;
}

int B21::commit_status() {
    auto code = 0;
    for(const auto& I : int_pt) code += I.b_section->commit_status();
    return code;
}

int B21::clear_status() {
    auto code = 0;
    for(const auto& I : int_pt) code += I.b_section->clear_status();
    return code;
}

int B21::reset_status() {
    auto code = 0;
    for(const auto& I : int_pt) code += I.b_section->reset_status();
    return code;
}

void B21::print() {}
