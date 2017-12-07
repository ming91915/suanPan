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

const vec CDP::norm_weight(std::initializer_list<double>{ 1, 1, 1, 2, 2, 2 });

vec CDP::compute_response(const double f, const double a, const double cb, const double kappa) const {
    vec out(6);

    const auto s_phi = sqrt(1. + a * (a + 2.) * kappa);
    const auto b_phi = (1. + a - s_phi) / a;
    const auto p_phi = pow(b_phi, cb);
    const auto t_phi = (.5 * a + 1.) / s_phi;

    out(0) = f * s_phi * b_phi;                         // f
    out(1) = out(0) / p_phi;                            // \bar{f}
    out(2) = 1. - p_phi;                                // d
    out(3) = f * t_phi * (1. + a - 2. * s_phi);         // \mathrm{d}f
    out(4) = (out(3) + f * t_phi * cb * s_phi) / p_phi; // \mathrm{d}\bar{f}
    out(5) = t_phi * cb * p_phi / b_phi;                // \mathrm{d}d

    return out;
}

double CDP::compute_r(const vec& p_stress) const {
    const auto abs_sum = accu(abs(p_stress));
    return abs_sum == 0. ? 0. : .5 + .5 * accu(p_stress) / abs_sum;
}

vec CDP::compute_d_r(const vec& p_stress) const {
    const auto abs_sum = accu(abs(p_stress));

    vec out(3);
    out.fill(abs_sum);

    if(abs_sum != 0.) {
        out -= accu(p_stress) * sign(p_stress);
        out /= 2. * abs_sum * abs_sum;
    }

    return out;
}

CDP::CDP(const unsigned T, const double E, const double V, const double ST, const double SC, const double GT, const double GC, const double AP, const double FBFC, const double R)
    : Material3D(T, MT_CDP, R)
    , elastic_modulus(E)
    , poissons_ratio(V)
    , shear_modulus(.5 * elastic_modulus / (1. + poissons_ratio))
    , double_shear(2. * shear_modulus)
    , bulk_modulus(elastic_modulus / (3. - 6. * poissons_ratio))
    , alpha((FBFC - 1.) / (2. * FBFC - 1.))
    , alpha_p(AP)
    , f_t(ST > 0. ? ST : -ST)
    , f_c(SC < 0. ? SC : -SC)
    , g_t(GT)
    , g_c(GC)
    , a_t(.5)
    , a_c(2.)
    , cb_t(.5)
    , cb_c(2.)
    , factor_a(3. * bulk_modulus * alpha_p)
    , factor_b(3. * alpha * factor_a + sqrt(6.) * shear_modulus)
    , factor_c(1. - alpha)
    , tolerance(1E-10) {}

void CDP::initialize(const shared_ptr<DomainBase>&) {
    const auto lambda = shear_modulus * poissons_ratio / (.5 - poissons_ratio);

    initial_stiffness.zeros(6, 6);

    for(auto I = 0; I < 3; ++I)
        for(auto J = 0; J < 3; ++J) initial_stiffness(I, J) = lambda;

    for(auto I = 0; I < 3; ++I) initial_stiffness(I, I) += double_shear;
    for(auto I = 3; I < 6; ++I) initial_stiffness(I, I) = shear_modulus;

    trial_stiffness = current_stiffness = initial_stiffness;

    trial_history.zeros(2);
    current_history.zeros(2);
    trial_plastic_strain.zeros(6);
    current_plastic_strain.zeros(6);
}

unique_ptr<Material> CDP::get_copy() { return make_unique<CDP>(*this); }

int CDP::update_trial_status(const vec& t_strain) {
    trial_history = current_history;
    trial_plastic_strain = current_plastic_strain;

    const auto& trial_kappa_t = trial_history(0);
    const auto& trial_kappa_c = trial_history(1);

    trial_strain = t_strain;

    // predictor
    trial_stress = initial_stiffness * (trial_strain - trial_plastic_strain);

    // predictor tensor \sigma_{tr}
    const auto t_stress = tensor::stress::to_tensor(trial_stress);

    // principal predictor tensor \hat{\sigma}_{tr}
    vec p_stress;
    mat trans_mat;
    if(!eig_sym(p_stress, trans_mat, t_stress)) return -1;

    // deviatoric principal predictor \hat{s}_tr
    const vec d_stress = p_stress - mean(p_stress);

    const auto const_yield = alpha * tensor::invariant1(p_stress) + sqrt(3. * tensor::invariant2(d_stress));

    auto t_para = compute_response(f_t, a_t, cb_t, trial_kappa_t);
    auto c_para = compute_response(f_c, a_c, cb_c, trial_kappa_c);

    auto yield_func = const_yield + factor_c * c_para(1);

    auto beta = 0.;

    // effective stress \hat{\bar{\sigma}}
    auto e_stress = p_stress;

    // \hat{\bar{\sigma}}_1
    const auto& e_stress_max = e_stress(2);

    if(e_stress_max > 0.) beta = -c_para(1) / t_para(1) * factor_c - 1. - alpha;

    yield_func += beta;

    auto r_weight = compute_r(e_stress);
    if(yield_func < tolerance) {
        trial_stress *= (1. - c_para(2)) * (1. - r_weight * t_para(2));
        return 0;
    }

    const vec u_stress = d_stress / norm(d_stress);
    const vec t_pfpsigma = sqrt(1.5) * u_stress + alpha;

    const auto const_max = (u_stress(2) + alpha_p) / g_t;
    const auto const_min = (u_stress(0) + alpha_p) / g_c;

    const vec dsigmadlambda = -double_shear * u_stress - factor_a;

    auto p_lambda = 0.;

    auto counter = 0, flag = 0;
    while(true) {
        const auto i_lambda = beta == 0. ? (const_yield + factor_c * c_para(1)) / factor_b : (const_yield + factor_c * c_para(1) + beta * e_stress(2)) / (factor_b - beta * dsigmadlambda(2));

        if(abs(i_lambda - p_lambda) < tolerance) break;

        p_lambda = i_lambda;

        e_stress = p_stress + i_lambda * dsigmadlambda;

        beta = e_stress(2) > 0. ? -c_para(1) / t_para(1) * factor_c - 1. - alpha : 0.;

        r_weight = compute_r(e_stress);
        const auto d_r = compute_d_r(e_stress);

        auto inner_counter = 0;
        while(true) {
            // H
            vec h(2, fill::zeros);
            if(r_weight != 0.) h(0) = r_weight * const_max * t_para(0);
            if(r_weight != 1.) h(1) = (1. - r_weight) * const_min * c_para(0);

            // \pfrac{H}{\kappa}
            mat phpkappa(2, 2, fill::zeros);
            if(r_weight != 0.) phpkappa(0, 0) = r_weight * const_max * t_para(3);
            if(r_weight != 1.) phpkappa(1, 1) = (1. - r_weight) * const_min * c_para(3);

            // \pfrac{H}{\sigma}
            const mat phpsigma = vec{ const_max * t_para(0), -const_min * c_para(0) } * d_r.t();

            // \pfrac{F}{\kappa}
            rowvec pfpkappa(2);
            if(e_stress(2) > 0.) {
                const auto t_factor = e_stress(2) / t_para(1);
                pfpkappa(0) = c_para(1) / t_para(1) * t_factor * t_para(4);
                pfpkappa(1) = (1. - t_factor) * c_para(4);
            } else {
                pfpkappa(0) = 0.;
                pfpkappa(1) = c_para(4);
            }
            pfpkappa *= factor_c;

            // \pfrac{F}{\sigma}
            auto pfpsigma = t_pfpsigma;
            pfpsigma(2) += beta;

            // dqdkappa
            const mat dqdkappa = i_lambda * phpkappa - eye(2, 2) - (h + i_lambda * phpsigma * dsigmadlambda) * pfpkappa / dot(pfpsigma, dsigmadlambda);

            const vec q = -i_lambda * h + trial_history - current_history;

            const vec incre_kappa = solve(dqdkappa, q);

            if(norm(incre_kappa) < tolerance) break;

            trial_history += incre_kappa;

            t_para = compute_response(f_t, a_t, cb_t, trial_kappa_t);
            c_para = compute_response(f_c, a_c, cb_c, trial_kappa_c);

            beta = e_stress(2) > 0. ? -c_para(1) / t_para(1) * factor_c - 1. - alpha : 0.;

            if(++inner_counter > 200) {
                flag = 1;
                break;
            }
        }

        suanpan_info("inner counter: %u\n", inner_counter);

        if(++counter > 20) {
            suanpan_error("cnnot converge in ten iterations.\n");
            flag = 1;
            break;
        }
    }

    if(flag == 1) return -1;

    auto d_tr_stress = tensor::dev(trial_stress);
    d_tr_stress /= dot(norm_weight, square(d_tr_stress));
    trial_plastic_strain += p_lambda * (d_tr_stress + alpha_p);
    trial_stress = tensor::stress::to_voigt(trans_mat * diagmat(e_stress) * trans_mat.t());
    trial_stress *= (1. - c_para(2)) * (1. - r_weight * t_para(2));

    return 0;
}

int CDP::clear_status() {
    current_strain.zeros();
    current_stress.zeros();
    current_history.zeros();
    current_plastic_strain.zeros();
    current_stiffness = initial_stiffness;
    return reset_status();
}

int CDP::commit_status() {
    current_strain = trial_strain;
    current_stress = trial_stress;
    current_history = trial_history;
    current_plastic_strain = trial_plastic_strain;
    current_stiffness = trial_stiffness;
    return 0;
}

int CDP::reset_status() {
    trial_strain = current_strain;
    trial_stress = current_stress;
    trial_history = current_history;
    trial_plastic_strain = current_plastic_strain;
    trial_stiffness = current_stiffness;
    return 0;
}
