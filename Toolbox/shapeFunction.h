/**
 * @fn shapeFunction
 * @brief An shapeFunction function.
 *
 * The function is designed to return the shape function for four- and eight-node quad elements.
 *
 * @author T
 * @date 03/07/2017
 * @file shapeFunction.h
 */

#ifndef SHAPEFUNCTION_H
#define SHAPEFUNCTION_H

#include <armadillo>

using namespace arma;

template <typename T> Mat<T> shapeFunctionQuad(const Col<T>&, const unsigned&, const unsigned& = 4);

#endif
