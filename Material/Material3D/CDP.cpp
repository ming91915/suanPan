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

double CDP::compute_d_t(const double kappa_t) const {
    const auto sqrt_phi = sqrt(1. + a_t * (a_t + 2.) * kappa_t);
    return 1. - pow((1. + a_t - sqrt_phi) / a_t, cb_t);
}

double CDP::compute_f_t(const double kappa_t) const {
    const auto sqrt_phi = sqrt(1. + a_t * (a_t + 2.) * kappa_t);
    return crack_stress * sqrt_phi * (1. + a_t - sqrt_phi) / a_t;
}

double CDP::compute_d_f_t(const double kappa_t) const {
    const auto sqrt_phi = sqrt(1. + a_t * (a_t + 2.) * kappa_t);
    return crack_stress * (.5 * a_t + 1.) * (a_t + 1 - 2. * sqrt_phi) / sqrt_phi;
}

double CDP::compute_d_c(const double kappa_c) const {
    const auto sqrt_phi = sqrt(1. + a_c * (a_c + 2.) * kappa_c);
    return 1. - pow((1. + a_c - sqrt_phi) / a_c, cb_c);
}

double CDP::compute_f_c(const double kappa_c) const {
    const auto sqrt_phi = sqrt(1. + a_c * (a_c + 2.) * kappa_c);
    return peak_stress * sqrt_phi * (1. + a_c - sqrt_phi) / a_c;
}

double CDP::compute_d_f_c(const double kappa_c) const {
    const auto sqrt_phi = sqrt(1. + a_c * (a_c + 2.) * kappa_c);
    return peak_stress * (.5 * a_c + 1.) * (a_c + 1 - 2. * sqrt_phi) / sqrt_phi;
}

double CDP::compute_bar_f_t(const double kappa_t) const {
    const auto sqrt_phi = sqrt(1. + a_t * (a_t + 2.) * kappa_t);
    return crack_stress * sqrt_phi * pow((1. + a_t - sqrt_phi) / a_t, 1. - cb_t);
}

double CDP::compute_d_bar_f_t(const double kappa_t) const {
    const auto sqrt_phi = sqrt(1. + a_t * (a_t + 2.) * kappa_t);
    auto result = crack_stress * (1. + .5 * a_t);
    result *= a_t + 1. + (cb_t - 2.) * sqrt_phi;
    result /= sqrt_phi * pow((a_t + 1. - sqrt_phi) / a_t, cb_t);
    return result;
}

double CDP::compute_bar_f_c(const double kappa_c) const {
    const auto sqrt_phi = sqrt(1. + a_c * (a_c + 2.) * kappa_c);
    return peak_stress * sqrt_phi * pow((1. + a_c - sqrt_phi) / a_c, 1. - cb_c);
}

double CDP::compute_d_bar_f_c(const double kappa_c) const {
    const auto sqrt_phi = sqrt(1. + a_c * (a_c + 2.) * kappa_c);
    auto result = peak_stress * (1. + .5 * a_c);
    result *= a_c + 1. + (cb_c - 2.) * sqrt_phi;
    result /= sqrt_phi * pow((a_c + 1. - sqrt_phi) / a_c, cb_c);
    return result;
}

double CDP::compute_beta(const double kappa_t, const double kappa_c) const {
    const auto t_factor = compute_bar_f_c(kappa_c) / compute_bar_f_t(kappa_t);
    return t_factor * (1. - alpha) - alpha - 1.;
}

double CDP::compute_r(const vec& in) const { return .5 + .5 * accu(in) / accu(abs(in)); }

vec CDP::compute_d_r(const vec& in) const {
    const auto abs_sum = accu(abs(in));

    vec out(in.n_elem);

    out.fill(abs_sum);

    out -= accu(in) * sign(in);

    out /= 2. * abs_sum * abs_sum;

    return out;
}

CDP::CDP(const unsigned T, const double E, const double V, const double AP, const double FBFC, const double GT, const double GC,const double R)
    : Material3D(T, MT_CDP, R)
    , elastic_modulus(E)
    , poissons_ratio(V)
    , shear_modulus(.5 * elastic_modulus / (1. + poissons_ratio))
    , double_shear(2. * shear_modulus)
    , bulk_modulus(elastic_modulus / (3. - 6. * poissons_ratio))
    , fbfc(FBFC)
    , alpha((fbfc - 1.) / (2. * fbfc - 1.))
    , alpha_p(AP)
    , factor_a(9. * bulk_modulus * alpha * alpha_p + sqrt(6.) * shear_modulus)
    , factor_b(3. * bulk_modulus * alpha_p)
    , peak_stress(0)
    , crack_stress(0)
    , bar_d_t(0)
    , a_t(0)
    , cb_t(log(1. - bar_d_t) / log(.5 * (1. + a_t - sqrt(1. + a_t * a_t)) / a_t))
    , bar_d_c(0)
    , a_c(0)
    , cb_c(log(1. - bar_d_c) / log(.5 + .5 / a_c))
    , g_t(GT)
    , g_c(GC)
    , tolerance(1E-10) {}

void CDP::initialize(const shared_ptr<DomainBase>&) {}

unique_ptr<Material> CDP::get_copy() { return make_unique<CDP>(*this); }

int CDP::update_trial_status(const vec& t_strain) {
    auto& trial_lambda = trial_history(0);  // consistency parameter
    auto& trial_kappa_t = trial_history(1); // tension damage parameter
    auto& trial_kappa_c = trial_history(2); // compression damage parameter

    trial_strain = t_strain;
    // elastic predictor
    const auto predictor = tensor::stress::to_tensor(initial_stiffness * (trial_strain - trial_plastic_strain)); // \sigma_{n+1}^{tr}

    vec prin_predictor;
    mat trans_mat;
    eig_sym(prin_predictor, trans_mat, predictor); // \hat{\sigma}_{n+1}^{tr}

    // get deviatoric stress of elastic predictor
    const auto dev_predictor = tensor::dev(predictor); // s_{n+1}^{tr}
    // solve for principal stress tensor
    const auto prin_dev_predictor = tensor::dev(prin_predictor); // \hat{s}_{n+1}^{tr}

    const auto I1 = tensor::invariant1(prin_predictor);
    const auto J2 = tensor::invariant2(prin_dev_predictor);

    const auto tmp_g = alpha * I1 + sqrt(3. * J2);

    auto yield_func = tmp_g + (1. - alpha) * compute_bar_f_c(trial_kappa_c);

    auto trial_beta = compute_beta(trial_kappa_t, trial_kappa_c);
    if(prin_predictor.max() > 0.) yield_func += trial_beta * prin_predictor.max();

    // elastic loading/unloading quit
    if(yield_func < tolerance) {
        trial_stress = tensor::stress::to_voigt(predictor);
        return 0;
    }

    // constant quantities
    const auto norm_predictor = norm(prin_dev_predictor);
    const vec unit_predictor = prin_dev_predictor / norm_predictor;
    const vec d_sigma_d_lambda = -double_shear * unit_predictor - factor_b;
    // some constants
    const auto tmp_a = (unit_predictor.max() + alpha_p) / g_t;
    const auto tmp_b = (unit_predictor.min() + alpha_p) / g_c;

    auto incre_lambda = 0., r_weight = 0.;

    auto counter = 0;

    vec prin_effective_stress;

    while(true) {
        if(counter++ > 10) break;

        trial_beta = compute_beta(trial_kappa_t, trial_kappa_c);

        prin_effective_stress = prin_predictor + incre_lambda * d_sigma_d_lambda; // \hat{\bar{\sigma}}_{n+1}^{tr}
        const auto prin_effective_stress_max = prin_effective_stress.max();

        const auto tmp_h = tmp_g + (1. - alpha) * compute_bar_f_c(trial_kappa_c);

        incre_lambda = prin_effective_stress_max > 0. ? (tmp_h + trial_beta * prin_predictor.max()) / (factor_a - trial_beta * d_sigma_d_lambda.max()) : tmp_h / factor_a;

        trial_plastic_strain = current_plastic_strain + (tensor::stress::to_voigt(dev_predictor) / norm_predictor + alpha_p) * incre_lambda;

        r_weight = compute_r(prin_effective_stress);
        const auto f_t = compute_f_t(trial_kappa_t);
        const auto f_c = compute_f_c(trial_kappa_c);
        const auto d_f_t = compute_d_f_t(trial_kappa_t);
        const auto d_f_c = compute_d_f_c(trial_kappa_c);

        // compute H
        vec h(2, fill::zeros);
        if(r_weight != 0.) h(0) = r_weight * tmp_a * f_t;
        if(r_weight != 1.) h(1) = (1. - r_weight) * tmp_b * f_c;

        // compute \pfrac{H}{\kappa}
        mat p_h_p_kappa(2, 2, fill::zeros);
        if(r_weight != 0.) p_h_p_kappa(0, 0) = r_weight * tmp_a * d_f_t;
        if(r_weight != 1.) p_h_p_kappa(1, 1) = (1. - r_weight) * tmp_b * d_f_c;

        // compute \pfrac{H}{\hat{\bar{\sigma}}_{n+1}}
        vec tmp_c(2);
        tmp_c(0) = tmp_a * f_t;
        tmp_c(1) = tmp_b * f_c;
        const mat p_h_p_sigma = tmp_c * compute_d_r(prin_effective_stress).t();

        // compute \pfrac{\hat{F}}{\kappa}
        vec p_f_p_kappa(2, fill::zeros);
        if(prin_effective_stress_max > 0.) {
            const auto tmp_d = prin_effective_stress_max / f_t;
            p_f_p_kappa(0) = -f_c / f_t * tmp_d * d_f_t;
            p_f_p_kappa(1) = (tmp_d - 1.) * d_f_c;
        } else
            p_f_p_kappa(1) = -d_f_c;

        // compute \pfrac{\hat{F}}{\hat{\bar{\sigma}}_{n+1}}
        vec p_f_p_sigma(3);
        p_f_p_sigma.fill(alpha);
        p_f_p_sigma += sqrt(1.5) * unit_predictor;
        if(prin_effective_stress_max > 0.) p_f_p_sigma(2) += trial_beta;
        p_f_p_sigma /= 1. - alpha;

        vec q = h * incre_lambda;
        q(0) += current_history(1) - trial_kappa_t;
        q(1) += current_history(2) - trial_kappa_c;

        // compute \dfrac{Q}{\kappa}
        mat d_q_d_kappa = incre_lambda * p_h_p_kappa;
        d_q_d_kappa(0, 0) -= 1.;
        d_q_d_kappa(1, 1) -= 1.;
        const vec tmp_e = p_f_p_kappa / dot(p_f_p_sigma, d_sigma_d_lambda);
        const vec tmp_f = incre_lambda * p_h_p_sigma * d_sigma_d_lambda + h;
        d_q_d_kappa -= tmp_f * tmp_e.t();

        const vec incre_kappa = solve(d_q_d_kappa, q);

        trial_kappa_t += incre_kappa(0);
        trial_kappa_c += incre_kappa(1);
    }

    trial_lambda += incre_lambda;
    trial_stress = tensor::stress::to_voigt(trans_mat * prin_effective_stress * trans_mat.t());
    trial_stress *= (1. - compute_d_c(trial_kappa_c)) * (1. - r_weight * compute_d_t(trial_kappa_t));

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
