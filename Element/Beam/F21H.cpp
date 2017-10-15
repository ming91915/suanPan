#include "F21H.h"
#include <Domain/DomainBase.h>
#include <Domain/Node.h>
#include <Section/Section.h>
#include <Toolbox/IntegrationPlan.h>
#include <Toolbox/shapeFunction.hpp>
#include <Toolbox/tensorToolbox.h>

const unsigned F21H::b_node = 2;
const unsigned F21H::b_dof = 3;

F21H::IntegrationPoint::IntegrationPoint(const double C, const double W, unique_ptr<Section>&& M)
    : coor(C)
    , weight(W)
    , b_section(move(M))
    , B(2, 3, fill::zeros)
    , current_section_deformation(2, fill::zeros)
    , trial_section_deformation(2, fill::zeros)
    , current_section_resistance(2, fill::zeros)
    , trial_section_resistance(2, fill::zeros) {}

void F21H::IntegrationPoint::commit_status() {
    current_section_deformation = trial_section_deformation;
    current_section_resistance = trial_section_resistance;
}

void F21H::IntegrationPoint::clear_status() {
    current_section_deformation.zeros();
    trial_section_deformation.zeros();
    current_section_resistance.zeros();
    trial_section_resistance.zeros();
}

void F21H::IntegrationPoint::reset_status() {
    trial_section_deformation = current_section_deformation;
    trial_section_resistance = current_section_resistance;
}

F21H::F21H(const unsigned& T, const uvec& N, const unsigned& S, const double& L, const bool& F)
    : Element(T, ET_F21H, b_node, b_dof, N, uvec{ S }, F)
    , hinge_length(L) {}

void F21H::initialize(const shared_ptr<DomainBase>& D) {
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

    // perform integration of elastic region
    const IntegrationPlan plan(1, 3, IntegrationType::GAUSS);
    const auto int_pt_num = plan.n_rows + 4;
    const auto elastic_length = 1. - 8. * hinge_length;
    initial_local_flexibility.zeros(3, 3);
    int_pt.clear(), int_pt.reserve(int_pt_num);
    double coor, weight;
    for(unsigned I = 0; I < int_pt_num; ++I) {
        if(I == 0) {
            coor = -1.;
            weight = hinge_length;
        } else if(I == 1) {
            coor = 16. / 3. * hinge_length - 1.;
            weight = 3. * hinge_length;
        } else if(I == int_pt_num - 2) {
            coor = 1. - 16. / 3. * hinge_length;
            weight = 3. * hinge_length;
        } else if(I == int_pt_num - 1) {
            coor = 1.;
            weight = hinge_length;
        } else {
            coor = plan(I - 2, 0) * elastic_length;
            weight = plan(I - 2, 1) * elastic_length / 2.;
        }
        int_pt.emplace_back(coor, weight, section_proto->get_copy());
        int_pt[I].B(0, 0) = 1.;
        int_pt[I].B(1, 1) = (coor - 1.) / 2.;
        int_pt[I].B(1, 2) = (coor + 1.) / 2.;
        initial_local_flexibility += int_pt[I].B.t() * t_stiffness * int_pt[I].B * weight * length;
    }

    current_local_flexibility = initial_local_flexibility;
    trial_local_flexibility = initial_local_flexibility;
    current_local_deformation.zeros(3);
    trial_local_deformation.zeros(3);
    current_local_resistance.zeros(3);
    trial_local_resistance.zeros(3);
}

int F21H::update_status() {
    auto& disp_i = node_ptr.at(0).lock()->get_trial_displacement();
    auto& disp_j = node_ptr.at(1).lock()->get_trial_displacement();

    const auto new_length = length;

    current_local_deformation = trial_local_deformation;

    // transform global deformation to local one (remove rigid body motion)
    vec t_disp(6);
    for(auto I = 0; I < 3; ++I) t_disp(I) = disp_i(I), t_disp(I + 3) = disp_j(I);
    trial_local_deformation = trans_mat * t_disp;

    vec incre_local_deformation = trial_local_deformation - current_local_deformation;

    auto counter = 0;
    const auto end_num = int_pt.size() - 1;
    auto converged = false;
    while(true) {
        const vec incre_local_resistance = solve(trial_local_flexibility, incre_local_deformation);
        trial_local_resistance += incre_local_resistance;
        trial_local_flexibility.zeros();
        for(auto I = 0; I < int_pt.size(); ++I) {
            int_pt[I].trial_section_resistance += int_pt[I].B * incre_local_resistance;
            const vec incre_deformation = (int_pt[I].b_section->get_resistance() - int_pt[I].trial_section_resistance) / (I == 0 || I == end_num ? int_pt[I].b_section->get_stiffness().diag() : int_pt[I].b_section->get_initial_stiffness().diag());
            int_pt[I].trial_section_deformation -= incre_deformation;
            int_pt[I].b_section->update_trial_status(int_pt[I].trial_section_deformation);
            // factor .5 is moved to weight
            const mat tmp_a = int_pt[I].B.t() * int_pt[I].weight * new_length;
            auto tmp_b = I == 0 || I == end_num ? int_pt[I].b_section->get_stiffness() : int_pt[I].b_section->get_initial_stiffness();
            tmp_b(0, 0) = 1. / tmp_b(0, 0), tmp_b(1, 1) = 1. / tmp_b(1, 1);
            trial_local_flexibility += tmp_a * tmp_b * int_pt[I].B;
            incre_local_deformation += tmp_a * incre_deformation;
        }
        if(norm(incre_local_deformation) < 1E-12) {
            converged = true;
            break;
        }
        if(++counter > 10) break;
    }

    stiffness = trans_mat.t() * solve(converged ? trial_local_flexibility : initial_local_flexibility, trans_mat);
    resistance = trans_mat.t() * trial_local_resistance;

    return 0;
}

int F21H::commit_status() {
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

int F21H::clear_status() {
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

int F21H::reset_status() {
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

void F21H::print() { suanpan_info("A Forced-Based Beam Element.\nhttps://doi.org/10.1016/0045-7949(95)00103-N\n"); }
