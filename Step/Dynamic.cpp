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

#include "Dynamic.h"
#include <Solver/Integrator/Integrator.h>
#include <Solver/Solver.h>

Dynamic::Dynamic(const unsigned& T, const double& P)
    : Step(T, CT_DYNAMIC, P) {}

int Dynamic::analyze() {
    auto& S = get_solver();
    auto& G = get_integrator();

    auto time_left = get_time_period();
    auto step = get_ini_step_size();

    unsigned num_increment = 0, num_converged_step = 0;

    while(true) {
        // check if the target time point is hit
        if(time_left <= 1E-14) return 0;
        // check if the maximum substep number is hit
        if(++num_increment > get_max_substep()) {
            suanpan_warning("analyze() reaches maximum substep number %u.\n", get_max_substep());
            return -1;
        }
        // update incremental and trial time
        G->update_incre_time(step);
        // call solver
        const auto code = S->analyze();
        if(code == 0) { // success step
            // commit converged iteration
            G->commit_status();
            // record response
            G->record();
            // eat current increment
            time_left -= step;
            if(!is_fixed_step_size() && ++num_converged_step > 5) {
                step *= 1.2;
                num_converged_step = 0;
            }
            // check if time overflows
            if(step > time_left) step = time_left;
        } else if(code == -1) { // failed step
            // reset to the start of current substep
            G->reset_status();
            // check if minimum step size is hit
            if(step <= get_min_step_size()) {
                suanpan_error("analyze() reaches minimum step size %.3E.\n", get_min_step_size());
                return -1;
            }
            // check if fixed step size
            if(is_fixed_step_size()) {
                suanpan_error("analyze() does not converge for given fixed step size %.3E.\n", step);
                return -1;
            }
            // step size is allowed to decrease
            step /= 2.;
        } else // positive codes are from lapack subroutines
            return -1;
    }
}
