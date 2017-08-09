#include "shapeFunctionQuad.h"

mat shapeFunctionQuad(const vec& INTPTS, const unsigned& ORDER, const unsigned& NODENUM)
{
    const auto& X = INTPTS(0);
    const auto& Y = INTPTS(1);

    mat N(ORDER + 1, NODENUM, fill::zeros);

    if(ORDER != 0 && ORDER != 1) {
        printf("Wrong Order.\n");
        return N;
    }

    if(NODENUM == 4) {
        switch(ORDER) {
        case 0:
            N(0, 3) = (1. - X) * (1. + Y);
            N(0, 2) = (1. + X) * (1. + Y);
            N(0, 1) = (1. + X) * (1. - Y);
            N(0, 0) = (1. - X) * (1. - Y);
            break;
        case 1:
            N(1, 3) = 1. - X;
            N(1, 2) = X + 1.;
            N(1, 1) = -X - 1.;
            N(1, 0) = X - 1.;
            N(0, 3) = -Y - 1.;
            N(0, 2) = Y + 1.;
            N(0, 1) = 1. - Y;
            N(0, 0) = Y - 1.;
            break;
        default:
            break;
        }
        N /= 4.;
    } else if(NODENUM == 8) {
        switch(ORDER) {
        case 0:
            N(0, 7) = .5 * (1. - X) * (1. - Y * Y);
            N(0, 6) = .5 * (1. - X * X) * (1. + Y);
            N(0, 5) = .5 * (1. + X) * (1. - Y * Y);
            N(0, 4) = .5 * (1. - X * X) * (1. - Y);
            N(0, 0) = .25 * (1. - X) * (1. - Y) - .5 * N(0, 4) - .5 * N(0, 7);
            N(0, 1) = .25 * (1. + X) * (1. - Y) - .5 * N(0, 4) - .5 * N(0, 5);
            N(0, 2) = .25 * (1. + X) * (1. + Y) - .5 * N(0, 5) - .5 * N(0, 6);
            N(0, 3) = .25 * (1. - X) * (1. + Y) - .5 * N(0, 6) - .5 * N(0, 7);
            break;
        case 1:
            N(1, 7) = X * Y - Y;
            N(1, 6) = .5 - X * X / 2.;
            N(1, 5) = -Y - X * Y;
            N(1, 4) = X * X / 2. - .5;
            N(1, 3) = Y / 2. - X / 4. - X * Y / 2. + X * X / 4.;
            N(1, 2) = X / 4. + Y / 2. + X * Y / 2. + X * X / 4.;
            N(1, 1) = Y / 2. - X / 4. + X * Y / 2. - X * X / 4.;
            N(1, 0) = X / 4. + Y / 2. - X * Y / 2. - X * X / 4.;
            N(0, 7) = Y * Y / 2. - .5;
            N(0, 6) = -X - X * Y;
            N(0, 5) = .5 - Y * Y / 2.;
            N(0, 4) = X * Y - X;
            N(0, 3) = X / 2. - Y / 4. + X * Y / 2. - Y * Y / 4.;
            N(0, 2) = X / 2. + Y / 4. + X * Y / 2. + Y * Y / 4.;
            N(0, 1) = X / 2. - Y / 4. - X * Y / 2. + Y * Y / 4.;
            N(0, 0) = X / 2. + Y / 4. - X * Y / 2. - Y * Y / 4.;
            break;
        default:
            break;
        }
    }

    return N;
};
