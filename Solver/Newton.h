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
 * @class Newton
 * @brief A Newton class defines a solver using Newton--Raphson
 * iteration.
 * @author T
 * @date 27/08/2017
 * @version 0.1.2
 * @file Newton.h
 * @addtogroup Solver
 * @{
 */

#ifndef NEWTON_H
#define NEWTON_H

#include "Solver.h"

class Newton : public Solver {
public:
    explicit Newton(const unsigned& = 0, const shared_ptr<Converger>& = nullptr, const shared_ptr<Integrator>& = nullptr);

    int update_status() override;

    int analyze() override;

    void print() override;
};

#endif

//! @}
