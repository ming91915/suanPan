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

#include "PlaneStressBilinear.h"

const double PlaneStressBilinear::root_two_third = sqrt(2. / 3.);

PlaneStressBilinear::PlaneStressBilinear(const unsigned T, const double E, const double V, const double Y, const double H, const double B, const double D)
    : Material2D(T, MT_PLANESTRESSBILINEAR, PlaneType::S, D)
    , elastic_modulus(E)
    , poissons_ratio(V)
    , yield_stress(Y)
    , hardening_ratio(H)
    , beta(B)
    , plastic_modulus(elastic_modulus * hardening_ratio / (1. - hardening_ratio))
    , tolerance(1E-14 * yield_stress) {}

void PlaneStressBilinear::initialize(const shared_ptr<DomainBase>&) {
    const auto t_factor = elastic_modulus / (1. - poissons_ratio * poissons_ratio);
    initial_stiffness.zeros(3, 3);
    initial_stiffness(2, 2) = .5 * (1. - poissons_ratio) * (initial_stiffness(0, 0) = initial_stiffness(1, 1) = t_factor);
    initial_stiffness(0, 1) = initial_stiffness(1, 0) = t_factor * poissons_ratio;

    if(P.is_empty()) {
        P.zeros(3, 3);
        P(1, 1) = P(0, 0) = 2. / 3.;
        P(0, 1) = P(1, 0) = -1. / 3.;
        P(2, 2) = 2.;
        CP = initial_stiffness * P;
        PCP = P * CP;
    }

    current_stiffness = initial_stiffness;
    trial_stiffness = initial_stiffness;
}

double PlaneStressBilinear::get_parameter(const ParameterType& T) const { return 0.; }

unique_ptr<Material> PlaneStressBilinear::get_copy() { return make_unique<PlaneStressBilinear>(*this); }

int PlaneStressBilinear::update_incre_status(const vec& i_strain) { return update_trial_status(current_strain + i_strain); }

int PlaneStressBilinear::update_trial_status(const vec& t_strain) {
    trial_strain = t_strain;
    incre_strain = trial_strain - current_strain;
    trial_stress = current_stress + initial_stiffness * incre_strain;
    trial_back_stress = current_back_stress;
    trial_plastic_strain = current_plastic_strain;
    trial_stiffness = initial_stiffness;

    const vec shifted_stress = trial_stress - current_back_stress;

    const auto yield_func = sqrt(as_scalar(shifted_stress.t() * P * shifted_stress)) - root_two_third * (yield_stress + (1. - beta) * plastic_modulus * current_plastic_strain);

    if(yield_func > tolerance) {
        const auto tmp_a = sqrt(as_scalar(shifted_stress.t() * PCP * shifted_stress));
        const vec unit_norm = CP * shifted_stress / tmp_a;
        trial_stiffness -= unit_norm * unit_norm.t() / beta;
    }

    return 0;
}

int PlaneStressBilinear::clear_status() {
    current_strain.zeros();
    current_stress.zeros();
    current_stiffness = initial_stiffness;
    current_back_stress.zeros();
    current_plastic_strain = 0.;
    return reset_status();
}

int PlaneStressBilinear::commit_status() {
    current_strain = trial_strain;
    current_stress = trial_stress;
    current_stiffness = trial_stiffness;
    current_back_stress = trial_back_stress;
    current_plastic_strain = trial_plastic_strain;
    return 0;
}

int PlaneStressBilinear::reset_status() {
    trial_strain = current_strain;
    trial_stress = current_stress;
    trial_stiffness = current_stiffness;
    trial_back_stress = current_back_stress;
    trial_plastic_strain = current_plastic_strain;
    return 0;
}
