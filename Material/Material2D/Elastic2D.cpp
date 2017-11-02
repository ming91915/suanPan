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
#include <Recorder/OutputType.h>

Elastic2D::Elastic2D(const unsigned& T, const double& E, const double& P, const double& R, const PlaneType& PT)
    : Material2D(T, MT_ELASTIC2D, PT, R)
    , elastic_modulus(E)
    , poissons_ratio(P) {}

void Elastic2D::initialize(const shared_ptr<DomainBase>&) {
    const auto EE = plane_type == PlaneType::S ? elastic_modulus : elastic_modulus / (1. - poissons_ratio * poissons_ratio);
    const auto VV = plane_type == PlaneType::S ? poissons_ratio : poissons_ratio / (1. - poissons_ratio);

    const auto t_factor = EE / (1. - VV * VV);

    initial_stiffness.zeros(3, 3);
    initial_stiffness(0, 1) = initial_stiffness(1, 0) = VV * (initial_stiffness(0, 0) = initial_stiffness(1, 1) = t_factor);
    initial_stiffness(2, 2) = .5 * t_factor * (1. - VV);

    current_stiffness = initial_stiffness;
    trial_stiffness = initial_stiffness;
}

unique_ptr<Material> Elastic2D::get_copy() { return make_unique<Elastic2D>(*this); }

int Elastic2D::update_trial_status(const vec& t_strain) {
    trial_strain = t_strain;
    trial_stress = trial_stiffness * trial_strain;
    return 0;
}

int Elastic2D::clear_status() {
    current_strain.zeros();
    current_stress.zeros();
    return reset_status();
}

int Elastic2D::commit_status() {
    current_strain = trial_strain;
    current_stress = trial_stress;
    return 0;
}

int Elastic2D::reset_status() {
    trial_strain = current_strain;
    trial_stress = current_stress;
    return 0;
}

void Elastic2D::print() {
    suanpan_info("Strain: ");
    get_strain().t().print();
    suanpan_info("Stress: ");
    get_stress().t().print();
}

double Elastic2D::get_parameter(const ParameterType& T) const {
    switch(T) {
    case ParameterType::DENSITY:
        return density;
    case ParameterType::POISSONSRATIO:
        return poissons_ratio;
    default:
        return 0.;
    }
}

vector<vec> Elastic2D::record(const OutputType& P) {
    vector<vec> output;
    output.reserve(1);

    const auto sigma_33 = elastic_modulus * poissons_ratio / (1. + poissons_ratio) / (1. - 2. * poissons_ratio) * (trial_strain(0) + trial_strain(1));

    if(P == OutputType::MISES) {
        vec trial_mises(1);
        if(plane_type == PlaneType::S) {
            trial_mises(0) = sqrt(trial_stress(0) * trial_stress(0) - trial_stress(0) * trial_stress(1) + trial_stress(1) * trial_stress(1) + 3. * trial_stress(2) * trial_stress(2));
        } else if(plane_type == PlaneType::E) {
            const auto tmp_a = trial_stress(0) - trial_stress(1);
            const auto tmp_b = trial_stress(1) - sigma_33;
            const auto tmp_c = sigma_33 - trial_stress(0);
            trial_mises(0) = sqrt(.5 * (tmp_a * tmp_a + tmp_b * tmp_b + tmp_c * tmp_c + 6. * trial_stress(2) * trial_stress(2)));
        }
        output.emplace_back(trial_mises);
    } else if(P == OutputType::S) {
        vec trail_sigma(4);

        trail_sigma(0) = trial_stress(0);
        trail_sigma(1) = trial_stress(1);
        trail_sigma(3) = trial_stress(2);
        trail_sigma(2) = plane_type == PlaneType::S ? 0. : sigma_33;

        output.emplace_back(trail_sigma);
    }

    return output;
}
