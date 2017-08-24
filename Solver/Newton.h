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
public:
    explicit Newton(const unsigned& = 0, const shared_ptr<Domain>& = nullptr);
    explicit Newton(const shared_ptr<Domain>&);

    int update_status() override;

    int analyze(const unsigned&) override;

    void print() override;
};

#endif

//! @}
