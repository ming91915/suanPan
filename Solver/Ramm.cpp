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

#include "Ramm.h"
#include <Converger/Converger.h>
#include <Domain/DomainBase.h>
#include <Domain/Factory.hpp>
#include <Solver/Integrator/Integrator.h>

Ramm::Ramm(const unsigned& T, const shared_ptr<Converger>& C, const shared_ptr<Integrator>& G)
    : Solver(T, CT_RAMM, C, G) {}

int Ramm::analyze() {
    auto& C = get_converger();
    auto& G = get_integrator();
    const auto& W = G->get_domain()->get_factory().lock();

    auto& max_iteration = C->get_max_iteration();

    // ninja anchor
    auto& t_ninja = get_ninja(W);

    auto load_ref = W->get_current_load();
    for(auto& I : load_ref)
        if(I != 0.) I = 1.;

    double incre_lambda;

    vec disp_a, disp_ref;

    // iteration counter
    unsigned counter = 0;

    while(true) {
        // assemble resistance
        G->assemble_resistance();
        // assemble stiffness
        G->assemble_stiffness();
        // process constraints and loads
        G->process();

        // solve ninja
        auto flag = W->get_stiffness()->solve(t_ninja, load_ref * W->get_trial_time() - W->get_trial_resistance());
        // make sure lapack solver succeeds
        if(flag != 0) return flag;
        // solve reference displacement
        flag = W->get_stiffness()->solve_trs(disp_a, load_ref);
        // make sure lapack solver succeeds
        if(flag != 0) return flag;

        if(counter == 0) {
            incre_lambda = arc_length / sqrt(dot(disp_a, disp_a) + 1.);

            // check the sign of stiffness
            const auto& t_stiff = get_stiffness(W);
            for(unsigned I = 0; I < t_stiff.n_cols; ++I)
                if(t_stiff(I, I) < 0.) {
                    incre_lambda = -incre_lambda;
                    break;
                }
        } else
            incre_lambda = -dot(disp_ref, t_ninja) / dot(disp_ref, disp_a);

        // abaqus update
        disp_ref = disp_a;

        t_ninja += disp_a * incre_lambda;

        // avoid machine error accumulation
        G->erase_machine_error();
        // update trial load factor
        W->update_trial_time(W->get_trial_time() + incre_lambda);
        // update trial displacement
        W->update_trial_displacement(W->get_trial_displacement() + t_ninja);
        // update for nodes and elements
        G->update_trial_status();

        // exit if converged
        if(C->if_converged()) {
            arc_length *= sqrt(target_iteration / double(counter));
            return 0;
        }
        // exit if maximum iteration is hit
        if(++counter > max_iteration) {
            arc_length *= sqrt(target_iteration / double(counter));
            return -1;
        }
    }
}

void Ramm::print() { suanpan_info("A solver using Ramm's arc length method.\n"); }
