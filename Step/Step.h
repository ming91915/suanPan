/**
 * @class Step
 * @brief A Step class.
 * @author T
 * @date 03/07/2017
 * @file Step.h
 */

#ifndef STEP_H
#define STEP_H

#include <Domain/Tag.h>
#include <Solver/Solver.h>

class Step : public Tag
{
    double time_period = 1.0; /**< time period */
    shared_ptr<Solver> solver;

public:
    explicit Step(const unsigned& = 0,
        const unsigned& = CT_STEP,
        const shared_ptr<Solver>& = nullptr,
        const double& = 1.);
    virtual ~Step() {}

    virtual int analyze();

    void setSolver(const shared_ptr<Solver>&);
    const shared_ptr<Solver>& getSolver() const;

    void setTimePerid(const double&);
    const double& getTimePeriod() const;
};

#endif
