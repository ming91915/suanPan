#include "shapeFunctionQuad.h"

mat shapeFunctionQuad(const vec& INTPTS, const unsigned& ORDER, const unsigned& NODENUM) {
    const auto& X = INTPTS(0);
    const auto& Y = INTPTS(1);

    mat N(ORDER + 1, NODENUM, fill::zeros);

    if(ORDER != 0 && ORDER != 1) {
        printf("Wrong Order.\n");
        return N;
    }

    const auto XP = 1. + X;
    const auto XM = 1. - X;
    const auto YP = 1. + Y;
    const auto YM = 1. - Y;

    if(NODENUM == 4) {
        switch(ORDER) {
        case 0:
            N(0, 3) = XM * YP;
            N(0, 2) = XP * YP;
            N(0, 1) = XP * YM;
            N(0, 0) = XM * YM;
            break;
        case 1:
            N(1, 3) = XM;
            N(1, 2) = XP;
            N(1, 1) = -XP;
            N(1, 0) = -XM;
            N(0, 3) = -YP;
            N(0, 2) = YP;
            N(0, 1) = YM;
            N(0, 0) = -YM;
            break;
        default:
            break;
        }
        N /= 4.;
    } else if(NODENUM == 8) {
        const auto X2 = X * X;
        const auto Y2 = Y * Y;
        const auto XY = X * Y;
        switch(ORDER) {
        case 0:
            N(0, 7) = .5 * XM * (1. - Y2);
            N(0, 6) = .5 * (1. - X2) * YP;
            N(0, 5) = .5 * XP * (1. - Y2);
            N(0, 4) = .5 * (1. - X2) * YM;
            N(0, 0) = .25 * XM * YM - .5 * N(0, 4) - .5 * N(0, 7);
            N(0, 1) = .25 * XP * YM - .5 * N(0, 4) - .5 * N(0, 5);
            N(0, 2) = .25 * XP * YP - .5 * N(0, 5) - .5 * N(0, 6);
            N(0, 3) = .25 * XM * YP - .5 * N(0, 6) - .5 * N(0, 7);
            break;
        case 1:
            N(1, 7) = XY - Y;
            N(1, 6) = .5 - X2 / 2.;
            N(1, 5) = -Y - XY;
            N(1, 4) = X2 / 2. - .5;
            N(1, 3) = Y / 2. - X / 4. - XY / 2. + X2 / 4.;
            N(1, 2) = X / 4. + Y / 2. + XY / 2. + X2 / 4.;
            N(1, 1) = Y / 2. - X / 4. + XY / 2. - X2 / 4.;
            N(1, 0) = X / 4. + Y / 2. - XY / 2. - X2 / 4.;
            N(0, 7) = Y2 / 2. - .5;
            N(0, 6) = -X - XY;
            N(0, 5) = .5 - Y2 / 2.;
            N(0, 4) = XY - X;
            N(0, 3) = X / 2. - Y / 4. + XY / 2. - Y2 / 4.;
            N(0, 2) = X / 2. + Y / 4. + XY / 2. + Y2 / 4.;
            N(0, 1) = X / 2. - Y / 4. - XY / 2. + Y2 / 4.;
            N(0, 0) = X / 2. + Y / 4. - XY / 2. - Y2 / 4.;
            break;
        default:
            break;
        }
    }

    return N;
};
