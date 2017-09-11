#include "shapeFunction.h"

template <typename T> Mat<T> shapeFunctionQuad(const Col<T>& INTPTS, const unsigned& ORDER, const unsigned& NODENUM) {
    const auto& X = INTPTS(0);
    const auto& Y = INTPTS(1);

    Mat<T> N(ORDER + 1, NODENUM, fill::zeros);

    if(ORDER != 0 && ORDER != 1) {
        N.reset();
        return N;
    }

    const auto XP = 1. + X;
    const auto XM = 1. - X;
    const auto YP = 1. + Y;
    const auto YM = 1. - Y;

    if(NODENUM == 4) {

        if(ORDER == 0) {
            N(0, 3) = XM * YP;
            N(0, 2) = XP * YP;
            N(0, 1) = XP * YM;
            N(0, 0) = XM * YM;
        } else if(ORDER == 1) {
            N(1, 3) = XM;
            N(1, 2) = XP;
            N(1, 1) = -XP;
            N(1, 0) = -XM;
            N(0, 3) = -YP;
            N(0, 2) = YP;
            N(0, 1) = YM;
            N(0, 0) = -YM;
        }
        N /= 4.;
    } else if(NODENUM == 8) {
        const auto XX = X * X;
        const auto YY = Y * Y;
        const auto XY = X * Y;
        if(ORDER == 0) {
            N(0, 7) = .5 * XM * (1. - YY);
            N(0, 6) = .5 * (1. - XX) * YP;
            N(0, 5) = .5 * XP * (1. - YY);
            N(0, 4) = .5 * (1. - XX) * YM;
            N(0, 0) = .25 * XM * YM - .5 * N(0, 4) - .5 * N(0, 7);
            N(0, 1) = .25 * XP * YM - .5 * N(0, 4) - .5 * N(0, 5);
            N(0, 2) = .25 * XP * YP - .5 * N(0, 5) - .5 * N(0, 6);
            N(0, 3) = .25 * XM * YP - .5 * N(0, 6) - .5 * N(0, 7);
        } else if(ORDER == 1) {
            const auto X2 = X / 2.;
            const auto Y2 = Y / 2.;
            const auto X4 = X / 4.;
            const auto Y4 = Y / 4.;
            const auto X24 = XX / 4.;
            const auto Y24 = YY / 4.;
            const auto XY2 = XY / 2.;
            N(1, 7) = XY - Y;
            N(1, 6) = .5 - XX / 2.;
            N(1, 5) = -Y - XY;
            N(1, 4) = XX / 2. - .5;
            N(1, 3) = Y2 - X4 - XY2 + X24;
            N(1, 2) = X4 + Y2 + XY2 + X24;
            N(1, 1) = Y2 - X4 + XY2 - X24;
            N(1, 0) = X4 + Y2 - XY2 - X24;
            N(0, 7) = YY / 2. - .5;
            N(0, 6) = -X - XY;
            N(0, 5) = .5 - YY / 2.;
            N(0, 4) = XY - X;
            N(0, 3) = X2 - Y4 + XY2 - Y24;
            N(0, 2) = X2 + Y4 + XY2 + Y24;
            N(0, 1) = X2 - Y4 - XY2 + Y24;
            N(0, 0) = X2 + Y4 - XY2 - Y24;
        }
    }

    return N;
};
