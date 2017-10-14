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

#include "RebarLayer.h"
#include <Toolbox/tensorToolbox.h>

RebarLayer::RebarLayer(const unsigned& T, const unsigned& XT, const unsigned& YT, const double& RX, const double& RY, const double& A, const double& D)
    : Material2D(T, MT_CONCRETE2D, PlaneType::S, D)
    , tag_major(XT)
    , tag_minor(YT)
    , rebar_ratio_major(RX)
    , rebar_ratio_minor(RY)
    , inclination(A)
    , trans(transform::form_trans(inclination)) {}

RebarLayer::RebarLayer(const RebarLayer& P)
    : Material2D(P.get_tag(), MT_CONCRETE2D, PlaneType::S, P.density)
    , tag_major(P.tag_major)
    , tag_minor(P.tag_minor)
    , rebar_ratio_major(P.rebar_ratio_major)
    , rebar_ratio_minor(P.rebar_ratio_minor)
    , inclination(P.inclination)
    , trans(P.trans) {
    if(P.rebar_major != nullptr) rebar_major = P.rebar_major->get_copy();
    if(P.rebar_minor != nullptr) rebar_minor = P.rebar_minor->get_copy();
}

void RebarLayer::initialize(const shared_ptr<DomainBase>& D) {
    if(D->find_material(tag_major)) {
        rebar_major = D->get_material(tag_major)->get_copy();
        if(!rebar_major->initialized) {
            rebar_major->Material::initialize(D);
            rebar_major->initialize(D);
        }
    } else {
        suanpan_error("initialize() cannot find a proper defined material with tag %u.\n", tag_major);
        return;
    }
    if(D->find_material(tag_minor)) {
        rebar_minor = D->get_material(tag_minor)->get_copy();
        if(!rebar_minor->initialized) {
            rebar_minor->Material::initialize(D);
            rebar_minor->initialize(D);
        }
    } else {
        suanpan_error("initialize() cannot find a proper defined material with tag %u.\n", tag_minor);
        return;
    }

    initial_stiffness.zeros(3, 3);

    initial_stiffness(0, 0) = rebar_ratio_major * rebar_major->get_initial_stiffness().at(0);
    initial_stiffness(1, 1) = rebar_ratio_minor * rebar_minor->get_initial_stiffness().at(0);

    initial_stiffness = solve(trans, initial_stiffness * trans);

    trial_stiffness = initial_stiffness;
    current_stiffness = initial_stiffness;
}

unique_ptr<Material> RebarLayer::get_copy() { return make_unique<RebarLayer>(*this); }

int RebarLayer::update_incre_status(const vec& i_strain) { return update_trial_status(current_strain + i_strain); }

int RebarLayer::update_trial_status(const vec& t_strain) {
    trial_strain = t_strain;

    auto p_strain = transform::rotate_strain(trial_strain, trans);

    // update status
    rebar_major->update_trial_status(vec{ p_strain(0) });
    rebar_minor->update_trial_status(vec{ p_strain(1) });

    vec p_stress(3, fill::zeros);

    // collect principal stress components
    p_stress(0) = rebar_ratio_major * rebar_major->get_stress().at(0);
    p_stress(1) = rebar_ratio_minor * rebar_minor->get_stress().at(0);

    // transform back to nominal direction
    solve(trial_stress, trans, p_stress);

    // collect principal stiffness components
    trial_stiffness.zeros(3, 3);
    trial_stiffness(0, 0) = rebar_ratio_major * rebar_major->get_stiffness().at(0);
    trial_stiffness(1, 1) = rebar_ratio_minor * rebar_minor->get_stiffness().at(0);

    // transform back to nominal direction
    solve(trial_stiffness, trans, trial_stiffness * trans);

    return 0;
}

int RebarLayer::clear_status() {
    rebar_major->clear_status();
    rebar_minor->clear_status();
    return 0;
}

int RebarLayer::commit_status() {
    rebar_major->commit_status();
    rebar_minor->commit_status();
    return 0;
}

int RebarLayer::reset_status() {
    rebar_major->reset_status();
    rebar_minor->reset_status();
    return 0;
}
