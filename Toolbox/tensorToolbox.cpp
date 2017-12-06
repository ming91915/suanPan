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

#include "tensorToolbox.h"

vec tensor::unit_tensor2() {
    vec T(6, fill::zeros);
    T(0) = T(1) = T(2) = 1.;
    return T;
}

mat tensor::unit_deviatoric_tensor4() {
    mat T = zeros(6, 6);

    for(auto I = 3; I < 6; ++I) T(I, I) = .5;

    for(auto I = 0; I < 3; ++I)
        for(auto J = 0; J < 3; ++J) T(I, J) = I == J ? 2. / 3. : -1. / 3.;

    return T;
}

mat tensor::unit_symmetric_tensor4() {
    mat T = zeros(6, 6);

    for(auto I = 0; I < 3; ++I) T(I, I) = 1.;
    for(auto I = 3; I < 6; ++I) T(I, I) = .5;

    return T;
}

double tensor::invariant1(const vec& S) { return accu(S); }

double tensor::invariant2(const vec& S) { return -S(0) * S(1) - S(1) * S(2) - S(2) * S(0); }

double tensor::invariant3(const vec& S) { return prod(S); }

double tensor::invariant1(const mat& S) { return accu(S.diag()); }

double tensor::invariant2(const mat& S) { return .5 * (pow(arma::trace(S), 2.) - arma::trace(S * S)); }

double tensor::invariant3(const mat& S) { return det(S); }

double tensor::trace(const vec& S) {
    if(S.n_elem != 6) throw;

    return S(0) + S(1) + S(2);
}

double tensor::mean(const vec& S) {
    if(S.n_elem != 6) throw;
    return trace(S) / 3.;
}

vec tensor::dev(const vec& S) {
    if(S.n_elem != 6) throw;

    auto D = S;
    const auto M = mean(D);
    for(auto I = 0; I < 3; ++I) D(I) -= M;

    return D;
}

mat tensor::dev(const mat& in) {
    auto out = in;
    out.diag() -= mean(out.diag());
    return out;
}

mat tensor::strain::to_tensor(const vec& in_strain) {
    mat out_strain(3, 3);

    out_strain(0, 0) = in_strain(0);
    out_strain(1, 1) = in_strain(1);
    out_strain(2, 2) = in_strain(2);
    out_strain(0, 1) = out_strain(1, 0) = .5 * in_strain(3);
    out_strain(1, 2) = out_strain(2, 1) = .5 * in_strain(4);
    out_strain(2, 0) = out_strain(0, 2) = .5 * in_strain(5);

    return out_strain;
}

vec tensor::strain::to_voigt(const mat& in_strain) {
    vec out_strain(6);

    out_strain(0) = in_strain(0, 0);
    out_strain(1) = in_strain(1, 1);
    out_strain(2) = in_strain(2, 2);
    out_strain(3) = 2. * in_strain(0, 1);
    out_strain(4) = 2. * in_strain(1, 2);
    out_strain(5) = 2. * in_strain(2, 0);

    return out_strain;
}

mat tensor::stress::to_tensor(const vec& in_stress) {
    mat out_stress(3, 3);

    out_stress(0, 0) = in_stress(0);
    out_stress(1, 1) = in_stress(1);
    out_stress(2, 2) = in_stress(2);
    out_stress(0, 1) = out_stress(1, 0) = in_stress(3);
    out_stress(1, 2) = out_stress(2, 1) = in_stress(4);
    out_stress(2, 0) = out_stress(0, 2) = in_stress(5);

    return out_stress;
}

vec tensor::stress::to_voigt(const mat& in_stress) {
    vec out_stress(6);

    out_stress(0) = in_stress(0, 0);
    out_stress(1) = in_stress(1, 1);
    out_stress(2) = in_stress(2, 2);
    out_stress(3) = in_stress(0, 1);
    out_stress(4) = in_stress(1, 2);
    out_stress(5) = in_stress(2, 0);

    return out_stress;
}

double transform::atan2(const vec& direction_cosine) { return std::atan2(direction_cosine(1), direction_cosine(0)); }

double transform::strain::angle(const vec& strain) { return .5 * std::atan2(strain(2), strain(0) - strain(1)); }

mat transform::strain::trans(const double angle) {
    const auto sin_angle = sin(angle);
    const auto cos_angle = cos(angle);
    const auto sin_sin = sin_angle * sin_angle;
    const auto cos_cos = cos_angle * cos_angle;
    const auto sin_cos = sin_angle * cos_angle;

    mat trans(3, 3);
    trans(0, 0) = trans(1, 1) = cos_cos;
    trans(0, 1) = trans(1, 0) = sin_sin;
    trans(1, 2) = -(trans(0, 2) = sin_cos);
    trans(2, 0) = -(trans(2, 1) = 2. * sin_cos);
    trans(2, 2) = cos_cos - sin_sin;

    return trans;
}

vec transform::strain::principal(const vec& strain) {
    const auto tmp_a = .5 * (strain(0) + strain(1));
    const auto tmp_b = .5 * sqrt(pow(strain(0) - strain(1), 2.) + pow(strain(2), 2.));

    vec p_strain(3);
    p_strain(0) = tmp_a + tmp_b;
    p_strain(1) = tmp_a - tmp_b;
    p_strain(2) = 0.;

    return p_strain;
}

vec transform::strain::rotate(const vec& strain, const double theta) { return trans(theta) * strain; }

double transform::stress::angle(const vec& stress) { return .5 * std::atan2(2. * stress(2), stress(0) - stress(1)); }

mat transform::stress::trans(const double angle) {
    const auto sin_angle = sin(angle);
    const auto cos_angle = cos(angle);
    const auto sin_sin = sin_angle * sin_angle;
    const auto cos_cos = cos_angle * cos_angle;
    const auto sin_cos = sin_angle * cos_angle;

    mat trans(3, 3);
    trans(0, 0) = trans(1, 1) = cos_cos;
    trans(0, 1) = trans(1, 0) = sin_sin;
    trans(1, 2) = -(trans(0, 2) = 2. * sin_cos);
    trans(2, 0) = -(trans(2, 1) = sin_cos);
    trans(2, 2) = cos_cos - sin_sin;

    return trans;
}

vec transform::stress::principal(const vec& stress) {
    const auto tmp_a = .5 * (stress(0) + stress(1));
    const auto tmp_b = .5 * sqrt(pow(stress(0) - stress(1), 2.) + pow(2. * stress(2), 2.));

    vec p_stress(3);
    p_stress(0) = tmp_a + tmp_b;
    p_stress(1) = tmp_a - tmp_b;
    p_stress(2) = 0.;

    return p_stress;
}

vec transform::stress::rotate(const vec& stress, const double theta) { return trans(theta) * stress; }

mat transform::beam::global_to_local(const double, const double, const double, const double) {
    mat trans_mat(5, 12, fill::zeros);
    return trans_mat;
}

mat transform::beam::global_to_local(const double cosine, const double sine, const double length) {
    mat trans_mat(3, 6, fill::zeros);

    trans_mat(0, 0) = -(trans_mat(0, 3) = cosine);
    trans_mat(0, 1) = -(trans_mat(0, 4) = sine);
    trans_mat(1, 0) = trans_mat(2, 0) = -(trans_mat(1, 3) = trans_mat(2, 3) = sine / length);
    trans_mat(1, 4) = trans_mat(2, 4) = -(trans_mat(1, 1) = trans_mat(2, 1) = cosine / length);
    trans_mat(1, 2) = trans_mat(2, 5) = 1.;

    return trans_mat;
}

/**
 * \brief a subroutine to get transformation matrix between global and local coordinate system
 * \param direction_cosine the direction cosine consists of either two or three elements
 * \param length the length of the member
 * \return a matrix that transforms global quantities to local quantities with rigid body motions removed
 */
mat transform::beam::global_to_local(const vec& direction_cosine, const double length) {
    if(direction_cosine.n_elem == 2) return global_to_local(direction_cosine(0), direction_cosine(1), length);
    if(direction_cosine.n_elem == 3) return global_to_local(direction_cosine(0), direction_cosine(1), direction_cosine(2), length);
    throw logic_error("direction cosine must contains two or three elements.\n");
}
