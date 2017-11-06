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

#include "Concrete2D.h"
#include "Toolbox/utility.h"
#include <Domain/DomainBase.h>
#include <Toolbox/tensorToolbox.h>

Concrete2D::Concrete2D(const unsigned T, const unsigned M, const double P, const PlaneType PT)
    : Material2D(T, MT_CONCRETE2D, PT, 0.)
    , concrete_tag(M)
    , poissons_ratio(P) {}

Concrete2D::Concrete2D(const Concrete2D& P)
    : Material2D(P.get_tag(), MT_CONCRETE2D, P.plane_type, P.density)
    , concrete_tag(P.concrete_tag)
    , poissons_ratio(P.poissons_ratio) {
    if(P.concrete_major != nullptr) concrete_major = P.concrete_major->get_copy();
    if(P.concrete_minor != nullptr) concrete_minor = P.concrete_minor->get_copy();
    Material::initialize();
    Concrete2D::initialize();
}

void Concrete2D::initialize(const shared_ptr<DomainBase>& D) {
    if(D != nullptr) {
        if(!D->find_material(concrete_tag)) {
            D->disable_material(get_tag());
            return;
        }

        auto& concrete_proto = D->get_material(concrete_tag);
        if(!concrete_proto->initialized) {
            concrete_proto->Material::initialize(D);
            concrete_proto->initialize(D);
        }

        concrete_major = concrete_proto->get_copy();
        concrete_minor = concrete_proto->get_copy();
    }

    density = concrete_major->get_parameter();

    const auto& E11 = concrete_major->get_initial_stiffness().at(0);
    const auto VV = plane_type == PlaneType::S ? poissons_ratio : poissons_ratio / (1. - poissons_ratio);

    const auto t_factor = (plane_type == PlaneType::S ? E11 : E11 / (1. - poissons_ratio * poissons_ratio)) / (1. - VV * VV);

    initial_stiffness.zeros(3, 3);
    initial_stiffness(0, 1) = initial_stiffness(1, 0) = VV * (initial_stiffness(0, 0) = initial_stiffness(1, 1) = t_factor);
    initial_stiffness(2, 2) = .5 * t_factor * (1. - VV);

    trial_stiffness = initial_stiffness;
    current_stiffness = initial_stiffness;
}

unique_ptr<Material> Concrete2D::get_copy() { return make_unique<Concrete2D>(*this); }

int Concrete2D::update_trial_status(const vec& t_strain) {
    trial_strain = t_strain;

    principal_direction = transform::strain_angle(trial_strain);

    const auto trans_mat = transform::form_strain_trans(principal_direction);

    const auto principal_strain = transform::nominal_to_principal_strain(trial_strain);

    // update status
    concrete_major->update_trial_status(principal_strain(0));
    concrete_minor->update_trial_status(principal_strain(1));

    vec principal_stress(3);

    // collect principal stress components
    principal_stress(0) = concrete_major->get_stress().at(0);
    principal_stress(1) = concrete_minor->get_stress().at(0);
    principal_stress(2) = 0.;

    // collect principal stiffness components
    trial_stiffness.zeros(3, 3);
    trial_stiffness(0, 0) = concrete_major->get_stiffness().at(0);
    trial_stiffness(1, 1) = concrete_minor->get_stiffness().at(0);

    // tension combined with compression consider coupled effect
    if(suanpan::sign(principal_strain(0)) > 0. && suanpan::sign(principal_strain(1)) < 0.) {
        const auto coupled = sqrt(1. + 400 * principal_strain(0));
        principal_stress(1) /= coupled;
        trial_stiffness(1, 1) /= coupled;
    }

    trial_stiffness(2, 2) = trial_stiffness(0, 0) * trial_stiffness(1, 1) / (trial_stiffness(0, 0) + trial_stiffness(1, 1));

    // transform back to nominal direction
    trial_stress = trans_mat.t() * principal_stress;

    // transform back to nominal direction
    trial_stiffness = trans_mat.t() * trial_stiffness * trans_mat;

    return 0;
}

int Concrete2D::clear_status() {
    current_strain.zeros();
    trial_strain.zeros();
    current_stress.zeros();
    trial_stress.zeros();
    current_stiffness = initial_stiffness;
    trial_stiffness = initial_stiffness;
    auto code = concrete_major->clear_status();
    code += concrete_minor->clear_status();
    return code;
}

int Concrete2D::commit_status() {
    current_strain = trial_strain;
    current_stress = trial_stress;
    current_stiffness = trial_stiffness;
    auto code = concrete_major->commit_status();
    code += concrete_minor->commit_status();
    return code;
}

int Concrete2D::reset_status() {
    trial_strain = current_strain;
    trial_stress = current_stress;
    trial_stiffness = current_stiffness;
    auto code = concrete_major->reset_status();
    code += concrete_minor->reset_status();
    return code;
}
