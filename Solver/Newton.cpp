////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2017 Theodore Chang
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////

#include "Newton.h"
#include <Converger/Converger.h>
#include <Domain/DomainBase.h>
#include <Domain/Factory.hpp>
#include <Solver/Integrator/Integrator.h>

Newton::Newton(const unsigned& T, const shared_ptr<Converger>& C, const shared_ptr<Integrator>& G)
    : Solver(T, CT_NEWTON, C, G) {}

int Newton::analyze() {
    auto& C = get_converger();
    auto& G = get_integrator();
    const auto& W = G->get_domain()->get_factory().lock();

    // suanpan_info("current step time: %.3E\n", W->get_trial_time());

    auto& max_iteration = C->get_max_iteration();

    // iteration counter
    unsigned counter = 0;

    while(true) {
        // assemble resistance
        G->assemble_resistance();
        // assemble stiffness
        G->assemble_stiffness();
        // process constraints and loads
        G->process();

        // call solver
        const auto flag = W->get_stiffness()->solve(get_ninja(W), W->get_trial_load() - W->get_trial_resistance());
        // make sure lapack solver succeeds
        if(flag != 0) return flag;

        // avoid machine error accumulation
        G->erase_machine_error();
        // update trial status for factory
        W->update_trial_displacement(W->get_trial_displacement() + W->get_ninja());
        // update for nodes and elements
        G->update_trial_status();

        // exit if converged
        if(C->if_converged()) return 0;
        // exit if maximum iteration is hit
        if(++counter > max_iteration) return -1;
    }
}

void Newton::print() { suanpan_info("A solver using Newton--Raphson iteration method.\n"); }
