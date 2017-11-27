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

#include "BilinearElastic1D.h"
#include <Toolbox/utility.h>

BilinearElastic1D::BilinearElastic1D(const unsigned& T, const double& E, const double& Y, const double& H, const double& R)
    : Material1D(T, MT_BILINEARELASTIC1D, R)
    , elastic_modulus(E)
    , yield_stress(Y > 0. ? Y : -Y)
    , yield_strain(yield_stress / elastic_modulus)
    , hardening_modulus(elastic_modulus * H) {}

void BilinearElastic1D::initialize(const shared_ptr<DomainBase>&) { trial_stiffness = current_stiffness = initial_stiffness = elastic_modulus; }

unique_ptr<Material> BilinearElastic1D::get_copy() { return make_unique<BilinearElastic1D>(*this); }

int BilinearElastic1D::update_trial_status(const vec& t_strain) {
    trial_strain = t_strain;

    const auto abs_strain = abs(trial_strain(0));
    const auto sign_strain = suanpan::sign(trial_strain(0));

    if(abs_strain > yield_strain) {
        trial_stiffness = hardening_modulus;
        trial_stress = yield_stress + (abs_strain - yield_strain) * trial_stiffness;
    } else {
        trial_stiffness = elastic_modulus;
        trial_stress = abs_strain * elastic_modulus;
    }

    if(sign_strain < 0.) trial_stress = -trial_stress;

    return 0;
}

int BilinearElastic1D::clear_status() {
    current_strain.zeros();
    current_stress.zeros();
    current_stiffness = initial_stiffness;
    return reset_status();
}

int BilinearElastic1D::commit_status() {
    current_strain = trial_strain;
    current_stress = trial_stress;
    current_stiffness = trial_stiffness;
    return 0;
}

int BilinearElastic1D::reset_status() {
    trial_strain = current_strain;
    trial_stress = current_stress;
    trial_stiffness = current_stiffness;
    return 0;
}
