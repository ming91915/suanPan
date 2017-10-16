#include "F21.h"
#include <Domain/DomainBase.h>
#include <Domain/Node.h>
#include <Section/Section.h>
#include <Toolbox/IntegrationPlan.h>
#include <Toolbox/shapeFunction.hpp>
#include <Toolbox/tensorToolbox.h>

const unsigned F21::b_node = 2;
const unsigned F21::b_dof = 3;

F21::IntegrationPoint::IntegrationPoint(const double C, const double W, unique_ptr<Section>&& M)
    : coor(C)
    , weight(W)
    , b_section(move(M))
    , B(2, 3, fill::zeros)
    , current_section_deformation(2, fill::zeros)
    , trial_section_deformation(2, fill::zeros)
    , current_section_resistance(2, fill::zeros)
    , trial_section_resistance(2, fill::zeros) {}

void F21::IntegrationPoint::commit_status() {
    current_section_deformation = trial_section_deformation;
    current_section_resistance = trial_section_resistance;
}

void F21::IntegrationPoint::clear_status() {
    current_section_deformation.zeros();
    trial_section_deformation.zeros();
    current_section_resistance.zeros();
    trial_section_resistance.zeros();
}

void F21::IntegrationPoint::reset_status() {
    trial_section_deformation = current_section_deformation;
    trial_section_resistance = current_section_resistance;
}

F21::F21(const unsigned& T, const uvec& N, const unsigned& S, const unsigned& P, const bool& F)
    : Element(T, ET_F21, b_node, b_dof, N, uvec{ S }, F)
    , int_pt_num(P) {}

void F21::initialize(const shared_ptr<DomainBase>& D) {
    auto& coord_i = node_ptr.at(0).lock()->get_coordinate();
    auto& coord_j = node_ptr.at(1).lock()->get_coordinate();

    // chord vector
    const vec pos_diff = coord_j - coord_i;
    length = norm(pos_diff);
    direction_cosine = pos_diff / length;
    trans_mat = transform::beam::global_to_local(direction_cosine, length);
    inclination = transform::atan2(direction_cosine);

    const auto& section_proto = D->get_section(unsigned(material_tag(0)));
    auto t_stiffness = section_proto->get_initial_stiffness();
    t_stiffness(0, 0) = 1. / t_stiffness(0, 0);
    t_stiffness(1, 1) = 1. / t_stiffness(1, 1);

    const IntegrationPlan plan(1, int_pt_num, IntegrationType::LOBATTO);

    int_pt.clear(), int_pt.reserve(int_pt_num);
    initial_local_flexibility.zeros(3, 3);
    for(unsigned I = 0; I < int_pt_num; ++I) {
        int_pt.emplace_back(plan(I, 0), plan(I, 1), section_proto->get_copy());
        int_pt[I].B(0, 0) = 1.;
        int_pt[I].B(1, 1) = (plan(I, 0) - 1.) / 2.;
        int_pt[I].B(1, 2) = (plan(I, 0) + 1.) / 2.;
        initial_local_flexibility += int_pt[I].B.t() * t_stiffness * int_pt[I].B * int_pt[I].weight * length / 2.;
    }

    current_local_flexibility = initial_local_flexibility;
    trial_local_flexibility = initial_local_flexibility;
    current_local_deformation.zeros(3);
    trial_local_deformation.zeros(3);
    current_local_resistance.zeros(3);
    trial_local_resistance.zeros(3);
}

int F21::update_status() {
    auto& disp_i = node_ptr.at(0).lock()->get_trial_displacement();
    auto& disp_j = node_ptr.at(1).lock()->get_trial_displacement();

    const auto new_length = length;

    vec t_disp(6);
    for(auto I = 0; I < 3; ++I) t_disp(I) = disp_i(I), t_disp(I + 3) = disp_j(I);

    const auto t_local_deformation = trial_local_deformation;
    // transform global deformation to local one (remove rigid body motion)
    trial_local_deformation = trans_mat * t_disp;
    vec incre_local_deformation = trial_local_deformation - t_local_deformation;
    incre_local_deformation.t().print();

    auto counter = 0;
    auto converged = false;
    while(true) {
        const vec incre_local_resistance = solve(trial_local_flexibility, incre_local_deformation);
        trial_local_resistance += incre_local_resistance;
        trial_local_flexibility.zeros();
        for(auto&& I : int_pt) {
            I.trial_section_resistance += I.B * incre_local_resistance;
            const vec incre_deformation = (I.trial_section_resistance - I.b_section->get_resistance()) / I.b_section->get_stiffness().diag();
            I.trial_section_deformation += incre_deformation;
            I.b_section->update_trial_status(I.trial_section_deformation);
            const mat tmp_a = I.B.t() * I.weight * new_length / 2.;
            auto tmp_b = I.b_section->get_stiffness();
            tmp_b(0, 0) = 1. / tmp_b(0, 0), tmp_b(1, 1) = 1. / tmp_b(1, 1);
            trial_local_flexibility += tmp_a * tmp_b * I.B;
            incre_local_deformation -= tmp_a * incre_deformation;
        }
        cout << norm(incre_local_deformation) << endl;
        if(norm(incre_local_deformation) < 1E-10) {
            converged = true;
            break;
        }
        if(++counter > 5) break;
    }

    if(!converged) {
        suanpan_info("iteration fails to converge at element level.\n");
        return -1;
    }

    stiffness = trans_mat.t() * solve(trial_local_flexibility, trans_mat);
    resistance = trans_mat.t() * trial_local_resistance;

    return 0;
}

int F21::commit_status() {
    current_local_flexibility = trial_local_flexibility;
    current_local_deformation = trial_local_deformation;
    current_local_resistance = trial_local_resistance;
    auto code = 0;
    for(auto& I : int_pt) {
        I.commit_status();
        code += I.b_section->commit_status();
    }
    return code;
}

int F21::clear_status() {
    current_local_flexibility = initial_local_flexibility;
    trial_local_flexibility = initial_local_flexibility;
    current_local_deformation.zeros();
    trial_local_deformation.zeros();
    current_local_resistance.zeros();
    trial_local_resistance.zeros();
    auto code = 0;
    for(auto& I : int_pt) {
        I.clear_status();
        code += I.b_section->clear_status();
    }
    return code;
}

int F21::reset_status() {
    trial_local_flexibility = current_local_flexibility;
    trial_local_deformation = current_local_deformation;
    trial_local_resistance = current_local_resistance;
    auto code = 0;
    for(auto& I : int_pt) {
        I.reset_status();
        code += I.b_section->reset_status();
    }
    return code;
}

void F21::print() { suanpan_info("A Forced-Based Beam Element.\nhttps://doi.org/10.1016/0045-7949(95)00103-N\n"); }
