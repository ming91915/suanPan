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
    , B(2, 3, fill::zeros) {}

mat F21H::quick_inverse(const mat& stiffness) {
    mat flexibility(2, 2, fill::zeros);

    flexibility(0, 0) = 1. / stiffness(0, 0);
    flexibility(1, 1) = 1. / stiffness(1, 1);

    return flexibility;
}

F21H::F21H(const unsigned T, const uvec& N, const unsigned S, const double L, const bool F)
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

    // quick computation of flexibility
    const auto t_flexibility = quick_inverse(section_proto->get_initial_stiffness());

    // perform integration of elastic region
    const IntegrationPlan plan(1, 4, IntegrationType::GAUSS);
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
        initial_local_flexibility += int_pt[I].B.t() * t_flexibility * int_pt[I].B * weight * length;
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

    vec t_disp(6);
    for(auto I = 0; I < 3; ++I) t_disp(I) = disp_i(I), t_disp(I + 3) = disp_j(I);

    vec residual = -trial_local_deformation;

    // transform global deformation to local one (remove rigid body motion)
    trial_local_deformation = trans_mat * t_disp;

    // initial residual be aware of how to compute it
    residual += trial_local_deformation;

    const auto new_length = length;

    const auto end_num = int_pt.size() - 1;

    auto counter = 0;
    while(true) {
        trial_local_resistance += solve(trial_local_flexibility, residual);
        trial_local_flexibility.zeros();
        residual = trial_local_deformation;
        for(auto I = 0; I < int_pt.size(); ++I) {
            // set an anchor
            auto& stiffness_anchor = I == 0 || I == end_num ? int_pt[I].b_section->get_stiffness() : int_pt[I].b_section->get_initial_stiffness();

            // compute unbalanced deformation
            const vec incre_deformation = (int_pt[I].B * trial_local_resistance - int_pt[I].b_section->get_resistance()) / stiffness_anchor.diag();

            // update status
            int_pt[I].b_section->update_trial_status(int_pt[I].b_section->get_deformation() + incre_deformation);

            // collect new flexibility and deformation
            const mat t_factor = int_pt[I].B.t() * int_pt[I].weight * new_length;
            trial_local_flexibility += t_factor * quick_inverse(stiffness_anchor) * int_pt[I].B;
            residual -= t_factor * int_pt[I].b_section->get_deformation();
        }
        // quit if converged
        if(norm(residual) < 1E-10) break;
        // impose a relatively more strict rule
        if(++counter > 5) {
            suanpan_extra_debug("iteration fails to converge at element level.\n");
            return -1;
        }
    }

    stiffness = trans_mat.t() * solve(trial_local_flexibility, trans_mat);
    resistance = trans_mat.t() * trial_local_resistance;

    return 0;
}

int F21H::clear_status() {
    current_local_flexibility = initial_local_flexibility;
    trial_local_flexibility = initial_local_flexibility;
    current_local_deformation.zeros();
    trial_local_deformation.zeros();
    current_local_resistance.zeros();
    trial_local_resistance.zeros();
    auto code = 0;
    for(const auto& I : int_pt) code += I.b_section->clear_status();
    return code;
}

int F21H::commit_status() {
    current_local_flexibility = trial_local_flexibility;
    current_local_deformation = trial_local_deformation;
    current_local_resistance = trial_local_resistance;
    auto code = 0;
    for(const auto& I : int_pt) code += I.b_section->commit_status();
    return code;
}

int F21H::reset_status() {
    trial_local_flexibility = current_local_flexibility;
    trial_local_deformation = current_local_deformation;
    trial_local_resistance = current_local_resistance;
    auto code = 0;
    for(const auto& I : int_pt) code += I.b_section->reset_status();
    return code;
}

void F21H::print() { suanpan_info("A Forced-Based Beam Element.\nhttps://doi.org/10.1016/0045-7949(95)00103-N\n"); }
