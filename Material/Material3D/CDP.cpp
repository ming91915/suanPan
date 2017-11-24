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

#include "CDP.h"
#include <Toolbox/tensorToolbox.h>

double CDP::compute_tension_backbone(const double kappa_t) const {
    const auto sqrt_phi = sqrt(1. + a_t * (a_t + 2.) * kappa_t);
    return crack_stress * sqrt_phi * (1. + a_t - sqrt_phi) / a_t;
}

double CDP::compute_compression_backbone(const double kappa_c) const {
    const auto sqrt_phi = sqrt(1. + a_c * (a_c + 2.) * kappa_c);
    return peak_stress * sqrt_phi * (1. + a_c - sqrt_phi) / a_c;
}

double CDP::compute_effective_tension_backbone(const double kappa_t) const {
    const auto sqrt_phi = sqrt(1. + a_t * (a_t + 2.) * kappa_t);
    return crack_stress * sqrt_phi * pow((1. + a_t - sqrt_phi) / a_t, 1. - cb_t);
}

double CDP::compute_effective_compression_backbone(const double kappa_c) const {
    const auto sqrt_phi = sqrt(1. + a_c * (a_c + 2.) * kappa_c);
    return peak_stress * sqrt_phi * pow((1. + a_c - sqrt_phi) / a_c, 1. - cb_c);
}

double CDP::compute_beta(const double kappa_t, const double kappa_c) const {
    const auto t_factor = compute_effective_compression_backbone(kappa_c) / compute_effective_tension_backbone(kappa_t);
    return t_factor * (alpha - 1.) - alpha - 1.;
}

double CDP::compute_ramp_weight(const vec& in) const { return .5 + .5 * accu(in) / accu(abs(in)); }

vec CDP::compute_ramp_weight_prime(const vec& in) const {
    const auto abs_sum = accu(abs(in));

    vec out(in.n_elem);

    out.fill(abs_sum);

    out -= accu(in) * sign(in);

    out /= 2. * abs_sum * abs_sum;

    return out;
}

CDP::CDP(const unsigned T, const double E, const double V, const double AP, const double H, const double B, const double R)
    : Material3D(T, MT_CDP, R)
    , elastic_modulus(E)
    , poissons_ratio(V)
    , shear_modulus(.5 * elastic_modulus / (1. + poissons_ratio))
    , bulk_modulus(elastic_modulus / (3. - 6. * poissons_ratio))
    , fbfc(0)
    , alpha(1. / (2. - 1. / fbfc))
    , alpha_p(AP)
    , factor_a(9. * bulk_modulus * alpha * alpha_p + sqrt(6.) * shear_modulus)
    , factor_b(0)
    , peak_stress(0)
    , peak_strain(0)
    , crack_stress(0)
    , crack_strain(0)
    , bar_d_t(0)
    , a_t(0)
    , cb_t(log(1. - bar_d_t) / log(.5 * (1. + a_t - sqrt(1. + a_t * a_t)) / a_t))
    , bar_d_c(0)
    , a_c(0)
    , cb_c(log(1. - bar_d_c) / log(.5 + .5 / a_c))
    , g_t(0)
    , g_c(0) {}

void CDP::initialize(const shared_ptr<DomainBase>&) {}

unique_ptr<Material> CDP::get_copy() { return make_unique<CDP>(*this); }

int CDP::update_trial_status(const vec& t_strain) {
    trial_history = current_history;

    auto& trial_lambda = trial_history(0);  // consistency parameter
    auto& trial_kappa_t = trial_history(1); // tension damage parameter
    auto& trial_kappa_c = trial_history(2); // compression damage parameter

    auto incre_lambda = 0.;

    trial_strain = t_strain;
    trial_stress = initial_stiffness * trial_strain;

    // convert voigt to tensor 3-by-3 matrix
    const auto stress_tensor = tensor::stress::to_tensor(trial_stress);
    // get deviatoric stress
    const auto dev_stress_tensor = tensor::dev(stress_tensor);

    // solve for principal stress tensor
    vec principal_stress;
    mat trans_mat;
    eig_sym(principal_stress, trans_mat, stress_tensor);

    // get deviatoric principal stress
    const vec dev_principal_stress = principal_stress - mean(principal_stress);

    // set anchors pay attention to the order
    const auto& p_stress_1 = dev_principal_stress(2);
    const auto& p_stress_2 = dev_principal_stress(1);
    const auto& p_stress_3 = dev_principal_stress(0);

    const auto I1 = tensor::invariant1(principal_stress);
    const auto J2 = tensor::invariant2(dev_principal_stress);

    return 0;
}

int CDP::clear_status() {
    current_strain.zeros();
    current_stress.zeros();
    current_stiffness = initial_stiffness;

    return reset_status();
}

int CDP::commit_status() {
    current_strain = trial_strain;
    current_stress = trial_stress;
    current_stiffness = trial_stiffness;

    return 0;
}

int CDP::reset_status() {
    trial_strain = current_strain;
    trial_stress = current_stress;
    trial_stiffness = current_stiffness;

    return 0;
}
