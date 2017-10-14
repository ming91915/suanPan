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

mat unitDevTensor4() {
    mat T = zeros(6, 6);

    for(auto I = 3; I < 6; ++I) T(I, I) = .5;

    for(auto I = 0; I < 3; ++I)
        for(auto J = 0; J < 3; ++J) T(I, J) = I == J ? 2. / 3. : -1. / 3.;

    return T;
}

double tr(const vec& S) {
    auto T = 0.;
    for(auto I = 0; I < 3; ++I) T += S(I);
    return T;
}

double mean(const vec& S) { return tr(S) / 3.; }

vec dev(const vec& S) {
    auto D = S;
    const auto M = mean(S);
    for(auto I = 0; I < 3; ++I) D(I) -= M;
    return D;
}

mat transform::form_trans(const double angle) {
    const auto sin_angle = sin(angle);
    const auto cos_angle = cos(angle);
    const auto sin_sin = sin_angle * sin_angle;
    const auto cos_cos = cos_angle * cos_angle;
    const auto sin_cos = sin_angle * cos_angle;

    mat trans(3, 3);
    trans(0, 0) = cos_cos;
    trans(0, 1) = sin_sin;
    trans(0, 2) = 2. * sin_cos;
    trans(1, 0) = sin_sin;
    trans(1, 1) = cos_cos;
    trans(1, 2) = -2. * sin_cos;
    trans(2, 0) = -sin_cos;
    trans(2, 1) = sin_cos;
    trans(2, 2) = cos_cos - sin_sin;

    return trans;
}

vec transform::nominal_to_principal_strain(const vec& strain) {
    const auto tmp_a = (strain(0) + strain(1)) / 2.;
    const auto tmp_b = sqrt(pow(strain(0) - strain(1), 2.) + pow(strain(2), 2.)) / 2.;

    vec p_strain(3);
    p_strain(0) = tmp_a + tmp_b;
    p_strain(1) = tmp_a - tmp_b;
    p_strain(2) = 0.;

    return p_strain;
}

vec transform::nominal_to_principal_stress(const vec& stress) {
    const auto tmp_a = (stress(0) + stress(1)) / 2.;
    const auto tmp_b = sqrt(pow(stress(0) - stress(1), 2.) + pow(2. * stress(2), 2.)) / 2.;

    vec p_stress(3);
    p_stress(0) = tmp_a + tmp_b;
    p_stress(1) = tmp_a - tmp_b;
    p_stress(2) = 0.;

    return p_stress;
}

mat transform::nominal_to_principal_strain(vec& strain, double& theta) {
    theta = atan(strain(2) / (strain(0) - strain(1))) / 2.;

    strain = nominal_to_principal_strain(strain);

    return form_trans(theta);
}

mat transform::nominal_to_principal_stress(vec& stress, double& theta) {
    theta = atan(2. * stress(2) / (stress(0) - stress(1))) / 2.;

    stress = nominal_to_principal_stress(stress);

    return form_trans(theta);
}

/**
 * \brief
 * \param strain strain in Voigt form (3)
 * \param trans tranformation matrix
 * \return new rotated strain
 */
vec transform::rotate_strain(const vec& strain, const mat& trans) {
    auto n_strain = strain;
    n_strain(2) /= 2.;
    n_strain = trans * n_strain;
    n_strain(2) *= 2.;
    return n_strain;
}

/**
 * \brief
 * \param stress stress in Voigt form (3)
 * \param trans tranformation matrix
 * \return new rotated stress
 */
vec transform::rotate_stress(const vec& stress, const mat& trans) { return trans * stress; }

/**
 * \brief
 * \param strain strain in Voigt form (3)
 * \param theta rotated auto-clock angle in radians
 * \return new rotated strain
 */
vec transform::rotate_strain(const vec& strain, const double theta) { return rotate_strain(strain, form_trans(theta)); }

/**
 * \brief
 * \param stress stress in Voigt form (3)
 * \param theta rotated auto-clock angle in radians
 * \return new rotated stress
 */
vec transform::rotate_stress(const vec& stress, const double theta) { return rotate_stress(stress, form_trans(theta)); }

mat transform::beam::global_to_local(const double, const double, const double, const double) {
    mat trans_mat(5, 12, fill::zeros);
    return trans_mat;
}

mat transform::beam::global_to_local(const double cosine, const double sine, const double length) {

    const auto tmp_a = cosine / length;
    const auto tmp_b = sine / length;

    mat trans_mat(3, 6, fill::zeros);
    trans_mat(0, 0) = -cosine;
    trans_mat(0, 1) = -sine;
    trans_mat(0, 3) = cosine;
    trans_mat(0, 4) = sine;
    trans_mat(1, 0) = -tmp_b;
    trans_mat(1, 1) = tmp_a;
    trans_mat(1, 3) = tmp_b;
    trans_mat(1, 4) = -tmp_a;
    trans_mat(1, 2) = 1.;
    trans_mat(2, 0) = -tmp_b;
    trans_mat(2, 1) = tmp_a;
    trans_mat(2, 3) = tmp_b;
    trans_mat(2, 4) = -tmp_a;
    trans_mat(2, 5) = 1.;

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
