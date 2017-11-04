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

#include "DSFM.h"
#include <suanPan>

DSFM::DSFM(const unsigned, const double, const double, const double, const double, const double, const double, const double, const double, const double)
    : peak_stress(0)
    , peak_strain(0)
    , initial_modulus(0)
    , initial_zeta(0)
    , crack_stress(0)
    , steel_modulus_l(0)
    , steel_modulus_t(0)
    , rebar_ratio_l(0)
    , rebar_ratio_t(0)
    , hardening_ratio_l(0)
    , hardening_ratio_t(0)
    , yield_stress_l(0)
    , yield_stress_t(0)
    , yield_strain_l(0)
    , yield_strain_t(0)
    , steel_trans(transform::form_strain_trans(inclination)) {}

void DSFM::initialize(const shared_ptr<DomainBase>&) {}

unique_ptr<Material> DSFM::get_copy() { throw; }

int DSFM::update_trial_status(const vec& t_strain) {
    trial_strain = t_strain;

    // compute steel part first
    const vec steel_strain = steel_trans * trial_strain;

    vec steel_stress(3);
    steel_stress(2) = 0.;
    const auto tmp_a = fabs(steel_strain(0));
    steel_stress(0) = tmp_a > yield_strain_l ? (1. - hardening_ratio_l) * yield_stress_l + hardening_ratio_l * steel_modulus_l * tmp_a : steel_modulus_l * tmp_a;
    if(steel_strain(0) < 0.) steel_stress(0) = -steel_stress(0);
    const auto tmp_b = fabs(steel_strain(1));
    steel_stress(1) = tmp_b > yield_strain_t ? (1. - hardening_ratio_t) * yield_stress_t + hardening_ratio_t * steel_modulus_t * tmp_b : steel_modulus_t * tmp_b;
    if(steel_strain(1) < 0.) steel_stress(1) = -steel_stress(1);
    steel_stress(0) *= rebar_ratio_l;
    steel_stress(1) *= rebar_ratio_t;

    steel_stress = steel_trans.t() * steel_stress;

    return 0;
}

int DSFM::clear_status() { throw; }

int DSFM::commit_status() { throw; }

int DSFM::reset_status() { throw; }
