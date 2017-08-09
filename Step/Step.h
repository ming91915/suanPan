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

class Step : public Tag
{
    double time_period = 1.0; /**< time period */
public:
    explicit Step(const unsigned& = 0, const unsigned& = CT_STEP) {}
    virtual ~Step() {}
};

#endif
