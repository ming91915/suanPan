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
#include <suanPan>

const double CSMM::crack_strain = 8E-5;

CSMM::CSMM(const unsigned T, const double PE, const double PS)
    : Material2D(T, MT_CSMM, PlaneType::S, 0.)
    , peak_strain(PE > 0. ? -PE : PE)
    , peak_stress(PS > 0. ? -PS : PS)
    , crack_stress(.31 * sqrt(-peak_stress))
    , initial_modulus(3875. * sqrt(-peak_stress))
    , steel_modulus(0)
    , yield_stress(0) {}

void CSMM::initialize(const shared_ptr<DomainBase>&) {}

unique_ptr<Material> CSMM::get_copy() { throw; }

int CSMM::update_trial_status(const vec& t_strain) {
    trial_strain = t_strain;

    const auto trans_mat = transform::form_trans(concrete_angle);

    const auto principal_strain = trans_mat * trial_strain;

    return 0;
}

int CSMM::clear_status() { throw; }

int CSMM::commit_status() { throw; }

int CSMM::reset_status() { throw; }
