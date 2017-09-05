/**
 * @class Ramp
 * @brief An Amplitude class that can generate Amplitude pattern.
 *
 * Linear/Ramp
 * \f{gather}{a=t\f}
 *
 * @author T
 * @date 03/07/2017
 * @file Ramp.h
 */

#ifndef RAMP_H
#define RAMP_H

#include "Amplitude.h"

class Ramp : public Amplitude {
public:
    explicit Ramp(const unsigned& = 0, const unsigned& = CT_RAMP);
    ~Ramp();

    double get_amplitude(const double&) override final;

    void print() override final;
};

#endif
