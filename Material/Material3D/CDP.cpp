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

inline double CDP::compute_sqrt_phi(const double a, const double kappa) const { return sqrt(1. + (2. * a + a * a) * kappa); }

inline double CDP::compute_big_phi(const double a, const double kappa) const { return (1. + a - compute_sqrt_phi(a, kappa)) / a; }

inline double CDP::compute_f(const double f, const double a, const double kappa) const { return f * compute_sqrt_phi(a, kappa) * compute_big_phi(a, kappa); }

inline double CDP::compute_bar_f(const double f, const double a, const double cb, const double kappa) const { return f * compute_sqrt_phi(a, kappa) * pow(compute_big_phi(a, kappa), 1. - cb); }

inline double CDP::compute_d_d(const double a, const double cb, const double kappa) const { return .5 * cb * (a + 2.) / compute_sqrt_phi(a, kappa) * pow(compute_big_phi(a, kappa), cb - 1.); }

inline double CDP::compute_d_f(const double f, const double a, const double kappa) const {
    const auto sqrt_phi = compute_sqrt_phi(a, kappa);
    return .5 * f * (a + 2.) / sqrt_phi * (a - 2. * sqrt_phi + 1.);
}

inline double CDP::compute_d_bar_f(const double f, const double a, const double cb, const double kappa) const {
    const auto sqrt_phi = compute_sqrt_phi(a, kappa);
    return .5 * f * (a + 2.) / sqrt_phi * pow(compute_big_phi(a, kappa), -cb) * (a + 1. + (cb - 2.) * sqrt_phi);
}

double CDP::compute_r(const vec& in) const {
    const auto abs_sum = accu(abs(in));
    return abs_sum == 0. ? 0. : .5 + .5 * accu(in) / abs_sum;
}

vec CDP::compute_d_r(const vec& in) const {
    const auto abs_sum = accu(abs(in));

    vec out(in.n_elem);

    out.fill(abs_sum);

    if(abs_sum == 0.) return out;

    out -= accu(in) * sign(in);

    out /= 2. * abs_sum * abs_sum;

    return out;
}

double CDP::compute_beta(const double kappa_t, const double kappa_c) const { return compute_bar_f(peak_stress, a_c, cb_c, kappa_c) / compute_bar_f(crack_stress, a_t, cb_t, kappa_t) * (alpha - 1.) - alpha - 1.; }

CDP::CDP(const unsigned T, const double E, const double V, const double AP, const double FBFC, const double GT, const double GC, const double R)
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
    , a_t(2.)
    , cb_t(log(1. - bar_d_t) / log(.5 * (1. + a_t - sqrt(1. + a_t * a_t)) / a_t))
    , bar_d_c(0)
    , a_c(2.)
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
    // elastic predictor \sigma_{n+1}^{tr}
    const auto predictor = tensor::stress::to_tensor(initial_stiffness * (trial_strain - trial_plastic_strain));

    // principal elastic predictor \hat{\sigma}_{n+1}^{tr}
    vec prin_predictor;
    // transformation P
    mat trans_mat;
    eig_sym(prin_predictor, trans_mat, predictor);

    // deviatoric stress of elastic predictor s_{n+1}^{tr}
    const auto dev_predictor = tensor::dev(predictor);
    // principal stress deviator \hat{s}_{n+1}^{tr}
    const auto prin_dev_predictor = tensor::dev(prin_predictor);

    const auto I1 = tensor::invariant1(prin_predictor);
    const auto J2 = tensor::invariant2(prin_dev_predictor);

    // constant part of yield function
    const auto const_yield = alpha * I1 + sqrt(3. * J2);

    auto yield_func = const_yield + (1. - alpha) * compute_bar_f(peak_stress, a_c, cb_c, trial_kappa_c);

    // \hat{\sigma}_1^{tr}
    const auto predictor_max = prin_predictor.max();

    if(predictor_max > 0.) yield_func += compute_beta(trial_kappa_t, trial_kappa_c) * predictor_max;

    // elastic loading/unloading quit
    if(yield_func < tolerance) {
        trial_stress = tensor::stress::to_voigt(predictor);
        return 0;
    }

    // constant quantities
    const auto norm_dev_predictor = norm(prin_dev_predictor);
    const vec unit_dev_predictor = prin_dev_predictor / norm_dev_predictor;
    const vec d_sigma_d_lambda = -double_shear * unit_dev_predictor - factor_b;
    const auto tmp_k = d_sigma_d_lambda.max();
    // some constants
    const auto tmp_a = (unit_dev_predictor.max() + alpha_p) / g_t;
    const auto tmp_b = (unit_dev_predictor.min() + alpha_p) / g_c;

    auto incre_lambda = 0., previous_lambda = 0., r_weight = 0.;

    auto counter = 0;

    vec prin_eff_stress;

    while(true) {
        if(counter++ > 10) break;

        const auto beta = compute_beta(trial_kappa_t, trial_kappa_c);

        // principal effective stress \hat{\bar{\sigma}}_{n+1}^{tr}
        prin_eff_stress = prin_predictor + incre_lambda * d_sigma_d_lambda;

        // \hat{\bar{\sigma}}_1^{tr}
        const auto sigma_max = prin_eff_stress.max();

        const auto tmp_h = const_yield + (1. - alpha) * compute_bar_f(peak_stress, a_c, cb_c, trial_kappa_c);

        incre_lambda = sigma_max > 0. ? (tmp_h + beta * predictor_max) / (factor_a + beta * tmp_k) : tmp_h / factor_a;

        r_weight = compute_r(prin_eff_stress);

        // \pfrac{F}{\hat{\bar{\sigma}}_{n+1}}
        vec p_f_p_sigma = alpha + sqrt(1.5) * unit_dev_predictor;
        if(sigma_max > 0.) p_f_p_sigma(2) += beta;

        while(true) {
            const auto f_t = compute_f(crack_stress, a_t, trial_kappa_t);
            const auto f_c = compute_f(peak_stress, a_c, trial_kappa_c);
            const auto d_f_t = compute_d_f(crack_stress, a_t, trial_kappa_t);
            const auto d_f_c = compute_d_f(peak_stress, a_c, trial_kappa_c);

            // damage evolution H
            vec h(2, fill::zeros);
            if(r_weight != 0.) h(0) = r_weight * tmp_a * f_t;
            if(r_weight != 1.) h(1) = (r_weight - 1.) * tmp_b * f_c;

            // \pfrac{H}{\kappa}
            mat p_h_p_kappa(2, 2, fill::zeros);
            if(r_weight != 0.) p_h_p_kappa(0, 0) = r_weight * tmp_a * d_f_t;
            if(r_weight != 1.) p_h_p_kappa(1, 1) = (1. - r_weight) * tmp_b * d_f_c;

            // \pfrac{H}{\hat{\bar{\sigma}}_{n+1}}
            vec p_h_p_r(2);
            p_h_p_r(0) = tmp_a * f_t;
            p_h_p_r(1) = tmp_b * f_c;
            const mat p_h_p_sigma = p_h_p_r * compute_d_r(prin_eff_stress).t();

            // \pfrac{F}{\kappa}
            vec p_f_p_kappa(2, fill::zeros);
            if(sigma_max > 0.) {
                const auto tmp_d = sigma_max / f_t;
                p_f_p_kappa(0) = f_c / f_t * tmp_d * d_f_t;
                p_f_p_kappa(1) = (1. - tmp_d) * d_f_c;
            } else
                p_f_p_kappa(1) = d_f_c;
            p_f_p_kappa *= 1. - alpha;

            vec q = h * incre_lambda;
            q(0) += current_history(1) - trial_kappa_t;
            q(1) += current_history(2) - trial_kappa_c;

            // \dfrac{Q}{\kappa}
            mat d_q_d_kappa = incre_lambda * p_h_p_kappa;
            d_q_d_kappa(0, 0) -= 1.;
            d_q_d_kappa(1, 1) -= 1.;
            const vec tmp_e = p_f_p_kappa / dot(p_f_p_sigma, d_sigma_d_lambda);
            const vec tmp_f = incre_lambda * p_h_p_sigma * d_sigma_d_lambda + h;
            d_q_d_kappa -= tmp_f * tmp_e.t();

            const vec incre_kappa = solve(d_q_d_kappa, q);

            trial_kappa_t += incre_kappa(0);
            trial_kappa_c += incre_kappa(1);

            if(norm(incre_kappa) < tolerance) break;
        }

        if(abs(incre_lambda - previous_lambda) < tolerance) break;

        previous_lambda = incre_lambda;
    }

    trial_lambda += incre_lambda;
    trial_plastic_strain += (tensor::stress::to_voigt(dev_predictor) / norm_dev_predictor + alpha_p) * incre_lambda;
    trial_stress = tensor::stress::to_voigt(trans_mat * prin_eff_stress * trans_mat.t());
    trial_stress *= pow(compute_big_phi(a_c, trial_kappa_c), cb_c) * (1. - r_weight * (1. - pow(compute_big_phi(a_t, trial_kappa_t), cb_t)));

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
