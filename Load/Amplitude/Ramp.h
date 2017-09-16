/*******************************************************************************
 * Copyright (C) 2017 Theodore Chang
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/
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
