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

mat shapeStress(const double& X, const double& Y, const unsigned& S) {
    mat N = zeros(3, S);

    for(auto I = 0; I < 3; ++I) N(I, I) = 1;

    if(S >= 5) {
        N(0, 4) = Y;
        N(1, 3) = X;
        if(S >= 7) {
            N(0, 6) = X;
            N(1, 5) = Y;
            N(2, 5) = -X;
            N(2, 6) = -Y;
            if(S >= 9) {
                const auto X2 = X * X;
                const auto Y2 = Y * Y;
                const auto XY = X * Y;
                N(0, 8) = 2. * XY;
                N(1, 7) = N(0, 8);
                N(2, 7) = -X2;
                N(2, 8) = -Y2;
                if(S == 11) {
                    N(0, 9) = -X2;
                    N(1, 9) = 2. * X2 - Y2;
                    N(2, 9) = 2. * XY;

                    N(0, 10) = 2. * Y2 - X2;
                    N(1, 10) = -Y2;
                    N(2, 10) = N(2, 9);
                }
            }
        }
    }

    return N;
}

mat shapeStress(const vec& C, const unsigned& S) { return shapeStress(C(0), C(1), S); }

mat shapeStress5(const vec& C) { return shapeStress(C, 5); }

mat shapeStress7(const vec& C) { return shapeStress(C, 7); }

mat shapeStress9(const vec& C) { return shapeStress(C, 9); }

mat shapeStress11(const vec& C) { return shapeStress(C, 11); }

mat shapeStress5(const double& X, const double& Y) { return shapeStress(X, Y, 5); }

mat shapeStress7(const double& X, const double& Y) { return shapeStress(X, Y, 7); }

mat shapeStress9(const double& X, const double& Y) { return shapeStress(X, Y, 9); }

mat shapeStress11(const double& X, const double& Y) { return shapeStress(X, Y, 11); }

mat shapeStrain(const double& X, const double& Y, const double& V, const unsigned& S) {
    mat N(3, S, fill::zeros);

    for(auto I = 0; I < 3; ++I) N(I, I) = 1;

    N(0, 1) = -V;
    N(1, 0) = -V;

    if(S >= 5) {
        N(0, 3) = -V * X;
        N(0, 4) = Y;

        N(1, 3) = X;
        N(1, 4) = -V * Y;
        if(S >= 7) {
            N(0, 5) = N(1, 4);
            N(0, 6) = N(1, 3);

            N(1, 5) = N(0, 4);
            N(1, 6) = N(0, 3);

            N(2, 5) = -2. * X * (1. + V);
            N(2, 6) = -2. * Y * (1. + V);
            if(S >= 9) {
                const auto X2 = X * X;
                const auto Y2 = Y * Y;
                const auto XY = X * Y;

                N(0, 8) = XY;
                N(0, 7) = -V * N(0, 8);

                N(1, 7) = N(0, 8);
                N(1, 8) = N(0, 7);

                N(2, 7) = -X2 * (1. + V);
                N(2, 8) = -Y2 * (1. + V);
                if(S == 11) {
                    N(0, 9) = V * Y2 - (2. * V + 1.) * X2;
                    N(1, 9) = (2. + V) * X2 - Y2;
                    N(2, 9) = 4. * XY * (1. + V);

                    N(0, 10) = (2. + V) * Y2 - X2;
                    N(1, 10) = V * X2 - (2. * V + 1.) * Y2;
                    N(2, 10) = N(2, 9);
                }
            }
        }
    }

    return N;
}

mat shapeStrain(const vec& C, const double& V, const unsigned& S) { return shapeStrain(C(0), C(1), V, S); }

mat shapeStrain5(const double& X, const double& Y, const double& V) { return shapeStrain(X, Y, V, 5); }

mat shapeStrain7(const double& X, const double& Y, const double& V) { return shapeStrain(X, Y, V, 7); }

mat shapeStrain9(const double& X, const double& Y, const double& V) { return shapeStrain(X, Y, V, 9); }

mat shapeStrain11(const double& X, const double& Y, const double& V) { return shapeStrain(X, Y, V, 11); }

mat shapeStrain5(const vec& C, const double& V) { return shapeStrain(C, V, 5); }

mat shapeStrain7(const vec& C, const double& V) { return shapeStrain(C, V, 7); }

mat shapeStrain9(const vec& C, const double& V) { return shapeStrain(C, V, 9); }

mat shapeStrain11(const vec& C, const double& V) { return shapeStrain(C, V, 11); }

mat form_trans(const double angle) {
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

void nominal_to_principal_strain(vec& strain) {
    const auto tmp_a = (strain(0) + strain(1)) / 2.;
    const auto tmp_b = sqrt(pow(strain(0) - strain(1), 2.) + pow(strain(2), 2.)) / 2.;

    strain(0) = tmp_a + tmp_b;
    strain(1) = tmp_a - tmp_b;
    strain(2) = 0.;
}

void nominal_to_principal_stress(vec& stress) {
    const auto tmp_a = (stress(0) + stress(1)) / 2.;
    const auto tmp_b = sqrt(pow(stress(0) - stress(1), 2.) + pow(2. * stress(2), 2.)) / 2.;

    stress(0) = tmp_a + tmp_b;
    stress(1) = tmp_a - tmp_b;
    stress(2) = 0.;
}

mat nominal_to_principal_strain(vec& strain, double& theta) {
    theta = atan(strain(2) / (strain(0) - strain(1))) / 2.;

    nominal_to_principal_strain(strain);

    return form_trans(theta);
}

mat nominal_to_principal_stress(vec& stress, double& theta) {
    theta = atan(2. * stress(2) / (stress(0) - stress(1))) / 2.;

    nominal_to_principal_stress(stress);

    return form_trans(theta);
}
