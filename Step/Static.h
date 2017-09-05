/**
 * @class Static
 * @brief A Static class.
 *
 * This class corresponds to the Static, General step in Abaqus, which
 * handles a static
 * problem using Newton (or quasi Newton) solvers.
 *
 * @author T
 * @date 26/08/2017
 * @version 0.1.1
 * @file Static.h
 * @addtogroup Step
 * @{
 */

#ifndef STATIC_H
#define STATIC_H

#include "Step.h"

class Static : public Step {
public:
    explicit Static(const unsigned& = 0, // tag
        const double& = 1.);             // step time period

    int analyze() override;
};

#endif

//! @}
