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

const double PlaneStressBilinear::two_third = 2. / 3.;
const double PlaneStressBilinear::root_two_third = sqrt(2. / 3.);

PlaneStressBilinear::PlaneStressBilinear(const unsigned T, const double E, const double V, const double Y, const double H, const double B, const double D)
    : Material2D(T, MT_PLANESTRESSBILINEAR, PlaneType::S, D)
    , elastic_modulus(E)
    , poissons_ratio(V)
    , yield_stress(Y)
    , hardening_ratio(H)
    , beta(B)
    , plastic_modulus(elastic_modulus * hardening_ratio / (1. - hardening_ratio))
    , tolerance(1E-14 * yield_stress)
    , factor_a(two_third * plastic_modulus)
    , factor_b((1. - beta) * plastic_modulus)
    , factor_c(elastic_modulus / 3. / (1. - poissons_ratio) + factor_a)
    , factor_d(elastic_modulus / (1. + poissons_ratio) + factor_a) {}

void PlaneStressBilinear::initialize(const shared_ptr<DomainBase>&) {
    const auto t_factor = elastic_modulus / (1. - poissons_ratio * poissons_ratio);
    initial_stiffness.zeros(3, 3);
    initial_stiffness(2, 2) = .5 * (1. - poissons_ratio) * (initial_stiffness(0, 0) = initial_stiffness(1, 1) = t_factor);
    initial_stiffness(0, 1) = initial_stiffness(1, 0) = t_factor * poissons_ratio;

    if(P.is_empty()) {
        P.zeros(3, 3);
        P(1, 1) = P(0, 0) = two_third;
        P(0, 1) = P(1, 0) = -.5 * two_third;
        P(2, 2) = 2.;
    }

    current_stiffness = initial_stiffness;
    trial_stiffness = initial_stiffness;

    inv_stiffness = inv(initial_stiffness);

    current_plastic_strain = 0.;
    trial_plastic_strain = 0.;

    current_equivalent_strain.zeros(3);
    trial_equivalent_strain.zeros(3);

    current_back_stress.zeros(3);
    trial_back_stress.zeros(3);
}

double PlaneStressBilinear::get_parameter(const ParameterType& T) const { return 0.; }

unique_ptr<Material> PlaneStressBilinear::get_copy() { return make_unique<PlaneStressBilinear>(*this); }

int PlaneStressBilinear::update_incre_status(const vec& i_strain) { return update_trial_status(current_strain + i_strain); }

int PlaneStressBilinear::update_trial_status(const vec& t_strain) {
    trial_plastic_strain = current_plastic_strain;
    trial_back_stress = current_back_stress;
    trial_equivalent_strain = current_equivalent_strain;
    trial_stiffness = initial_stiffness;

    trial_strain = t_strain;
    incre_strain = trial_strain - current_strain;
    trial_stress = trial_stiffness * (trial_strain - trial_equivalent_strain);

    const vec shifted_stress = trial_stress - trial_back_stress;

    const auto t_factor_c = yield_stress + factor_b * trial_plastic_strain;

    const auto yield_func = sqrt(dot(shifted_stress, P * shifted_stress)) - root_two_third * t_factor_c;

    if(yield_func > tolerance) {
        // some constants used in iteration
        const auto sqrt_two = sqrt(2.);                                         // 3.4.14
        const auto eta_11 = (shifted_stress(1) + shifted_stress(0)) / sqrt_two; // 3.4.14
        const auto eta_22 = (shifted_stress(1) - shifted_stress(0)) / sqrt_two; // 3.4.14
        const auto& eta_12 = shifted_stress(2);                                 // 3.4.14

        const auto eta_a = eta_11 * eta_11 / 3.;                   // 3.4.18
        const auto eta_b = eta_22 * eta_22 + 2. * eta_12 * eta_12; // 3.4.18

        auto incre_gamma = 0.;
        while(true) {
            const auto f_bar_square = eta_a / pow(1. + factor_c * incre_gamma, 2.) + eta_b / pow(1. + factor_d * incre_gamma, 2.); // 3.4.18
            const auto r_square = pow(t_factor_c + factor_b * root_two_third * incre_gamma * sqrt(f_bar_square), 2.) / 3.;         // 3.4.18

            const auto f_square = .5 * f_bar_square - r_square; // 3.4.19

            const auto theta_a = 1. + factor_a * incre_gamma;             // 3.4.10.1
            const auto theta_b = 1. - two_third * factor_b * incre_gamma; // 3.4.10.2

            const mat X = inv_stiffness + incre_gamma / theta_a * P;

            const vec t_a = solve(X, inv_stiffness * shifted_stress) / theta_a;
        }
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
