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

const double Concrete01::crack_strain = 8E-5;

Concrete01::Concrete01(const unsigned& T, const double& EP, const double& SP, const BackboneType& TP, const bool& CO, const double& R)
    : Material1D(T, MT_CONCRETE01, R)
    , peak_strain(EP > 0. ? -EP : EP)
    , peak_stress(SP > 0. ? -SP : SP)
    , backbone_type(TP)
    , center_oriented(CO) {}

void Concrete01::initialize(const shared_ptr<DomainBase>&) {
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
    default:
        break;
    }

    current_history.zeros(6);
    trial_history.zeros(6);

    compute_compression_backbone();

    initial_stiffness = trial_stiffness;
    current_stiffness = initial_stiffness;

    crack_stress = initial_stiffness(0) * crack_strain;
    d_factor = crack_stress * pow(crack_strain, .4);
}

unique_ptr<Material> Concrete01::get_copy() { return make_unique<Concrete01>(*this); }

int Concrete01::update_trial_status(const vec& t_strain) {
    trial_strain = t_strain;
    incre_strain = trial_strain - current_strain;

    if(incre_strain(0) == 0.) return 0;

    trial_history = current_history;

    auto& max_strain = trial_history(0);      // maximum compression strain logged
    auto& residual_strain = trial_history(1); // residual strain in unloading path
    auto& trial_cr_strain = trial_history(2); // unloading point strain compression side
    auto& trial_cr_stress = trial_history(3); // unloading point stress compression side
    auto& trial_tr_strain = trial_history(4); // unloading point strain tension side
    auto& trial_tr_stress = trial_history(5); // unloading point stress tension side

    if(trial_strain(0) < max_strain) {
        max_strain = trial_strain(0);
        if(!center_oriented) residual_strain = .145 * max_strain * max_strain / peak_strain + .13 * max_strain;
    }

    const auto strain_a = trial_strain(0) - residual_strain;
    const auto side = suanpan::sign(strain_a);
    const auto load_direction = suanpan::sign(incre_strain(0));

    if(side == -1)
        // the trial position is in compression zone
        // if current position is on backbone
        if(on_compression_backbone) {
            // yes on backbone
            if(load_direction == -1.)
                // loading
                compute_compression_backbone();
            else if(load_direction == 1.) {
                // unloading
                on_compression_backbone = false;
                trial_cr_strain = current_strain(0);
                trial_cr_stress = current_stress(0);
                trial_stiffness = trial_cr_stress / (trial_cr_strain - residual_strain);
                trial_stress = trial_stiffness * strain_a;
            }
        } else if(trial_strain(0) >= trial_cr_strain) {
            // still inside backbone
            trial_stiffness = trial_cr_stress / (trial_cr_strain - residual_strain);
            trial_stress = trial_stiffness * strain_a;
        } else {
            // reload to backbone
            on_compression_backbone = true;
            compute_compression_backbone();
        }
    else {
        // the trial position is in tension zone
        if(on_tension_backbone) {
            // yes on backbone
            if(load_direction == 1.) {
                // loading for first time
                compute_tension_backbone();
            } else if(load_direction == -1.) {
                // unloading
                on_tension_backbone = false;
                trial_tr_strain = current_strain(0);
                trial_tr_stress = current_stress(0);
                trial_stiffness = trial_tr_stress / (trial_tr_strain - residual_strain);
                trial_stress = trial_stiffness * strain_a;
            }
        } else if(trial_strain(0) <= trial_tr_strain) {
            // no not on backbone but still inside of backbone
            trial_stiffness = trial_tr_stress / (trial_tr_strain - residual_strain);
            trial_stress = trial_stiffness * strain_a;
        } else {
            // reloading from unloading path
            on_tension_backbone = true;
            compute_tension_backbone();
        }
    }

    return 0;
}

int Concrete01::clear_status() {
    current_strain.zeros();
    current_stress.zeros();
    current_history.zeros();
    current_stiffness = initial_stiffness;
    return reset_status();
}

int Concrete01::commit_status() {
    current_strain = trial_strain;
    current_stress = trial_stress;
    current_history = trial_history;
    current_stiffness = trial_stiffness;
    return 0;
}

int Concrete01::reset_status() {
    trial_strain = current_strain;
    trial_stress = current_stress;
    trial_history = current_history;
    trial_stiffness = current_stiffness;
    return 0;
}

void Concrete01::compute_compression_backbone() {
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
    case BackboneType::KPSU:
    case BackboneType::KPSC: {
        const auto tmp_a = .5 / ((.29 * peak_stress - 3.) / (145. * peak_stress + 1000.) + peak_strain);
        const auto ultimate_strain = -.8 / tmp_a + peak_strain;
        if(trial_strain(0) < ultimate_strain) {
            trial_stress = 0.;
            trial_stiffness = 0.;
        } else if(trial_strain(0) < peak_strain) {
            trial_stress = peak_stress + peak_stress * tmp_a * (trial_strain(0) - peak_strain);
            trial_stiffness = peak_stress * tmp_a;
        } else {
            trial_stress = peak_stress * normal_strain * (2. - normal_strain);
            trial_stiffness = 2. * peak_stress / peak_strain * (1. - normal_strain);
        }
        break;
    }
    }
}

void Concrete01::compute_tension_backbone() {
    const auto offset = trial_strain(0) - trial_history(1);
    if(offset > crack_strain) {
        // cracking
        trial_stress = d_factor / pow(offset, .4);
        trial_stiffness = -.4 * d_factor / pow(offset, 1.4);
    } else {
        // elastic
        trial_stiffness = initial_stiffness;
        trial_stress = trial_stiffness * offset;
    }
}
