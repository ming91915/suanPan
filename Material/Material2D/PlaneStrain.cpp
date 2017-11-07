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

#include "PlaneStrain.h"
#include <Domain/DomainBase.h>
#include <array>

const array<unsigned, 3> PlaneStrain::F = { 0, 1, 3 };

mat PlaneStrain::form_stiffness(const mat& full_stiffness) {
    mat t_stiffness(3, 3);

    for(auto I = 0; I < 3; ++I)
        for(auto J = 0; J < 3; ++J) t_stiffness(I, J) = full_stiffness(F[I], F[J]);

    return t_stiffness;
}

PlaneStrain::PlaneStrain(const unsigned T, const unsigned BT)
    : Material2D(T, MT_PLANESTRAIN, PlaneType::E, 0.)
    , base_tag(BT) {}

PlaneStrain::PlaneStrain(const PlaneStrain& P)
    : Material2D(P.get_tag(), MT_PLANESTRAIN, PlaneType::E, P.density)
    , base_tag(P.base_tag) {
    if(P.base != nullptr) base = P.base->get_copy();
    Material::initialize();
    PlaneStrain::initialize();
}

void PlaneStrain::initialize(const shared_ptr<DomainBase>& D) {
    if(D != nullptr && D->find_material(base_tag)) {
        auto& material_proto = D->get_material(base_tag);
        if(!material_proto->initialized) {
            material_proto->Material::initialize(D);
            material_proto->initialize(D);
        }
        base = material_proto->get_copy();

        density = base->get_parameter();
    }

    current_full_strain.zeros(6);
    trial_full_strain.zeros(6);

    current_stiffness = trial_stiffness = initial_stiffness = form_stiffness(base->get_initial_stiffness());
}

double PlaneStrain::get_parameter(const ParameterType& P) const { return base->get_parameter(P); }

unique_ptr<Material> PlaneStrain::get_copy() { return make_unique<PlaneStrain>(*this); }

int PlaneStrain::update_trial_status(const vec& t_strain) {
    trial_strain = t_strain;

    for(auto I = 0; I < 3; ++I) trial_full_strain(F[I]) = trial_strain(I);

    base->update_trial_status(trial_full_strain);

    for(auto I = 0; I < 3; ++I) trial_stress(I) = base->get_stress().at(F[I]);

    trial_stiffness = form_stiffness(base->get_stiffness());

    return 0;
}

int PlaneStrain::clear_status() {
    current_full_strain.zeros(6);
    trial_full_strain.zeros(6);
    Material::clear_status();
    return base->clear_status();
}

int PlaneStrain::commit_status() {
    current_full_strain = trial_full_strain;
    Material::commit_status();
    return base->commit_status();
}

int PlaneStrain::reset_status() {
    trial_full_strain = current_full_strain;
    Material::reset_status();
    return base->reset_status();
}
