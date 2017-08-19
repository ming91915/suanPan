/**
 * @class Newton
 * @brief A Newton class defines a solver using Newton--Raphson iteration.
 * @author T
 * @date 05/08/2017
 * @version 0.1.1
 * @file Newton.h
 * @{
 */

#ifndef NEWTON_H
#define NEWTON_H

#include "Solver.h"

class Newton : public Solver
{
    bool fixed_size = false; /**< auto-stepping */

    double ini_step_size = 1; /**< initial step size */

    double min_step_size = 1E-6; /**< minimum step size */
    double max_step_size = 1;    /**< maximum step size */

    unsigned max_iteration = 7;       /**< maximum iteration per sub-step */
    unsigned max_increment = 1000000; /**< maximum increment number */
public:
    explicit Newton(const unsigned& = 0,
        const shared_ptr<Domain>& = nullptr,
        const shared_ptr<Convergence>& = nullptr);
    explicit Newton(const shared_ptr<Domain>&, const shared_ptr<Convergence>& = nullptr);

    int updateStatus() override;

    int analyze(const double&) override;

    void setStepSize(const double&, const double& = 0, const double& = 0);

    void print() override;
};

#endif

//! @}
