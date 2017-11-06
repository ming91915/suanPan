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
#include <Domain/DomainBase.h>
#include <Toolbox/tensorToolbox.h>

RebarLayer::RebarLayer(const unsigned T, const unsigned XT, const unsigned YT, const double RX, const double RY, const double A)
    : Material2D(T, MT_CONCRETE2D, PlaneType::S, 0.)
    , tag_major(XT)
    , tag_minor(YT)
    , ratio_major(RX)
    , ratio_minor(RY)
    , inclination(A)
    , trans_strain(transform::form_strain_trans(inclination))
    , trans_stress(transform::form_stress_trans(-inclination)) {}

RebarLayer::RebarLayer(const RebarLayer& P)
    : Material2D(P.get_tag(), MT_CONCRETE2D, PlaneType::S, P.density)
    , tag_major(P.tag_major)
    , tag_minor(P.tag_minor)
    , ratio_major(P.ratio_major)
    , ratio_minor(P.ratio_minor)
    , inclination(P.inclination)
    , trans_strain(P.trans_strain)
    , trans_stress(P.trans_stress) {
    if(P.rebar_major != nullptr) rebar_major = P.rebar_major->get_copy();
    if(P.rebar_minor != nullptr) rebar_minor = P.rebar_minor->get_copy();
    Material::initialize();
    RebarLayer::initialize();
}

void RebarLayer::initialize(const shared_ptr<DomainBase>& D) {
    if(D != nullptr) {
        if(!D->find_material(tag_major) || !D->find_material(tag_minor)) {
            D->disable_material(get_tag());
            return;
        }

        auto& rebar_major_proto = D->get_material(tag_major);
        if(!rebar_major_proto->initialized) {
            rebar_major_proto->Material::initialize(D);
            rebar_major_proto->initialize(D);
        }
        rebar_major = rebar_major_proto->get_copy();

        auto& rebar_minor_proto = D->get_material(tag_minor);
        if(!rebar_minor_proto->initialized) {
            rebar_minor_proto->Material::initialize(D);
            rebar_minor_proto->initialize(D);
        }
        rebar_minor = rebar_minor_proto->get_copy();
    }

    density = ratio_major * rebar_major->get_parameter() + ratio_minor * rebar_minor->get_parameter();

    initial_stiffness.zeros(3, 3);

    initial_stiffness(0, 0) = ratio_major * rebar_major->get_initial_stiffness().at(0);
    initial_stiffness(1, 1) = ratio_minor * rebar_minor->get_initial_stiffness().at(0);

    initial_stiffness = trans_stress * initial_stiffness * trans_strain;

    trial_stiffness = initial_stiffness;
    current_stiffness = initial_stiffness;
}

unique_ptr<Material> RebarLayer::get_copy() { return make_unique<RebarLayer>(*this); }

int RebarLayer::update_trial_status(const vec& t_strain) {
    trial_strain = t_strain;

    const vec main_strain = trans_strain * trial_strain;

    // update status
    rebar_major->update_trial_status(main_strain(0));
    rebar_minor->update_trial_status(main_strain(1));

    vec main_stress(3);

    // collect main stress components
    main_stress(0) = ratio_major * rebar_major->get_stress().at(0);
    main_stress(1) = ratio_minor * rebar_minor->get_stress().at(0);
    main_stress(2) = 0.;

    // transform back to nominal direction
    trial_stress = trans_stress * main_stress;

    // collect principal stiffness components
    trial_stiffness.zeros(3, 3);
    trial_stiffness(0, 0) = ratio_major * rebar_major->get_stiffness().at(0);
    trial_stiffness(1, 1) = ratio_minor * rebar_minor->get_stiffness().at(0);

    // transform back to nominal direction
    trial_stiffness = trans_stress * trial_stiffness * trans_strain;

    return 0;
}

int RebarLayer::clear_status() {
    current_strain.zeros();
    trial_strain.zeros();
    current_stress.zeros();
    trial_stress.zeros();
    current_stiffness = initial_stiffness;
    trial_stiffness = initial_stiffness;
    auto code = rebar_major->clear_status();
    code += rebar_minor->clear_status();
    return code;
}

int RebarLayer::commit_status() {
    current_strain = trial_strain;
    current_stress = trial_stress;
    current_stiffness = trial_stiffness;
    auto code = rebar_major->commit_status();
    code += rebar_minor->commit_status();
    return code;
}

int RebarLayer::reset_status() {
    trial_strain = current_strain;
    trial_stress = current_stress;
    trial_stiffness = current_stiffness;
    auto code = rebar_major->reset_status();
    code += rebar_minor->reset_status();
    return code;
}
