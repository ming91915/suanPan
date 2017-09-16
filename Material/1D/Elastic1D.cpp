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

#include "Elastic1D.h"

Elastic1D::Elastic1D(const unsigned& T, const double& E, const double& R)
    : Material(T, MT_ELASTIC1D)
    , elastic_modulus(E) {
    density = R;
    Elastic1D::initialize();
}

Elastic1D::Elastic1D(const double& E, const double& R)
    : Material(0, MT_ELASTIC1D)
    , elastic_modulus(E) {
    density = R;
    Elastic1D::initialize();
}

void Elastic1D::initialize() {
    current_strain.zeros(1);
    current_stress.zeros(1);
    trial_strain.zeros(1);
    trial_stress.zeros(1);
    // incre_strain.zeros(1);
    // incre_stress.zeros(1);

    initial_stiffness = elastic_modulus;
    current_stiffness = initial_stiffness;
    trial_stiffness = initial_stiffness;
}

unique_ptr<Material> Elastic1D::get_copy() { return make_unique<Elastic1D>(*this); }

int Elastic1D::update_trial_status(const vec& t_strain) {
    trial_strain = t_strain;
    if(trial_strain(0) == current_strain(0)) return 0;
    trial_stress = elastic_modulus * trial_strain;
    // incre_strain = trial_strain - current_strain;
    // incre_stress = trial_stress - current_stress;
    return 0;
}

int Elastic1D::clear_status() {
    current_strain.zeros(1);
    current_stress.zeros(1);
    trial_strain.zeros(1);
    trial_stress.zeros(1);
    current_stiffness = initial_stiffness;
    trial_stiffness = initial_stiffness;
    return 0;
}

int Elastic1D::commit_status() {
    current_strain = trial_strain;
    current_stress = trial_stress;
    // current_stiffness = trial_stiffness;
    return 0;
}

int Elastic1D::reset_status() {
    trial_strain = current_strain;
    trial_stress = current_stress;
    // trial_stiffness = current_stiffness;
    return 0;
}

void Elastic1D::print() {
    suanpan_info("1-D Elastic Material %u.\n", get_tag());
    suanpan_info("Young's Modulus:\t%.4E.\n", elastic_modulus);
}
