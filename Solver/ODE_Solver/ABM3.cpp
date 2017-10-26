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

#include "ABM3.h"
#include <Solver/ODE_Solver/ODE.h>

ABM3::ABM3(const unsigned T, const unsigned N, const bool C, ODE* O)
    : ODE_Implicit(T, CT_ABM3, N, C, O) {}

unique_ptr<ODE_Solver> ABM3::get_copy() { return make_unique<ABM3>(*this); }

int ABM3::update_status() {
    history_step.clear();

    const auto step_size = ode_system->get_incre_time() / double(step_number);

    auto c_time = ode_system->get_current_time();
    auto c_disp = ode_system->get_current_variable();

    history_step.emplace_back(ode_system->eval(c_time, c_disp));

    c_time += step_size;

    history_step.emplace_back(ode_system->eval(c_time, c_disp += .5 * step_size * (history_step[0] + ode_system->eval(c_time, c_disp + step_size * history_step[0]))));

    history_step.emplace_back(ode_system->eval(c_time += step_size, c_disp += step_size * (1.5 * history_step[1] - .5 * history_step[0])));

    vec i_disp = step_size / 12. * (23. * history_step[2] - 16. * history_step[1] + 5. * history_step[0]);

    unsigned counter = 3;

    while(counter++ <= step_number) {
        history_step.emplace_back(ode_system->eval(c_time += step_size, c_disp += use_corrector ? step_size / 12. * (5. * ode_system->eval(c_time + step_size, c_disp + i_disp) + 8. * history_step[2] - history_step[1]) : i_disp));
        history_step.pop_front();
        i_disp = step_size / 12. * (23. * history_step[2] - 16. * history_step[1] + 5. * history_step[0]);
    }

    ode_system->set_trial_variable(c_disp);

    return 0;
}

void ABM3::print() { suanpan_info("A Bogacki--Shampine 2/3 Pair Solver object.\n"); }
