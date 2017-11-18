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

#include "DSFM.h"
#include <Domain/DomainBase.h>
#include <Toolbox/tensorToolbox.h>

DSFM::DSFM(const unsigned T, const unsigned ST, const unsigned CT)
    : Material2D(T, MT_DSFM)
    , rebar_tag(ST)
    , concrete_tag(CT) {}

DSFM::DSFM(const DSFM& P)
    : Material2D(P.get_tag(), MT_DSFM, PlaneType::S, P.density)
    , rebar_tag(P.rebar_tag)
    , concrete_tag(P.concrete_tag) {
    if(P.rebar != nullptr) {
        rebar = P.rebar->get_copy();
        rebar->Material::initialize();
        rebar->initialize();
    }
    if(P.concrete_major != nullptr) {
        concrete_major = P.concrete_major->get_copy();
        concrete_major->Material::initialize();
        concrete_major->initialize();
    }
    if(P.concrete_minor != nullptr) {
        concrete_minor = P.concrete_minor->get_copy();
        concrete_minor->Material::initialize();
        concrete_minor->initialize();
    }
    Material::initialize();
    DSFM::initialize();
}

void DSFM::initialize(const shared_ptr<DomainBase>& D) {
    if(D != nullptr) {
        if(!D->find_material(rebar_tag) || !D->find_material(concrete_tag)) {
            D->disable_material(get_tag());
            return;
        }

        auto& rebar_proto = D->get_material(rebar_tag);
        rebar_proto->Material::initialize(D);
        rebar_proto->initialize(D);
        rebar = rebar_proto->get_copy();
        rebar->Material::initialize(D);
        rebar->initialize(D);

        auto& concrete_proto = D->get_material(concrete_tag);
        concrete_proto->Material::initialize(D);
        concrete_proto->initialize(D);
        concrete_major = concrete_proto->get_copy();
        concrete_minor = concrete_proto->get_copy();
    }

    density = concrete_major->get_parameter(ParameterType::DENSITY) + rebar->get_parameter(ParameterType::DENSITY);

    initial_stiffness = rebar->get_initial_stiffness();

    const auto& E11 = concrete_major->get_initial_stiffness().at(0);
    const auto& E22 = concrete_minor->get_initial_stiffness().at(0);

    initial_stiffness(0, 0) += E11;
    initial_stiffness(1, 1) += E22;
    initial_stiffness(2, 2) += E11 * E22 / (E11 + E22);

    current_stiffness = initial_stiffness;
    trial_stiffness = initial_stiffness;
}

unique_ptr<Material> DSFM::get_copy() { return make_unique<DSFM>(*this); }

int DSFM::update_trial_status(const vec& t_strain) {
    trial_strain = t_strain;

    rebar->update_trial_status(trial_strain);

    mat trans_mat;

    auto counter = 0;
    auto flag = false;
    while(true) {
        trans_mat = transform::strain::trans(principal_angle);

        const vec principal_strain = trans_mat * trial_strain;

        concrete_major->update_trial_status(principal_strain(0));
        concrete_minor->update_trial_status(principal_strain(1));

        vec concrete_stress(3);
        concrete_stress(0) = concrete_major->get_stress().at(0);
        concrete_stress(1) = concrete_minor->get_stress().at(0);
        concrete_stress(2) = 0.;

        trial_stress = rebar->get_stress() + trans_mat.t() * concrete_stress;

        const auto trial_principal_angle = transform::stress::angle(trial_stress);

        if(fabs(trial_principal_angle - principal_angle) < 1E-8) {
            flag = true;
            break;
        }

        principal_angle = trial_principal_angle;

        if(++counter > 50) break;
    }

    if(!flag) suanpan_extra_debug("connot converge within 50 iterations.\n");

    const auto& E11 = concrete_major->get_stiffness().at(0);
    const auto& E22 = concrete_minor->get_stiffness().at(0);

    mat concrete_stiffness(3, 3, fill::zeros);
    concrete_stiffness(0, 0) = E11;
    concrete_stiffness(1, 1) = E22;
    concrete_stiffness(2, 2) = E11 * E22 / (E11 + E22);

    trial_stiffness = rebar->get_stiffness() + trans_mat.t() * concrete_stiffness * trans_mat;

    return 0;
}

int DSFM::clear_status() {
    current_strain.zeros();
    trial_strain.zeros();
    current_stress.zeros();
    trial_stress.zeros();
    current_stiffness = initial_stiffness;
    trial_stiffness = initial_stiffness;
    auto code = rebar->clear_status();
    code += concrete_major->clear_status();
    code += concrete_minor->clear_status();
    return code;
}

int DSFM::commit_status() {
    current_strain = trial_strain;
    current_stress = trial_stress;
    current_stiffness = trial_stiffness;
    auto code = rebar->commit_status();
    code += concrete_major->commit_status();
    code += concrete_minor->commit_status();
    return code;
}

int DSFM::reset_status() {
    trial_strain = current_strain;
    trial_stress = current_stress;
    trial_stiffness = current_stiffness;
    auto code = rebar->reset_status();
    code += concrete_major->reset_status();
    code += concrete_minor->reset_status();
    return code;
}
