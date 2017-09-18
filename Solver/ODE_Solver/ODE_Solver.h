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
 * @{
 */

#ifndef ODE_SOLVER_H
#define ODE_SOLVER_H

#include <Domain/Factory.hpp>
#include <Solver/Solver.h>

class ODE;

class ODE_Solver : public Solver {
    shared_ptr<ODE> ode_system = nullptr;
    shared_ptr<Factory<double>> factory = nullptr;

public:
    explicit ODE_Solver(const unsigned& = 0, const unsigned& = CT_ODESOLVER, const shared_ptr<ODE>& = nullptr, const shared_ptr<Converger>& = nullptr, const shared_ptr<Factory<double>>& = nullptr);

    int initialize() override;

    int analyze() final;

    int update_status() override = 0;

    void set_ode(const shared_ptr<ODE>&);
    const shared_ptr<ODE>& get_ode() const;

    void set_factory(const shared_ptr<Factory<double>>&);
    const shared_ptr<Factory<double>>& get_factory() const;
};

#endif

//! @}
