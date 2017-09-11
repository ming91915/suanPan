/**
 * @fn shapeFunction
 * @brief An shapeFunction function.
 *
 * The function is designed to return the shape function for four- and eight-node quad elements.
 *
 * @author T
 * @date 11/09/2017
 * @version 0.1.1
 * @file shapeFunction.hpp
 * @addtogroup Utility
 * @{
 */

#ifndef SHAPEFUNCTION_HPP
#define SHAPEFUNCTION_HPP

#include <armadillo>

using namespace arma;

template <typename T> Mat<T> shapeFunctionQuad(const Col<T>& INTPTS, const unsigned& ORDER, const unsigned& NODENUM = 4) {
    Mat<T> N;

    if(ORDER != 0 && ORDER != 1) return N;

    N.zeros(ORDER + 1, NODENUM);

    const auto& X = INTPTS(0);
    const auto& Y = INTPTS(1);

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
}

template <typename T> Mat<T> shapeFunctionCube(const Col<T>& INTPTS, const unsigned& ORDER, const unsigned& NODENUM = 8) {
    Mat<T> N;

    if(ORDER == 0)
        N.zeros(1, NODENUM);
    else if(ORDER == 1)
        N.zeros(3, NODENUM);
    else
        return N;

    const auto& X = INTPTS(0);
    const auto& Y = INTPTS(1);
    const auto& Z = INTPTS(2);

    const auto XP = X + 1.;
    const auto XM = X - 1.;
    const auto YP = Y + 1.;
    const auto YM = Y - 1.;
    const auto ZP = Z + 1.;
    const auto ZM = Z - 1.;

    if(NODENUM == 8) {
        if(ORDER == 0) {
            N(0, 0) = -XM * YM * ZM;
            N(0, 1) = XP * YM * ZM;
            N(0, 2) = -XP * YP * ZM;
            N(0, 3) = XM * YP * ZM;
            N(0, 4) = XM * YM * ZP;
            N(0, 5) = -XP * YM * ZP;
            N(0, 6) = XP * YP * ZP;
            N(0, 7) = -XM * YP * ZP;
        } else if(ORDER == 1) {
            N(0, 0) = -YM * ZM;
            N(0, 1) = YM * ZM;
            N(0, 2) = -YP * ZM;
            N(0, 3) = YP * ZM;
            N(0, 4) = YM * ZP;
            N(0, 5) = -YM * ZP;
            N(0, 6) = YP * ZP;
            N(0, 7) = -YP * ZP;
            N(1, 0) = -XM * ZM;
            N(1, 1) = XP * ZM;
            N(1, 2) = -XP * ZM;
            N(1, 3) = XM * ZM;
            N(1, 4) = XM * ZP;
            N(1, 5) = -XP * ZP;
            N(1, 6) = XP * ZP;
            N(1, 7) = -XM * ZP;
            N(2, 0) = -XM * YM;
            N(2, 1) = XP * YM;
            N(2, 2) = -XP * YP;
            N(2, 3) = XM * YP;
            N(2, 4) = XM * YM;
            N(2, 5) = -XP * YM;
            N(2, 6) = XP * YP;
            N(2, 7) = -XM * YP;
        }
        N /= 8.;
    } else if(NODENUM == 20) {
        const auto XX = XP * XM;
        const auto YY = YP * YM;
        const auto ZZ = ZP * ZM;
        if(ORDER == 0) {
            N(0, 0) = XM * YM * ZM * (X + Y + Z + 2.) / 8.;
            N(0, 1) = -XP * YM * ZM * (Y - X + Z + 2.) / 8.;
            N(0, 2) = -XP * YP * ZM * (X + Y - Z - 2.) / 8.;
            N(0, 3) = -XM * YP * ZM * (X - Y + Z + 2.) / 8.;
            N(0, 4) = -XM * YM * ZP * (X + Y - Z + 2.) / 8.;
            N(0, 5) = -XP * YM * ZP * (X - Y + Z - 2.) / 8.;
            N(0, 6) = XP * YP * ZP * (X + Y + Z - 2.) / 8.;
            N(0, 7) = XM * YP * ZP * (X - Y - Z + 2.) / 8.;
            N(0, 8) = -XX * YM * ZM / 4.;
            N(0, 9) = YY * XP * ZM / 4.;
            N(0, 10) = XX * YP * ZM / 4.;
            N(0, 11) = -YY * XM * ZM / 4.;
            N(0, 12) = XX * YM * ZP / 4.;
            N(0, 13) = -YY * XP * ZP / 4.;
            N(0, 14) = -XX * YP * ZP / 4.;
            N(0, 15) = YY * XM * ZP / 4.;
            N(0, 16) = -ZZ * XM * YM / 4.;
            N(0, 17) = ZZ * XP * YM / 4.;
            N(0, 18) = -ZZ * XP * YP / 4.;
            N(0, 19) = ZZ * XM * YP / 4.;
        } else if(ORDER == 1) {
            N(0, 0) = XM * YM * ZM / 8. + YM * ZM * (X + Y + Z + 2.) / 8;
            N(0, 1) = XP * YM * ZM / 8. - YM * ZM * (Y - X + Z + 2.) / 8.;
            N(0, 2) = -XP * YP * ZM / 8. - YP * ZM * (X + Y - Z - 2.) / 8.;
            N(0, 3) = -XM * YP * ZM / 8. - YP * ZM * (X - Y + Z + 2.) / 8.;
            N(0, 4) = -XM * YM * ZP / 8. - YM * ZP * (X + Y - Z + 2.) / 8.;
            N(0, 5) = -XP * YM * ZP / 8. - YM * ZP * (X - Y + Z - 2.) / 8.;
            N(0, 6) = XP * YP * ZP / 8. + YP * ZP * (X + Y + Z - 2.) / 8.;
            N(0, 7) = XM * YP * ZP / 8. + YP * ZP * (X - Y - Z + 2.) / 8.;
            N(0, 8) = -X * YM * ZM / 2.;
            N(0, 9) = YY * ZM / 4.;
            N(0, 10) = X * YP * ZM / 2.;
            N(0, 11) = -YY * ZM / 4.;
            N(0, 12) = X * YM * ZP / 2.;
            N(0, 13) = -YY * ZP / 4.;
            N(0, 14) = -X * YP * ZP / 2.;
            N(0, 15) = YY * ZP / 4.;
            N(0, 16) = -ZZ * YM / 4.;
            N(0, 17) = ZZ * YM / 4.;
            N(0, 18) = -ZZ * YP / 4.;
            N(0, 19) = ZZ * YP / 4.;
            N(1, 0) = XM * YM * ZM / 8. + XM * ZM * (X + Y + Z + 2.) / 8.;
            N(1, 1) = -XP * YM * ZM / 8. - XP * ZM * (Y - X + Z + 2.) / 8.;
            N(1, 2) = -XP * YP * ZM / 8. - XP * ZM * (X + Y - Z - 2.) / 8.;
            N(1, 3) = XM * YP * ZM / 8. - XM * ZM * (X - Y + Z + 2.) / 8.;
            N(1, 4) = -XM * YM * ZP / 8. - XM * ZP * (X + Y - Z + 2.) / 8.;
            N(1, 5) = XP * YM * ZP / 8. - XP * ZP * (X - Y + Z - 2.) / 8.;
            N(1, 6) = XP * YP * ZP / 8. + XP * ZP * (X + Y + Z - 2.) / 8.;
            N(1, 7) = -XM * YP * ZP / 8. + XM * ZP * (X - Y - Z + 2.) / 8.;
            N(1, 8) = -XX * ZM / 4.;
            N(1, 9) = Y * XP * ZM / 2.;
            N(1, 10) = XX * ZM / 4.;
            N(1, 11) = -Y * XM * ZM / 2.;
            N(1, 12) = XX * ZP / 4.;
            N(1, 13) = -Y * XP * ZP / 2.;
            N(1, 14) = -XX * ZP / 4.;
            N(1, 15) = Y * XM * ZP / 2.;
            N(1, 16) = -ZZ * XM / 4.;
            N(1, 17) = ZZ * XP / 4.;
            N(1, 18) = -ZZ * XP / 4.;
            N(1, 19) = ZZ * XM / 4.;
            N(2, 0) = XM * YM * ZM / 8. + XM * YM * (X + Y + Z + 2.) / 8.;
            N(2, 1) = -XP * YM * ZM / 8. - XP * YM * (Y - X + Z + 2.) / 8.;
            N(2, 2) = XP * YP * ZM / 8. - XP * YP * (X + Y - Z - 2.) / 8.;
            N(2, 3) = -XM * YP * ZM / 8. - XM * YP * (X - Y + Z + 2.) / 8.;
            N(2, 4) = XM * YM * ZP / 8. - XM * YM * (X + Y - Z + 2.) / 8.;
            N(2, 5) = -XP * YM * ZP / 8. - XP * YM * (X - Y + Z - 2.) / 8.;
            N(2, 6) = XP * YP * ZP / 8. + XP * YP * (X + Y + Z - 2.) / 8.;
            N(2, 7) = -XM * YP * ZP / 8. + XM * YP * (X - Y - Z + 2.) / 8.;
            N(2, 8) = -XX * YM / 4.;
            N(2, 9) = YY * XP / 4.;
            N(2, 10) = XX * YP / 4.;
            N(2, 11) = -YY * XM / 4.;
            N(2, 12) = XX * YM / 4.;
            N(2, 13) = -YY * XP / 4.;
            N(2, 14) = -XX * YP / 4.;
            N(2, 15) = YY * XM / 4.;
            N(2, 16) = -Z * XM * YM / 2.;
            N(2, 17) = Z * XP * YM / 2.;
            N(2, 18) = -Z * XP * YP / 2.;
            N(2, 19) = Z * XM * YP / 2.;
        }
    }

    return N;
}

#endif

//! @}
