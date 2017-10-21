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

Ramm::Ramm(const unsigned& T)
    : Solver(T, CT_RAMM) {}

int Ramm::analyze() {
    auto& C = get_converger();
    auto& G = get_integrator();
    const auto& W = G->get_domain().lock()->get_factory();

    auto& max_iteration = C->get_max_iteration();

    auto& load_ref = W->get_reference_load();

    // ninja anchor
    auto& t_ninja = get_ninja(W);

    double t_lambda;

    vec disp_a, disp_ref;

    // iteration counter
    unsigned counter = 0;

    while(true) {
        // assemble resistance
        G->assemble_resistance();
        // assemble stiffness
        G->assemble_matrix();
        // process loads
        G->process_load();
        // process constraints
        G->process_constraint();

        // solve ninja
        auto flag = W->get_stiffness()->solve(t_ninja, load_ref * W->get_trial_load_factor() + W->get_trial_load() - W->get_sushi());
        // make sure lapack solver succeeds
        if(flag != 0) return flag;
        // solve reference displacement
        flag = W->get_stiffness()->solve_trs(disp_a, load_ref);
        // make sure lapack solver succeeds
        if(flag != 0) return flag;

        if(counter == 0) {
            t_lambda = arc_length / sqrt(dot(disp_a, disp_a) + 1.);

            // check the sign of stiffness for unloading
            const auto& t_stiff = get_stiffness(W);
            const auto& t_pivot = t_stiff.IPIV;
            auto det_sign = 1;
            for(auto I = 0; I < t_pivot.n_elem; ++I) {
                if(t_stiff(I, I) < 0) det_sign = -det_sign;
                if(I + 1 != t_pivot(I)) det_sign = -det_sign;
            }
            if(det_sign < 0) t_lambda = -t_lambda;
        } else
            t_lambda = -dot(disp_ref, t_ninja) / dot(disp_ref, disp_a);

        // abaqus update
        disp_ref = disp_a;

        t_ninja += disp_a * t_lambda;

        // avoid machine error accumulation
        G->erase_machine_error();
        // update trial displacement
        W->update_trial_displacement(W->get_trial_displacement() + t_ninja);
        // update trial load factor
        W->update_trial_load_factor(W->get_trial_load_factor() + t_lambda);
        // set time to load factor
        W->update_trial_time(W->get_trial_load_factor().at(0));
        // update for nodes and elements
        if(G->update_trial_status() != 0) return -1;

        ++counter;

        // exit if converged
        if(C->is_converged()) {
            if(!fixed_arc_length) arc_length *= sqrt(max_iteration / double(counter));
            return 0;
        }
        // exit if maximum iteration is hit
        if(counter > max_iteration) {
            if(!fixed_arc_length) arc_length *= .5;
            return -1;
        }
    }
}

void Ramm::print() { suanpan_info("A solver using Ramm's arc length method.\n"); }
