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

const double CDP::sqrt_three_over_two = sqrt(1.5);

vec CDP::compute_backbone(const double f, const double a, const double cb, const double kappa) {
    vec out(6);

    const auto s_phi = sqrt(1. + a * (a + 2.) * kappa);
    const auto t_phi = (1. + .5 * a) / s_phi;
    const auto b_phi = (1. + a - s_phi) / a;
    const auto p_phi = pow(b_phi, cb);

    out(0) = 1. - p_phi;                                // d
    out(1) = f * s_phi * b_phi;                         // f
    out(2) = out(1) / p_phi;                            // \bar{f}
    out(3) = cb * t_phi * p_phi / b_phi;                // \md{d}
    out(4) = f * t_phi * (1. + a - 2. * s_phi);         // \md{f}
    out(5) = (out(4) + f * t_phi * cb * s_phi) / p_phi; // \md{\bar{f}}

    return out;
}

vec CDP::compute_d_weight(const vec& in) {
    const auto abs_sum = accu(abs(in));

    vec out(3);

    out.fill(abs_sum);

    if(abs_sum != 0.) {
        out -= accu(in) * sign(in);
        out /= 2. * abs_sum * abs_sum;
    }

    return out;
}

mat CDP::compute_jacobian(const mat& in) {
    mat out(3, 6);

    for(auto I = 0; I < 3; ++I) {
        for(auto J = 0; J < 3; ++J) out(I, J) = in(J, I) * in(J, I);
        out(I, 3) = 2. * in(0, I) * in(1, I);
        out(I, 4) = 2. * in(1, I) * in(2, I);
        out(I, 5) = 2. * in(2, I) * in(0, I);
    }

    return out;
}

double CDP::compute_weight(const vec& in) {
    const auto abs_sum = accu(abs(in));
    return abs_sum == 0. ? 0. : .5 + .5 * accu(in) / abs_sum;
}

CDP::CDP(const unsigned T, const double E, const double V, const double ST, const double SC, const double GT, const double GC, const double AT, const double AC, const double DT, const double DC, const double AP, const double BC, const double R)
    : Material3D(T, MT_CDP, R)
    , elastic_modulus(E)
    , poissons_ratio(V < .5 ? V : .2)
    , shear_modulus(elastic_modulus / (2. + 2. * poissons_ratio))
    , double_shear(2. * shear_modulus)
    , bulk_modulus(elastic_modulus / (3. - 6. * poissons_ratio))
    , f_t(ST > 0. ? ST : -ST)
    , a_t(AT)
    , cb_t(log(DT) / log(.5 * (1. + a_t - sqrt(1. + a_t * a_t)) / a_t))
    , g_t(GT)
    , f_c(SC < 0. ? SC : -SC)
    , a_c(AC)
    , cb_c(a_c == 1. ? .5 : log(DC) / log(.5 + .5 / a_c))
    , g_c(GC)
    , alpha((BC - 1.) / (2. * BC - 1.))
    , alpha_p(AP)
    , factor_a(3. * bulk_modulus * alpha_p)
    , factor_b(3. * alpha * factor_a + sqrt_three_over_two * double_shear)
    , factor_c(1. - alpha)
    , tolerance(1E-10)
    , unit_alpha_p(alpha_p * tensor::unit_tensor2()) {}

void CDP::initialize(const shared_ptr<DomainBase>&) {
    const auto lambda = shear_modulus * poissons_ratio / (.5 - poissons_ratio);

    initial_stiffness.zeros(6, 6);

    for(auto I = 0; I < 3; ++I)
        for(auto J = 0; J < 3; ++J) initial_stiffness(I, J) = lambda;

    for(auto I = 0; I < 3; ++I) initial_stiffness(I, I) += double_shear;
    for(auto I = 3; I < 6; ++I) initial_stiffness(I, I) = shear_modulus;

    inv_stiffness = inv(initial_stiffness);

    trial_stiffness = current_stiffness = initial_stiffness;

    trial_history.zeros(2);
    current_history.zeros(2);
    trial_plastic_strain.zeros(6);
    current_plastic_strain.zeros(6);
}

unique_ptr<Material> CDP::get_copy() { return make_unique<CDP>(*this); }

int CDP::update_trial_status(const vec& t_strain) {
    trial_strain = t_strain;
    trial_history = current_history;
    trial_plastic_strain = current_plastic_strain;

    const auto& kappa_t = trial_history(0);
    const auto& kappa_c = trial_history(1);

    // predictor \sigma^{tr}
    trial_stress = initial_stiffness * (trial_strain - trial_plastic_strain);

    // principal predictor \hat{\sigma}^{tr}
    vec p_predictor;
    mat trans_mat;
    if(!eig_sym(p_predictor, trans_mat, tensor::stress::to_tensor(trial_stress))) return -1;

    // deviatoric principal predictor \hat{s}^{tr}
    const vec d_predictor = p_predictor - mean(p_predictor);
    // unit deviatoric principal predictor \hat{n}
    const vec u_predictor = d_predictor / norm(d_predictor);

    const vec c_dfdsigma = sqrt_three_over_two * u_predictor + alpha;
    const vec dsigmadlambda = -double_shear * u_predictor - factor_a;
    const vec dgdsigma = u_predictor + alpha_p;

    const auto const_t = dgdsigma(2) / g_t;
    const auto const_c = dgdsigma(0) / g_c;

    const auto const_yield = alpha * tensor::invariant1(p_predictor) + sqrt(3. * tensor::invariant2(d_predictor));

    auto t_para = compute_backbone(f_t, a_t, cb_t, kappa_t);
    auto c_para = compute_backbone(f_c, a_c, cb_c, kappa_c);

    auto t_yield = const_yield + factor_c * c_para(2);

    auto beta = 0.;

    // effective stress \hat{\bar{\sigma}}
    auto e_stress = p_predictor;

    if(e_stress(2) > 0.) beta = -c_para(2) / t_para(2) * factor_c - 1. - alpha;

    const auto yield_func = t_yield + beta * e_stress(2);

    auto r_weight = compute_weight(e_stress);

    if(yield_func < tolerance) {
        const auto damage = (1. - c_para(0)) * (1. - r_weight * t_para(0));
        trial_stress *= damage;
        trial_stiffness = damage * initial_stiffness;
        return 0;
    }

    vec dfdsigma = c_dfdsigma, dfdkappa(2), h(2);
    mat phpkappa(2, 2), phpsigma, dqdkappa;

    auto p_lambda = 0.;

    auto counter = 0, flag = 0;
    while(true) {
        if(++counter > 20) {
            flag = 1;
            break;
        }

        const auto i_lambda = beta == 0. ? t_yield / factor_b : (t_yield + beta * p_predictor(2)) / (factor_b - beta * dsigmadlambda(2));

        if(abs(i_lambda - p_lambda) < tolerance) break;

        p_lambda = i_lambda;

        e_stress = p_predictor + i_lambda * dsigmadlambda;

        r_weight = compute_weight(e_stress);

        dfdsigma(2) = c_dfdsigma(2) + beta;

        if(e_stress(2) > 0.) {
            const auto t_factor = factor_c * e_stress(2) / t_para(2);
            dfdkappa(0) = c_para(2) / t_para(2) * t_factor * t_para(5);
            dfdkappa(1) = (factor_c - t_factor) * c_para(5);
        } else {
            dfdkappa(0) = 0.;
            dfdkappa(1) = factor_c * c_para(5);
        }

        h.zeros();
        if(r_weight != 0.) h(0) = r_weight * t_para(1) * const_t;
        if(r_weight != 1.) h(1) = (1. - r_weight) * c_para(1) * const_c;

        phpkappa.zeros();
        if(r_weight != 0.) phpkappa(0, 0) = r_weight * t_para(4) * const_t;
        if(r_weight != 1.) phpkappa(1, 1) = (1. - r_weight) * c_para(4) * const_c;

        phpsigma = vec{ t_para(1) * const_t, -c_para(1) * const_c } * compute_d_weight(e_stress).t();

        dqdkappa = i_lambda * phpkappa - eye(2, 2) - (i_lambda * phpsigma * dsigmadlambda + h) * dfdkappa.t() / dot(dfdsigma, dsigmadlambda);

        trial_history += solve(dqdkappa, trial_history - current_history - i_lambda * h);

        t_para = compute_backbone(f_t, a_t, cb_t, kappa_t);
        c_para = compute_backbone(f_c, a_c, cb_c, kappa_c);

        t_yield = const_yield + factor_c * c_para(2);

        beta = e_stress(2) > 0. ? -c_para(2) / t_para(2) * factor_c - 1. - alpha : 0.;
    }

    suanpan_debug("CDP model state determination loop counter: %u.\n", counter);

    if(flag == 1) {
        suanpan_error("CDP model cannot converge within 20 iterations.\n");
        return -1;
    }

    const auto d_stress = tensor::dev(trial_stress);
    const vec n = d_stress / tensor::norm(d_stress);
    trial_plastic_strain += p_lambda * (n + unit_alpha_p);
    trial_stress = tensor::stress::to_voigt(trans_mat * diagmat(e_stress) * trans_mat.t());
    trial_stress *= (1. - c_para(0)) * (1. - r_weight * t_para(0));

    const auto jacobian = compute_jacobian(trans_mat);

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
