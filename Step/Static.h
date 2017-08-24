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
    bool fixed_size = false; /**< auto-stepping */

    double ini_step_size = 1; /**< initial step size */

    double min_step_size = 1E-6; /**< minimum step size */
    double max_step_size = 1;    /**< maximum step size */

    unsigned max_increment = 1000000; /**< maximum increment number */
public:
    explicit Static(const unsigned& = 0,
        const shared_ptr<Domain>& = nullptr,
        const shared_ptr<Solver>& = nullptr,
        const shared_ptr<Convergence>& = nullptr,
        const double& = 1.);

    int analyze() override;
};

#endif
