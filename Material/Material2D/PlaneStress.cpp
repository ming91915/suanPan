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

#include "PlaneStress.h"
#include <Domain/DomainBase.h>
#include <array>

const array<unsigned, 3> PlaneStress::F = { 0, 1, 3 };

mat PlaneStress::form_stiffness(const mat& full_stiffness) {
    mat t_stiffness(3, 3);

    for(auto I = 0; I < 3; ++I)
        for(auto J = 0; J < 3; ++J) t_stiffness(I, J) = full_stiffness(F[I], F[J]);

    if(full_stiffness(2, 2) == 0.) {
        suanpan_error("K(2,2)=0.\n");
        return t_stiffness;
    }

    for(auto I = 0; I < 3; ++I)
        for(auto J = 0; J < 3; ++J) t_stiffness(I, J) -= full_stiffness(F[I], 2) * full_stiffness(2, F[J]) / full_stiffness(2, 2);

    return t_stiffness;
}

PlaneStress::PlaneStress(const unsigned T, const unsigned BT)
    : Material2D(T, MT_PLANESTRESS, PlaneType::S, 0.)
    , base_tag(BT) {}

PlaneStress::PlaneStress(const PlaneStress& P)
    : Material2D(P.get_tag(), MT_PLANESTRESS, PlaneType::S, P.density)
    , base_tag(P.base_tag) {
    if(P.base != nullptr) base = P.base->get_copy();
    Material::initialize();
    PlaneStress::initialize();
}

void PlaneStress::initialize(const shared_ptr<DomainBase>& D) {
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

double PlaneStress::get_parameter(const ParameterType& P) const { return base->get_parameter(P); }

unique_ptr<Material> PlaneStress::get_copy() { return make_unique<PlaneStress>(*this); }

int PlaneStress::update_trial_status(const vec& t_strain) {
    trial_strain = t_strain;

    for(auto I = 0; I < 3; ++I) trial_full_strain(F[I]) = trial_strain(I);

    auto& stress_anchor = base->get_stress().at(2);

    auto counter = 0;
    while(true) {
        base->update_trial_status(trial_full_strain);
        trial_full_strain(2) -= stress_anchor / base->get_stiffness().at(2, 2);
        if(fabs(stress_anchor) < 1E-10) break;
        if(++counter > 20) {
            suanpan_warning("cannot converge in 20 iterations.\n");
            break;
        }
    }

    for(auto I = 0; I < 3; ++I) trial_stress(I) = base->get_stress().at(F[I]);

    trial_stiffness = form_stiffness(base->get_stiffness());

    return 0;
}

int PlaneStress::clear_status() {
    current_full_strain.zeros(6);
    trial_full_strain.zeros(6);
    Material::clear_status();
    return base->clear_status();
}

int PlaneStress::commit_status() {
    current_full_strain = trial_full_strain;
    Material::commit_status();
    return base->commit_status();
}

int PlaneStress::reset_status() {
    trial_full_strain = current_full_strain;
    Material::reset_status();
    return base->reset_status();
}
