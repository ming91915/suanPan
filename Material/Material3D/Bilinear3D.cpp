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

#include "Bilinear3D.h"
#include <Toolbox/tensorToolbox.h>

const vec Bilinear3D::norm_weight = vec(std::initializer_list<double>({ 1, 1, 1, 2, 2, 2 }));
const double Bilinear3D::root_two_third = sqrt(2. / 3.);
const mat Bilinear3D::unit_dev_tensor = unitDevTensor4();

Bilinear3D::Bilinear3D(const unsigned& T, const double& E, const double& V, const double& Y, const double& H, const double& B, const double& R)
    : Material3D(T, MT_BILINEAR3D, R)
    , elastic_modulus(E)
    , poissons_ratio(V)
    , yield_stress(Y)
    , hardening_ratio(H)
    , beta(B)
    , tolerance(1E-14 * yield_stress)
    , shear_modulus(elastic_modulus / (1. + poissons_ratio) / 2.)
    , double_shear(2. * shear_modulus)
    , square_double_shear(double_shear * double_shear)
    , plastic_modulus(elastic_modulus * hardening_ratio / (1. - hardening_ratio))
    , factor(2. / 3. * plastic_modulus) {}

void Bilinear3D::initialize(const shared_ptr<DomainBase>&) {
    const auto lambda = shear_modulus * poissons_ratio / (.5 - poissons_ratio);

    initial_stiffness.zeros(6, 6);

    for(auto I = 0; I < 3; ++I)
        for(auto J = 0; J < 3; ++J) initial_stiffness(I, J) = lambda;

    for(auto I = 0; I < 3; ++I) initial_stiffness(I, I) += double_shear;

    for(auto I = 3; I < 6; ++I) initial_stiffness(I, I) = shear_modulus;

    current_stiffness = initial_stiffness;
    trial_stiffness = initial_stiffness;

    current_back_stress.zeros(6);
    trial_back_stress.zeros(6);

    current_plastic_strain = 0.;
    trial_plastic_strain = 0.;
}

unique_ptr<Material> Bilinear3D::get_copy() { return make_unique<Bilinear3D>(*this); }

int Bilinear3D::update_incre_status(const vec& i_strain) { return update_trial_status(current_strain + i_strain); }

int Bilinear3D::update_trial_status(const vec& t_strain) {
    trial_strain = t_strain;
    incre_strain = trial_strain - current_strain;
    trial_stress = current_stress + initial_stiffness * incre_strain;
    trial_back_stress = current_back_stress;
    trial_plastic_strain = current_plastic_strain;
    trial_stiffness = initial_stiffness;

    const vec shifted_stress = dev(trial_stress) - current_back_stress;

    const auto norm_shifted_stress = sqrt(dot(norm_weight, square(shifted_stress)));

    const auto yield_func = norm_shifted_stress - root_two_third * (yield_stress + (1. - beta) * plastic_modulus * current_plastic_strain);

    if(yield_func > tolerance) {
        const auto tmp_a = double_shear + factor;
        const auto gamma = yield_func / tmp_a;
        const vec unit_norm = shifted_stress / norm_shifted_stress;
        const vec tmp_b = gamma * unit_norm;
        const auto tmp_c = square_double_shear / tmp_a;
        const auto tmp_d = square_double_shear * gamma / norm_shifted_stress;

        trial_stress -= double_shear * tmp_b;
        trial_back_stress += factor * beta * tmp_b;
        trial_plastic_strain += root_two_third * gamma;

        trial_stiffness -= (tmp_c - tmp_d) * unit_norm * unit_norm.t();
        trial_stiffness -= tmp_d * unit_dev_tensor;
    }
    return 0;
}

int Bilinear3D::clear_status() {
    current_strain.zeros();
    current_stress.zeros();
    current_stiffness = initial_stiffness;
    current_back_stress.zeros();
    current_plastic_strain = 0.;
    return reset_status();
}

int Bilinear3D::commit_status() {
    current_strain = trial_strain;
    current_stress = trial_stress;
    current_stiffness = trial_stiffness;
    current_back_stress = trial_back_stress;
    current_plastic_strain = trial_plastic_strain;
    return 0;
}

int Bilinear3D::reset_status() {
    trial_strain = current_strain;
    trial_stress = current_stress;
    trial_stiffness = current_stiffness;
    trial_back_stress = current_back_stress;
    trial_plastic_strain = current_plastic_strain;
    return 0;
}
