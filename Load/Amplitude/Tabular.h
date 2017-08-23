/**
 * @class Tabular
 * @brief A Tabular class that can generate Amplitude pattern.
 *
 * @author T
 * @date 15/07/2017
 * @file Tabular.h
 */

#ifndef TABULAR_H
#define TABULAR_H

#include "Amplitude.h"

class Tabular : public Amplitude
{
    vec time;      /**< time */
    vec magnitude; /**< magnitude */
public:
    explicit Tabular(const unsigned& = 0, const unsigned& = CT_TABULAR);
    Tabular(const unsigned&, const vec&, const vec&);
    Tabular(const unsigned&, const char*);
    ~Tabular();

    double get_amplitude(const double&) override final;

    void print() override final;
};

#endif
