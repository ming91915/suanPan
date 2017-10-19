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

#include "B21H.h"
#include <Domain/DomainBase.h>
#include <Domain/Node.h>
#include <Recorder/OutputType.h>
#include <Section/Section.h>
#include <Toolbox/IntegrationPlan.h>
#include <Toolbox/shapeFunction.hpp>
#include <Toolbox/tensorToolbox.h>

const unsigned B21H::b_node = 2;
const unsigned B21H::b_dof = 3;

B21H::IntegrationPoint::IntegrationPoint(const double C, const double W, unique_ptr<Section>&& M)
    : coor(C)
    , weight(W)
    , b_section(move(M))
    , strain_mat(2, 3, fill::zeros) {}

B21H::B21H(const unsigned T, const uvec& N, const unsigned S, const double L, const bool F)
    : Element(T, ET_B21H, b_node, b_dof, N, uvec{ S }, F)
    , hinge_length(L > .5 ? .5 : L) {}

void B21H::initialize(const shared_ptr<DomainBase>& D) {
    auto& coord_i = node_ptr.at(0).lock()->get_coordinate();
    auto& coord_j = node_ptr.at(1).lock()->get_coordinate();

    // chord vector
    const vec pos_diff = coord_j - coord_i;
    length = norm(pos_diff);
    direction_cosine = pos_diff / length;
    trans_mat = transform::beam::global_to_local(direction_cosine, length);
    inclination = transform::atan2(direction_cosine);

    auto& section_proto = D->get_section(unsigned(material_tag(0)));

    elastic_section_stiffness = section_proto->get_initial_stiffness();

    const auto elastic_length = 1. - 2. * hinge_length;

    // build up the elastic interior
    const IntegrationPlan elastic_plan(1, 2, IntegrationType::GAUSS);
    elastic_int_pt.clear(), elastic_int_pt.reserve(elastic_plan.n_rows);
    for(unsigned I = 0; I < elastic_plan.n_rows; ++I) elastic_int_pt.emplace_back(elastic_plan(I, 0) * elastic_length, elastic_plan(I, 1) * elastic_length / 2., section_proto->get_copy());

    int_pt.clear(), int_pt.reserve(4);
    int_pt.emplace_back(-1., .25 * hinge_length, section_proto->get_copy());
    int_pt.emplace_back(4. / 3. * hinge_length - 1., .75 * hinge_length, section_proto->get_copy());
    int_pt.emplace_back(1. - 4. / 3. * hinge_length, .75 * hinge_length, section_proto->get_copy());
    int_pt.emplace_back(1., .25 * hinge_length, section_proto->get_copy());

    // elastic part will be reused in computation
    elastic_local_stiffness.zeros(3, 3);
    for(auto& I : elastic_int_pt) {
        I.strain_mat(0, 0) = 1. / length;
        I.strain_mat(1, 1) = (3. * I.coor - 1.) / length;
        I.strain_mat(1, 2) = (3. * I.coor + 1.) / length;
        elastic_local_stiffness += I.strain_mat.t() * elastic_section_stiffness * I.strain_mat * I.weight * length;
    }

    for(auto& I : int_pt) {
        I.strain_mat(0, 0) = 1. / length;
        I.strain_mat(1, 1) = (3. * I.coor - 1.) / length;
        I.strain_mat(1, 2) = (3. * I.coor + 1.) / length;
    }
}

int B21H::update_status() {
    const auto& node_i = node_ptr.at(0).lock();
    const auto& node_j = node_ptr.at(1).lock();

    auto& disp_i = node_i->get_trial_displacement();
    auto& disp_j = node_j->get_trial_displacement();

    const auto new_length = length;

    // transform global deformation to local one (remove rigid body motion)
    vec t_disp(6);
    for(auto I = 0; I < 3; ++I) t_disp(I) = disp_i(I), t_disp(I + 3) = disp_j(I);

    const vec local_deformation = trans_mat * t_disp;

    auto local_stiffness = elastic_local_stiffness;
    vec local_resistance = elastic_local_stiffness * local_deformation;
    for(const auto& I : int_pt) {
        I.b_section->update_trial_status(I.strain_mat * local_deformation);
        const mat tmp_a = I.strain_mat.t() * I.weight * new_length;
        local_stiffness += tmp_a * I.b_section->get_stiffness() * I.strain_mat;
        local_resistance += tmp_a * I.b_section->get_resistance();
    }

    for(const auto& I : elastic_int_pt) I.b_section->update_trial_status(I.strain_mat * local_deformation);

    stiffness = trans_mat.t() * local_stiffness * trans_mat;
    resistance = trans_mat.t() * local_resistance;

    return 0;
}

int B21H::commit_status() {
    auto code = 0;
    for(const auto& I : int_pt) code += I.b_section->commit_status();
    return code;
}

int B21H::clear_status() {
    auto code = 0;
    for(const auto& I : int_pt) code += I.b_section->clear_status();
    return code;
}

int B21H::reset_status() {
    auto code = 0;
    for(const auto& I : int_pt) code += I.b_section->reset_status();
    return code;
}

vector<vec> B21H::record(const OutputType& P) {
    vector<vec> output;
    output.reserve(int_pt.size() + elastic_int_pt.size());

    if(P == OutputType::E) {
        output.emplace_back(int_pt[0].b_section->get_deformation());
        output.emplace_back(int_pt[1].b_section->get_deformation());
        for(const auto& I : elastic_int_pt) output.emplace_back(I.b_section->get_deformation());
        output.emplace_back(int_pt[2].b_section->get_deformation());
        output.emplace_back(int_pt[3].b_section->get_deformation());
    } else if(P == OutputType::S) {
        output.emplace_back(int_pt[0].b_section->get_resistance());
        output.emplace_back(int_pt[1].b_section->get_resistance());
        for(const auto& I : int_pt) output.emplace_back(I.b_section->get_resistance());
        output.emplace_back(int_pt[2].b_section->get_resistance());
        output.emplace_back(int_pt[3].b_section->get_resistance());
    } else if(P == OutputType::PE) {
        output.emplace_back(int_pt[0].b_section->get_deformation() - int_pt[0].b_section->get_resistance() / int_pt[0].b_section->get_initial_stiffness().diag());
        output.emplace_back(int_pt[1].b_section->get_deformation() - int_pt[1].b_section->get_resistance() / int_pt[1].b_section->get_initial_stiffness().diag());
        for(const auto& I : int_pt) output.emplace_back(I.b_section->get_deformation() - I.b_section->get_resistance() / I.b_section->get_initial_stiffness().diag());
        output.emplace_back(int_pt[2].b_section->get_deformation() - int_pt[2].b_section->get_resistance() / int_pt[2].b_section->get_initial_stiffness().diag());
        output.emplace_back(int_pt[3].b_section->get_deformation() - int_pt[3].b_section->get_resistance() / int_pt[3].b_section->get_initial_stiffness().diag());
    }

    return output;
}

void B21H::print() {}
