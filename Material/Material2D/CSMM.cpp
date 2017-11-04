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

#include "CSMM.h"
#include <Toolbox/tensorToolbox.h>

const double CSMM::crack_strain = 8E-5;

CSMM::CSMM(const unsigned T, // tag
    const double PE,         // compression peak strain in negative
    const double PS,         // compression peak stress in negative
    const double SEL,        // steel elastic modulus longitudinal
    const double SET,        // steel elastic modulus transverse
    const double SYSL,       // steel yield stress longitudinal
    const double SYST,       // steel yield stress transverse
    const double SRL,        // steel reinforcement ratio longitudinal
    const double SRT,        // steel reinforcement ratio transverse
    const double)
    : Material2D(T, MT_CSMM, PlaneType::S, 0.)
    , peak_stress(PS > 0. ? -PS : PS)
    , peak_strain(PE > 0. ? -PE : PE)
    , initial_modulus(3875. * sqrt(-peak_stress))
    , initial_zeta(peak_stress < -41.53 ? 5.8 / sqrt(-peak_stress) : .9)
    , crack_stress(initial_modulus * crack_strain)
    , steel_modulus_l(SEL)
    , steel_modulus_t(SET)
    , rebar_ratio_l(SRL)
    , rebar_ratio_t(SRT)
    , b_l(pow(crack_stress / SYSL, 1.5) / rebar_ratio_l < 1.5E-3 ? 1.5E-3 : rebar_ratio_l)
    , b_t(pow(crack_stress / SYST, 1.5) / rebar_ratio_t < 1.5E-3 ? 1.5E-3 : rebar_ratio_t)
    , yield_stress_l((.93 - 2. * b_l) * SYSL)
    , yield_stress_t((.93 - 2. * b_t) * SYST)
    , yield_strain_l(yield_stress_l / steel_modulus_l)
    , yield_strain_t(yield_stress_t / steel_modulus_t) {}

void CSMM::initialize(const shared_ptr<DomainBase>&) {}

unique_ptr<Material> CSMM::get_copy() { return make_unique<CSMM>(*this); }

int CSMM::update_trial_status(const vec& t_strain) {
    trial_strain = t_strain;

    vec concrete_strain(3), concrete_stress(3), steel_stress(3);
    steel_stress(2) = 0.;

    auto counter = 0;

    auto flag = false;

    while(true) {
        // maximum iteration to avoid infinite loop
        if(++counter > 50) break;

        // transform trial strain to principal direction
        const auto principal_strain = principal_angle == 0. ? trial_strain : transform::rotate_strain(trial_strain, principal_angle);

        // find two ratios
        double gamma_12;

        const auto& max_tension = principal_strain(0);
        if(max_tension > 2E-3) {
            if(!cracked) cracked = true;
            gamma_12 = 1.9;
        } else
            gamma_12 = max_tension >= 0. ? .2 + 850. * max_tension : .2;

        const auto factor_a = cracked ? 1. : 5. / (5. - gamma_12);

        // get concrete strain accouting for poissons effect
        concrete_strain(2) = principal_strain(2);
        // concrete tension strain
        concrete_strain(0) = cracked ? principal_strain(0) + gamma_12 * principal_strain(1) : factor_a * (principal_strain(0) + gamma_12 * principal_strain(1));
        // concrete compression strain
        concrete_strain(1) = cracked ? principal_strain(1) : factor_a * (.2 * principal_strain(0) + principal_strain(1));

        // tension
        if(concrete_strain(0) > 0.) {
            // concrete tension stress
            concrete_stress(0) = concrete_strain(0) > crack_strain ? crack_stress * pow(crack_strain / concrete_strain(0), .4) : initial_modulus * concrete_strain(0);
        } else {
            const auto factor_b = concrete_strain(0) / peak_strain / initial_zeta;
            concrete_stress(0) = factor_b > 1. ? 1. - pow((factor_b - 1.) / (4. / initial_zeta - 1), 2.) : (2. - factor_b) * factor_b;
            concrete_stress(0) *= initial_zeta * peak_stress;
        }

        if(concrete_strain(1) > 0.) {
            concrete_stress(1) = concrete_strain(1) > crack_strain ? crack_stress * pow(crack_strain / concrete_strain(1), .4) : initial_modulus * concrete_strain(1);
        } else {
            auto zeta = initial_zeta;
            if(concrete_strain(0) > 0.) zeta /= sqrt(1. + 400. * concrete_strain(0));
            const auto factor_b = concrete_strain(1) / peak_strain / zeta;

            // concrete compression stress
            concrete_stress(1) = factor_b > 1. ? 1. - pow((factor_b - 1.) / (4. / zeta - 1), 2.) : (2. - factor_b) * factor_b;
            concrete_stress(1) *= zeta * peak_stress;
        }

        // concrete shear stress
        concrete_stress(2) = .5 * (concrete_stress(0) - concrete_stress(1)) / (principal_strain(0) - principal_strain(1)) * principal_strain(2);

        // transform unixial strain to steel orientation
        const auto steel_strain = transform::rotate_strain(concrete_strain, inclination - principal_angle);
        // part two to get steel response
        steel_stress(0) = steel_modulus_l * steel_strain(0);
        steel_stress(1) = steel_modulus_t * steel_strain(1);
        if(fabs(steel_strain(0)) > yield_strain_l) {
            steel_stress(0) *= .02 + .25 * b_l;
            steel_stress(0) += steel_strain(0) > 0. ? yield_stress_l : -yield_stress_l;
        }
        if(fabs(steel_strain(1)) > yield_strain_t) {
            steel_stress(1) *= .02 + .25 * b_t;
            steel_stress(1) += steel_strain(1) > 0. ? yield_stress_t : -yield_stress_t;
        }
        steel_stress(0) *= rebar_ratio_l;
        steel_stress(1) *= rebar_ratio_t;

        // part three rotate back to nominal direction
        trial_stress = principal_angle == 0. ? concrete_stress : transform::rotate_stress(concrete_stress, -principal_angle);
        trial_stress += inclination == 0. ? steel_stress : transform::rotate_stress(steel_stress, -inclination);

        // get trial principal angle
        const auto trial_principal_angle = transform::stress_angle(trial_stress);

        if(fabs(trial_principal_angle - principal_angle) < 1E-4) {
            flag = true;
            break;
        }

        principal_angle = trial_principal_angle;
    }

    if(!flag) suanpan_warning("cannot converge within 50 iterations, use the latest approximation.\n");

    return 0;
}

int CSMM::clear_status() { throw; }

int CSMM::commit_status() { throw; }

int CSMM::reset_status() { throw; }
