/**
 * @class Dynamic
 * @brief A Dynamic class.
 * @author T
 * @date 03/07/2017
 * @file Dynamic.h
 */

#ifndef DYNAMIC_H
#define DYNAMIC_H

#include "Step.h"

class Dynamic : public Step
{
public:
    explicit Dynamic(const unsigned& T = 0, const double& P = 1.)
        : Step(T, CT_DYNAMIC, P)
    {
    }

    virtual ~Dynamic() {}

    int analyze() override;
};

#endif
