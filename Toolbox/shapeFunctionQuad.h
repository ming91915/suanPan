/**
 * @fn shapeFunctionQuad
 * @brief An shapeFunctionQuad function.
 *
 * The function is designed to return the shape function for four- and eight-node quad
 * elements.
 *
 * @author T
 * @date 03/07/2017
 * @file integrationPlan.h
 */

#ifndef SHAPEFUNCTIONQUAD_H
#define SHAPEFUNCTIONQUAD_H

#include <suanPan.h>

mat shapeFunctionQuad(const vec&, const unsigned&, const unsigned& = 4);

#endif
