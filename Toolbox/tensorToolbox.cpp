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
