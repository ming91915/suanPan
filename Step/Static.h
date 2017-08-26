/**
 * @class Static
 * @brief A Static class.
 *
 * This class corresponds to the Static, General step in Abaqus, which handles a static
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

class Static : public Step
{
    bool fixed_size = false; /**< auto-stepping */

    double ini_step_size = 1; /**< initial step size */

    double min_step_size = 1E-8; /**< minimum step size */
    double max_step_size = 1;    /**< maximum step size */

    unsigned max_increment = 1000000; /**< maximum increment number */
public:
    explicit Static(const unsigned& = 0, // tag
        const double& = 1.);             // step time period

    int analyze() override;
};

#endif

//! @}
