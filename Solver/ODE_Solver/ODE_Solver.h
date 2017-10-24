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
 * @class ODE_Solver
 * @brief A ODE_Solver class.
 *
 * The ODE_Solver object stores ODE system status and calls an ODE object
 * to get trial
 * status.
 *
 * @author T
 * @date 16/07/2017
 * @version 0.1.0
 * @file ODE_Solver.h
 * @addtogroup ODE_Solver
 * @ingroup Solver
 * @{
 */

#ifndef ODE_SOLVER_H
#define ODE_SOLVER_H

#include <Domain/Tag.h>

class ODE;

class ODE_Solver : public Tag {
protected:
    ODE* ode_system = nullptr;

public:
    explicit ODE_Solver(const unsigned = 0, const unsigned = CT_ODESOLVER);

    virtual ~ODE_Solver();

    int initialize() const;

    virtual unique_ptr<ODE_Solver> get_copy() = 0;

    virtual int analyze() = 0;

    virtual int update_status() = 0;

    void set_ode(ODE*);
    ODE* get_ode() const;
};

namespace suanpan {
unique_ptr<ODE_Solver> make_copy(const unique_ptr<ODE_Solver>&);
}

#endif

//! @}
