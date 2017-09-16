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

#include "Elastic2D.h"

Elastic2D::Elastic2D(const unsigned& T, const double& E, const double& P, const double& R, const unsigned& TP)
    : Material(T, MT_ELASTIC2D)
    , elastic_modulus(E)
    , poissons_ratio(P)
    , material_type(TP) {
    density = R;

    const auto EE = material_type == 0 ? elastic_modulus : elastic_modulus / (1. - poissons_ratio * poissons_ratio);
    const auto VV = material_type == 0 ? poissons_ratio : poissons_ratio / (1. - poissons_ratio);

    initial_stiffness.zeros(3, 3);
    initial_stiffness(0, 0) = 1.;
    initial_stiffness(1, 1) = 1.;
    initial_stiffness(2, 2) = (1. - VV) / 2.;
    initial_stiffness(0, 1) = VV;
    initial_stiffness(1, 0) = VV;
    initial_stiffness *= EE / (1. - VV * VV);

    Elastic2D::initialize();
}

Elastic2D::Elastic2D(const double& E, const double& P, const double& R, const unsigned& TP)
    : Material(0, MT_ELASTIC2D)
    , elastic_modulus(E)
    , poissons_ratio(P)
    , material_type(TP) {
    density = R;

    const auto EE = material_type == 0 ? elastic_modulus : elastic_modulus / (1. - poissons_ratio * poissons_ratio);
    const auto VV = material_type == 0 ? poissons_ratio : poissons_ratio / (1. - poissons_ratio);

    initial_stiffness.zeros(3, 3);
    initial_stiffness(0, 0) = 1.;
    initial_stiffness(1, 1) = 1.;
    initial_stiffness(2, 2) = (1. - VV) / 2.;
    initial_stiffness(0, 1) = VV;
    initial_stiffness(1, 0) = VV;
    initial_stiffness *= EE / (1. - VV * VV);

    Elastic2D::initialize();
}

Elastic2D::~Elastic2D() {}

void Elastic2D::initialize() {
    current_strain.zeros(3);
    current_stress.zeros(3);
    trial_strain.zeros(3);
    trial_stress.zeros(3);

    current_stiffness = initial_stiffness;
    trial_stiffness = initial_stiffness;
}

unique_ptr<Material> Elastic2D::get_copy() { return make_unique<Elastic2D>(*this); }

int Elastic2D::update_incre_status(const vec& i_strain) { return update_trial_status(current_strain + i_strain); }

int Elastic2D::update_trial_status(const vec& t_strain) {
    trial_strain = t_strain;
    trial_stress = trial_stiffness * trial_strain;
    // incre_strain = trial_strain - current_strain;
    // incre_stress = trial_stress - current_stress;
    return 0;
}

int Elastic2D::clear_status() {
    initialize();
    return 0;
}

int Elastic2D::commit_status() {
    current_strain = trial_strain;
    current_stress = trial_stress;
    // current_stiffness = trial_stiffness;
    return 0;
}

int Elastic2D::reset_status() {
    trial_strain = current_strain;
    trial_stress = current_stress;
    // trial_stiffness = current_stiffness;
    return 0;
}

void Elastic2D::print() {
    suanpan_info("Strain: ");
    get_strain().t().print();
    suanpan_info("Stress: ");
    get_stress().t().print();
}

double Elastic2D::get_parameter(const unsigned& F) const {
    switch(F) {
    case 0:
        return density;
    case 1:
        return poissons_ratio;
    default:
        return 0.;
    }
}
