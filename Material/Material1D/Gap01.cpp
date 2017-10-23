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

#include "Gap01.h"
#include <Toolbox/utility.h>

Gap01::Gap01(const unsigned& T, const double& E, const double& Y, const double& G, const double& R)
    : Material1D(T, MT_GAP01, R)
    , elastic_modulus(E)
    , yield_stress(Y)
    , gap_strain(G) {}

void Gap01::initialize(const shared_ptr<DomainBase>&) {
    initial_stiffness = elastic_modulus;
    current_stiffness = initial_stiffness;
    trial_stiffness = initial_stiffness;

    current_history.zeros(2);
    trial_history.zeros(2);
}

unique_ptr<Material> Gap01::get_copy() { return make_unique<Gap01>(*this); }

int Gap01::update_trial_status(const vec& t_strain) {
    trial_strain = t_strain;
    incre_strain = trial_strain - current_strain;

    if(incre_strain(0) == 0.) return 0;

    trial_history = current_history;

    auto& reverse_strain = trial_history(0);
    auto& load_flag = trial_history(1);

    const auto trial_load_flag = suanpan::sign(incre_strain(0));

    if(load_flag > 0. && trial_load_flag > 0. && current_stress(0) == 0.) {
        // load from silent
        incre_strain = trial_strain - gap_strain - reverse_strain;
        if(incre_strain(0) < 0.) incre_strain(0) = 0.;
    } else if(load_flag < 0. && trial_load_flag > 0.) {
        // unload
        reverse_strain = current_strain(0);
        if(current_stress(0) == 0.) {
            incre_strain = trial_strain - gap_strain - reverse_strain;
            if(incre_strain(0) < 0.) incre_strain(0) = 0.;
        }
    }

    if(trial_load_flag != 0. && trial_load_flag != load_flag) load_flag = trial_load_flag;

    // update and bound stress and stiffness
    trial_stress = current_stress + elastic_modulus * incre_strain;
    trial_stiffness = initial_stiffness;

    // clamp stress and stiffness
    if(trial_stress(0) < 0.) {
        trial_stress(0) = 0.;
        trial_stiffness(0) = 0.;
    } else if(trial_stress(0) > yield_stress) {
        trial_stress(0) = yield_stress;
        trial_stiffness(0) = 0.;
    }

    return 0;
}

int Gap01::clear_status() {
    current_strain.zeros();
    current_stress.zeros();
    current_stiffness = initial_stiffness;
    current_history.zeros();
    return reset_status();
}

int Gap01::commit_status() {
    current_strain = trial_strain;
    current_stress = trial_stress;
    current_stiffness = trial_stiffness;
    current_history = trial_history;
    return 0;
}

int Gap01::reset_status() {
    trial_strain = current_strain;
    trial_stress = current_stress;
    trial_stiffness = current_stiffness;
    return 0;
}
