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
 * @class ODE_Implicit
 * @brief A ODE_Implicit class.
 *
 * The ODE_Implicit object stores ODE system status and calls an ODE object
 * to get trial
 * status.
 *
 * @author T
 * @date 22/10/2017
 * @version 0.1.0
 * @file ODE_Implicit.h
 * @addtogroup ODE_Implicit
 * @ingroup ODE_Solver
 * @{
 */

#ifndef ODE_IMPLICIT_H
#define ODE_IMPLICIT_H

#include <Solver/ODE_Solver/ODE_Solver.h>
#include <deque>

using std::deque;

class ODE_Implicit : public ODE_Solver {
protected:
    const unsigned step_num;

    const bool use_corrector;

    deque<vec> history_step;

public:
    explicit ODE_Implicit(const unsigned = 0, const unsigned = CT_ODESOLVER, const shared_ptr<ODE>& = nullptr, const unsigned = 20, const bool = false);

    virtual ~ODE_Implicit();

    int analyze() override;
};

#endif

//! @}
