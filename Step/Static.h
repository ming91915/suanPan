/**
 * @class Static
 * @brief A Static class.
 *
 * This class corresponds to the Static, General step in Abaqus, which handles a static
 * problem using Newton (or Newton like) solvers.
 *
 * @author T
 * @date 03/07/2017
 * @file Static.h
 */

#ifndef STATIC_H
#define STATIC_H

#include "Step.h"

class Static : public Step
{
public:
    explicit Static(const unsigned& = 0) {}
    ~Static() {}
};

#endif
