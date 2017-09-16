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

class Tabular : public Amplitude {
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
