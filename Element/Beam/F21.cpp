#include "F21.h"
#include <Toolbox/IntegrationPlan.h>
#include <Toolbox/shapeFunction.hpp>
#include <Toolbox/tensorToolbox.h>

const unsigned F21::b_node = 2;
const unsigned F21::b_dof = 3;

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

    inclination = atan2(direction_cosine(1), direction_cosine(0));

    const auto& section_proto = D->get_section(unsigned(material_tag(0)));

    const IntegrationPlan plan(1, int_pt_num, IntegrationType::LOBATTO);

    int_pt.clear();
    int_pt.reserve(int_pt_num);
    initial_local_flexibility.zeros(3, 3);
    for(unsigned I = 0; I < int_pt_num; ++I) {
        int_pt.emplace_back(plan(I, 0), plan(I, 1), section_proto->get_copy());
        int_pt[I].B(0, 0) = 1.;
        int_pt[I].B(1, 1) = (plan(I, 0) - 1.) / 2.;
        int_pt[I].B(1, 2) = (plan(I, 0) + 1.) / 2.;
        initial_local_flexibility += int_pt[I].B.t() * solve(int_pt[I].b_section->get_initial_stiffness(), int_pt[I].B) * int_pt[I].weight * length / 2.;
    }

    trans_mat = transform::beam::global_to_local(direction_cosine, length);

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

    current_local_deformation = trial_local_deformation;

    // transform global deformation to local one (remove rigid body motion)
    vec t_disp(6);
    for(auto I = 0; I < 3; ++I) {
        t_disp(I) = disp_i(I);
        t_disp(I + 3) = disp_j(I);
    }
    trial_local_deformation = trans_mat * t_disp;

    vec incre_local_deformation = trial_local_deformation - current_local_deformation;
    vec incre_local_resistance = solve(trial_local_flexibility, incre_local_deformation);

    auto counter = 0;

    while(true) {
        cout << norm(incre_local_deformation) << "\t" << counter << endl;
        trial_local_resistance += incre_local_resistance;
        trial_local_flexibility.zeros();
        incre_local_deformation.zeros();
        for(auto&& I : int_pt) {
            const vec incre_resistance = I.B * incre_local_resistance;
            I.trial_section_resistance += incre_resistance;
            const vec residual = I.trial_section_resistance - I.b_section->get_resistance();
            const vec incre_deformation{ residual.at(0) / I.b_section->get_stiffness().at(0, 0), residual.at(1) / I.b_section->get_stiffness().at(1, 1) };
            I.trial_section_deformation += incre_deformation;
            I.b_section->update_status(I.trial_section_deformation);
            const mat tmp_a = I.B.t() * I.weight * new_length / 2.;
            trial_local_flexibility += tmp_a * solve(I.b_section->get_stiffness(), I.B);
            incre_local_deformation += tmp_a * incre_deformation;
        }
        incre_local_resistance = -solve(trial_local_flexibility, incre_local_deformation);
        if(norm(incre_local_deformation) < 1E-8 || counter++ > 10) break;
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
