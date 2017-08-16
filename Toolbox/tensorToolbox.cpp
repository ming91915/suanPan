#include "tensorToolbox.h"

mat unitDevTensor4()
{
    mat T = zeros(6, 6);

    for(auto I = 3; I < 6; ++I) T(I, I) = .5;

    for(auto I = 0; I < 3; ++I)
        for(auto J = 0; J < 3; ++J) T(I, J) = I == J ? 2. / 3. : -1. / 3.;

    return T;
}

double tr(const vec& S)
{
    auto T = 0.;
    for(auto I = 0; I < 3; ++I) T += S(I);
    return T;
}

double mean(const vec& S) { return tr(S) / 3.; }

vec dev(const vec& S)
{
    auto D = S;
    auto M = mean(S);
    for(auto I = 0; I < 3; ++I) D(I) -= M;
    return D;
}

mat shapeStress(const double& X, const double& Y, const unsigned& S)
{
    mat N(3, S, fill::zeros);

    for(auto I = 0; I < 3; ++I) N(I, I) = 1;

    switch(S) {
    case 5:
        N(0, 4) = Y;
        N(1, 3) = X;
    case 7:
        N(0, 6) = X;
        N(1, 5) = Y;
        N(2, 5) = -X;
        N(2, 6) = -Y;
    case 9:
        N(0, 8) = 2. * X * Y;
        N(1, 7) = N(0, 8);
        N(2, 7) = -X * X;
        N(2, 8) = -Y * Y;
    default:
        suanpan_error("shapeStress() cannot identify the size.\n");
        return N;
    }
}

mat shapeStress(const vec& C, const unsigned& S) { return shapeStress(C(0), C(1), S); }

mat shapeStress5(const vec& C) { return shapeStress(C, 5); }

mat shapeStress7(const vec& C) { return shapeStress(C, 7); }

mat shapeStress9(const vec& C) { return shapeStress(C, 9); }

mat shapeStress5(const double& X, const double& Y) { return shapeStress(X, Y, 5); }

mat shapeStress7(const double& X, const double& Y) { return shapeStress(X, Y, 7); }

mat shapeStress9(const double& X, const double& Y) { return shapeStress(X, Y, 9); }

mat shapeStrain(const vec& C, const double& V, const unsigned& S)
{
    mat N(3, S, fill::zeros);

    for(auto I = 0; I < 3; ++I) N(I, I) = 1;

    N(0, 1) = -V;
    N(1, 0) = -V;

    switch(S) {
    case 5:
        N(0, 3) = -V * C(0);
        N(0, 4) = C(1);

        N(1, 3) = C(0);
        N(1, 4) = -V * C(1);
    case 7:
        N(0, 5) = N(1, 4);
        N(0, 6) = N(1, 3);

        N(1, 5) = N(0, 4);
        N(1, 6) = N(0, 3);

        N(2, 5) = -2. * C(0) * (1. + V);
        N(2, 6) = -2. * C(1) * (1. + V);
    case 9:
        N(0, 8) = C(0) * C(1);
        N(0, 7) = -V * N(0, 8);

        N(1, 7) = N(0, 8);
        N(1, 8) = N(0, 7);

        N(2, 7) = -C(0) * C(0) * (1. + V);
        N(2, 8) = -C(1) * C(1) * (1. + V);
    default:
        return N;
    }
}

mat shapeStrain5(const vec& C, const unsigned& V) { return shapeStrain(C, V, 5); }

mat shapeStrain7(const vec& C, const unsigned& V) { return shapeStrain(C, V, 7); }

mat shapeStrain9(const vec& C, const unsigned& V) { return shapeStrain(C, V, 9); }
