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
 * @class Ramm
 * @brief A Ramm class defines a solver using Ramm's version of arc--length method.
 * @author T
 * @date 27/07/2017
 * @version 0.1.0
 * @file Ramm.h
 */

#ifndef RAMM_H
#define RAMM_H

#include "Solver.h"

class Ramm : public Solver {
    double arc_length = 1E-4;
    bool fixed_arc_length = false;

public:
    explicit Ramm(const unsigned& = 0);

    int analyze() override;

    void print() override;
};

#endif
