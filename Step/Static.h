/**
 * @class Static
 * @brief A Static class.
 *
 * This class corresponds to the Static, General step in Abaqus, which handles a static
 * problem using Newton (or quasi Newton) solvers.
 *
 * @author T
 * @date 16/08/2017
 * @file Static.h
 */

#ifndef STATIC_H
#define STATIC_H

#include "Step.h"

class Static : public Step
{
public:
    explicit Static(const unsigned& = 0,
        const shared_ptr<Solver>& = nullptr,
        const double& = 1.);

    int analyze() override;
};

#endif
