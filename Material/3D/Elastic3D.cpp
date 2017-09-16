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

#include "Elastic3D.h"

Elastic3D::Elastic3D(const unsigned& T, const double& E, const double& P, const double& R)
    : Material(T, MT_ELASTIC3D)
    , elastic_modulus(E)
    , poissons_ratio(P) {
    density = R;
    Elastic3D::initialize();
}

Elastic3D::Elastic3D(const double& E, const double& P, const double& R)
    : Material(0, MT_ELASTIC3D)
    , elastic_modulus(E)
    , poissons_ratio(P) {
    density = R;
    Elastic3D::initialize();
}

Elastic3D::~Elastic3D() {}

void Elastic3D::initialize() {
    current_strain.zeros(6);
    current_stress.zeros(6);
    trial_strain.zeros(6);
    trial_stress.zeros(6);
    // incre_strain.zeros(6);
    // incre_stress.zeros(6);

    const auto shear_modulus = elastic_modulus / (2. + 2. * poissons_ratio);
    const auto lambda = shear_modulus * poissons_ratio / (.5 - poissons_ratio);

    initial_stiffness.zeros(6, 6);

    for(auto I = 0; I < 3; ++I)
        for(auto J = 0; J < 3; ++J) initial_stiffness(I, J) = lambda;

    for(auto I = 0; I < 3; ++I) initial_stiffness(I, I) += 2. * shear_modulus;

    for(auto I = 3; I < 6; ++I) initial_stiffness(I, I) = shear_modulus;

    current_stiffness = initial_stiffness;
    trial_stiffness = initial_stiffness;
}

unique_ptr<Material> Elastic3D::get_copy() { return make_unique<Elastic3D>(*this); }

int Elastic3D::update_trial_status(const vec& t_strain) {
    trial_strain = t_strain;
    trial_stress = trial_stiffness * trial_strain;
    // incre_strain = trial_strain - current_strain;
    // incre_stress = trial_stress - current_stress;
    return 0;
}

int Elastic3D::clear_status() {
    current_strain.zeros(6);
    current_stress.zeros(6);
    trial_strain.zeros(6);
    trial_stress.zeros(6);
    current_stiffness = initial_stiffness;
    trial_stiffness = initial_stiffness;
    return 0;
}

int Elastic3D::commit_status() {
    current_strain = trial_strain;
    current_stress = trial_stress;
    // current_stiffness = trial_stiffness;
    return 0;
}

int Elastic3D::reset_status() {
    trial_strain = current_strain;
    trial_stress = current_stress;
    // trial_stiffness = current_stiffness;
    return 0;
}

void Elastic3D::print() { suanpan_info("Isotropic Elastic Material.\n"); }
