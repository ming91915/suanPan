/**
 * @class Frequence
 * @brief A Frequence class.
 * @author T
 * @date 03/07/2017
 * @file Frequence.h
 */

#ifndef FREQUENCE_H
#define FREQUENCE_H

#include "Step.h"

class Frequence : public Step
{
public:
    explicit Frequence(const unsigned& T = 0)
        : Step(T, CT_FREQUENCE)
    {
    }

    virtual ~Frequence() {}
};

#endif
