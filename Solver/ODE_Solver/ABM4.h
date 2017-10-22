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
 * @class ABM4
 * @brief A solver using Adams--Moulton four--step method.
 *
 * @author T
 * @date 22/10/2017
 * @version 0.1.1
 * @file ABM4.h
 * @addtogroup ODE_Solver
 * @{
 */

#ifndef ABM4_H
#define ABM4_H

#include "ODE_Implicit.h"

class ABM4 final : public ODE_Implicit {
public:
    explicit ABM4(const unsigned = 0, const shared_ptr<ODE>& = nullptr, const unsigned = 20, const bool = false);

    int update_status() override;

    void print() override;
};

#endif

//! @}
