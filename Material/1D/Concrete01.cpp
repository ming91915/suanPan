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

#include "Concrete01.h"
#include <Toolbox/utility.h>

void Concrete01::initialize() {
    switch(backbone_type) {
    case BackboneType::POPOVICS: {
        N = .05802 * abs(peak_stress) + 1;
        break;
    }
    case BackboneType::THORENFELDT: {
        M = 1.;
        N = .85 + abs(peak_stress) / 17.;
        break;
    }
    case BackboneType::TSAI: {
        M = 1. + 17.9 / abs(peak_stress);
        N = abs(peak_stress) / 6.68 - 1.85;
        if(N < 1.) N = 1.;
        break;
    }
    }

    current_strain.zeros(1);
    current_stress.zeros(1);

    trial_strain.zeros(1);
    trial_stress.zeros(1);

    compute_backbone();
    initial_stiffness = trial_stiffness;
    current_stiffness = initial_stiffness;
}

unique_ptr<Material> Concrete01::get_copy() { return make_unique<Concrete01>(*this); }

int Concrete01::update_incre_status(const vec& i_strain) { return update_trial_status(current_strain + i_strain); }

int Concrete01::update_trial_status(const vec& t_strain) {
    trial_strain = t_strain;
    incre_strain = trial_strain - current_strain;

    const auto side = sign(trial_strain(0));
    const auto load_direction = sign(incre_strain(0));

    if(side == -1)
        // the trial position is in compression zone
        // if current position is on backbone
        if(on_backbone)
            // yes on backbone
            if(load_direction == -1)
                // loading
                compute_backbone();
            else {
                // unloading
                on_backbone = false;
                trial_reverse_strain = current_strain(0);
                trial_reverse_stress = current_stress(0);
                trial_stiffness(0) = trial_reverse_stress / trial_reverse_strain;
                trial_stress(0) = trial_stiffness(0) * trial_strain(0);
            }
        else if(trial_strain(0) >= current_reverse_strain)
            // still inside of backbone
            trial_stress(0) = trial_stiffness(0) * trial_strain(0);
        else {
            // reload to backbone
            on_backbone = true;
            compute_backbone();
        }
    else {
        // the trial position is in tension zone
        trial_stress.zeros();
        trial_stiffness.zeros();
    }

    return 0;
}

int Concrete01::clear_status() {
    current_strain.zeros();
    current_stress.zeros();

    trial_strain.zeros();
    trial_stress.zeros();

    current_stiffness = initial_stiffness;
    trial_stiffness = initial_stiffness;

    current_reverse_strain = 0.;
    current_reverse_stress = 0.;
    trial_reverse_strain = 0.;
    trial_reverse_stress = 0.;

    return 0;
}

int Concrete01::commit_status() {
    current_strain = trial_strain;
    current_stress = trial_stress;
    current_stiffness = trial_stiffness;
    current_reverse_strain = trial_reverse_strain;
    current_reverse_stress = trial_reverse_stress;
    return 0;
}

int Concrete01::reset_status() {
    trial_strain = current_strain;
    trial_stress = current_strain;
    trial_stiffness = current_stiffness;
    trial_reverse_strain = current_reverse_strain;
    trial_reverse_stress = current_reverse_stress;
    return 0;
}

void Concrete01::compute_backbone() {
    const auto normal_strain = trial_strain(0) / peak_strain;
    switch(backbone_type) {
    case BackboneType::POPOVICS: {
        const auto tmp_a = pow(normal_strain, N) - 1.;
        const auto tmp_b = tmp_a + N;
        trial_stress = peak_stress * normal_strain * N / tmp_b;
        trial_stiffness = peak_stress * N * tmp_a * (1. - N) / peak_strain / tmp_b / tmp_b;
        break;
    }
    case BackboneType::THORENFELDT: {
        const auto tmp_a = pow(normal_strain, N * M);
        const auto tmp_b = N - 1. + tmp_a;
        trial_stress = peak_stress * normal_strain * N / tmp_b;
        trial_stiffness = N * ((N - 1.) * normal_strain - (M * N - 1.) * tmp_a) / peak_strain / tmp_b / tmp_b;
        break;
    }
    case BackboneType::TSAI: {
        const auto tmp_a = pow(normal_strain, N) - 1.;
        const auto tmp_b = N - 1.;
        trial_stress = peak_stress * normal_strain * M / (1. + (M - N / tmp_b) * normal_strain + (tmp_a + 1.) / tmp_b);
        trial_stiffness = -peak_stress * M * tmp_a * tmp_b * tmp_b / peak_strain / pow(tmp_a + N + normal_strain * (M * N - M - N), 2.);
        break;
    }
    }
}
