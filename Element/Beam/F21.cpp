////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2017 Theodore Chang
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////

#include "F21.h"
#include <Domain/DomainBase.h>
#include <Domain/Node.h>
#include <Recorder/OutputType.h>
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
    , B(2, 3, fill::zeros) {}

mat F21::quick_inverse(const mat& stiffness) {
    mat flexibility(stiffness.n_rows, stiffness.n_cols, fill::zeros);

    for(auto I = 0; I < flexibility.n_rows; ++I)
        if(stiffness(I, I) != 0.) flexibility(I, I) = 1. / stiffness(I, I);

    return flexibility;
}

F21::F21(const unsigned& T, const uvec& N, const unsigned& S, const unsigned& P, const bool& F)
    : SectionElement(T, ET_F21, b_node, b_dof, N, uvec{ S }, F)
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

    const auto& section_proto = D->get_section(unsigned(section_tag(0)));

    if(section_proto->section_type != SectionType::D2) {
        suanpan_warning("initialize() needs a 2D section.\n");
        D->disable_element(get_tag());
        return;
    }

    // quick computation of flexibility
    const auto t_flexibility = quick_inverse(section_proto->get_initial_stiffness());

    const IntegrationPlan plan(1, int_pt_num, IntegrationType::LOBATTO);

    initial_local_flexibility.zeros(3, 3);
    int_pt.clear(), int_pt.reserve(int_pt_num);
    for(unsigned I = 0; I < int_pt_num; ++I) {
        int_pt.emplace_back(plan(I, 0), plan(I, 1) / 2., section_proto->get_copy());
        int_pt[I].B(0, 0) = 1.;
        int_pt[I].B(1, 1) = (plan(I, 0) - 1.) / 2.;
        int_pt[I].B(1, 2) = (plan(I, 0) + 1.) / 2.;
        // factor .5 moved to weight
        initial_local_flexibility += int_pt[I].B.t() * t_flexibility * int_pt[I].B * int_pt[I].weight * length;
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

    vec t_disp(6);
    for(auto I = 0; I < 3; ++I) t_disp(I) = disp_i(I), t_disp(I + 3) = disp_j(I);

    vec residual_deformation = -trial_local_deformation;

    // transform global deformation to local one (remove rigid body motion)
    trial_local_deformation = trans_mat * t_disp;

    // initial residual be aware of how to compute it
    residual_deformation += trial_local_deformation;

    const auto new_length = length;

    auto counter = 0;
    while(true) {
        trial_local_resistance += solve(trial_local_flexibility, residual_deformation);
        residual_deformation.zeros();
        trial_local_flexibility.zeros();
        for(const auto& I : int_pt) {
            const vec target_section_resistance = I.B * trial_local_resistance;
            // compute unbalanced deformation
            const vec incre_deformation = quick_inverse(I.b_section->get_stiffness()) * (target_section_resistance - I.b_section->get_resistance());
            // update status
            I.b_section->update_trial_status(I.b_section->get_deformation() + incre_deformation);
            // collect new flexibility and deformation
            const mat t_flexibility = I.B.t() * quick_inverse(I.b_section->get_stiffness()) * I.weight * new_length;
            trial_local_flexibility += t_flexibility * I.B;
            residual_deformation += t_flexibility * (I.b_section->get_resistance() - target_section_resistance);
        }
        // quit if converged
        if(norm(residual_deformation) < 1E-12) break;
        // impose a relatively more strict rule
        if(++counter > 5) {
            suanpan_extra_debug("iteration fails to converge at element level.\n");
            return -1;
        }
    }

    stiffness = trans_mat.t() * pinv(trial_local_flexibility) * trans_mat;
    resistance = trans_mat.t() * trial_local_resistance;

    return 0;
}

int F21::clear_status() {
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

int F21::commit_status() {
    current_local_flexibility = trial_local_flexibility;
    current_local_deformation = trial_local_deformation;
    current_local_resistance = trial_local_resistance;
    auto code = 0;
    for(const auto& I : int_pt) code += I.b_section->commit_status();
    return code;
}

int F21::reset_status() {
    trial_local_flexibility = current_local_flexibility;
    trial_local_deformation = current_local_deformation;
    trial_local_resistance = current_local_resistance;
    auto code = 0;
    for(const auto& I : int_pt) code += I.b_section->reset_status();
    return code;
}

vector<vec> F21::record(const OutputType& P) {
    vector<vec> output;
    output.reserve(int_pt.size());

    if(P == OutputType::E)
        for(const auto& I : int_pt) output.emplace_back(I.b_section->get_deformation());
    else if(P == OutputType::S)
        for(const auto& I : int_pt) output.emplace_back(I.b_section->get_resistance());
    else if(P == OutputType::PE)
        for(const auto& I : int_pt) output.emplace_back(I.b_section->get_deformation() - I.b_section->get_resistance() / I.b_section->get_initial_stiffness().diag());

    return output;
}

void F21::print() { suanpan_info("A Forced-Based Beam Element.\nhttps://doi.org/10.1016/0045-7949(95)00103-N\n"); }
