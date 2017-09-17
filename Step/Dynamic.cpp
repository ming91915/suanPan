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

    unsigned num_increment = 0;

    while(time_left > 0. && ++num_increment <= get_max_iteration()) {
        G->update_incre_time(step);
        const auto code = S->analyze(get_tag());
        if(code == 0) { // SUCCEEDED STEP
            G->commit_status();
            G->record();
            time_left -= step;
        } else if(code == -1) { // FAILED UNCONVERGED
            G->reset_status();
            if(step <= get_min_step_size()) {
                suanpan_error("analyze() reaches minimum step size %.3E.\n", get_min_step_size());
                return -1;
            }
            if(is_fixed_step_size()) {
                suanpan_error("analyze() does not converge for given fixed step size %.3E.\n", step);
                return -1;
            }
            step /= 2.;
        } else
            return -1;
    }

    if(num_increment > get_max_iteration()) {
        suanpan_warning("analyze() reaches maximum iteration number %u.\n", get_max_iteration());
        return -1;
    }

    return 0;
}
