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

#include "ABM4.h"
#include <Solver/ODE_Solver/ODE.h>

ABM4::ABM4(const unsigned T, const shared_ptr<ODE>& D, const unsigned N, const bool C)
    : ODE_Implicit(T, CT_ABM4, D, N, C) {}

unique_ptr<ODE_Solver> ABM4::get_copy() { return make_unique<ABM4>(*this); }

int ABM4::update_status() {
    history_step.clear();

    auto& D = get_ode();

    const auto step_size = D->get_incre_time() / double(step_num);

    auto c_time = D->get_current_time();
    auto c_disp = D->get_current_variable();

    history_step.emplace_back(D->eval(c_time, c_disp));

    c_time += step_size;

    history_step.emplace_back(D->eval(c_time, c_disp += .5 * step_size * (history_step[0] + D->eval(c_time, c_disp + step_size * history_step[0]))));

    history_step.emplace_back(D->eval(c_time += step_size, c_disp += step_size * (1.5 * history_step[1] - .5 * history_step[0])));

    history_step.emplace_back(D->eval(c_time += step_size, c_disp += step_size / 12. * (23. * history_step[2] - 16. * history_step[1] + 5. * history_step[0])));

    vec i_disp = step_size / 24. * (55 * history_step[3] - 59. * history_step[2] + 37. * history_step[1] - 9. * history_step[0]);

    unsigned counter = 4;

    while(counter++ <= step_num) {
        history_step.emplace_back(D->eval(c_time += step_size, c_disp += use_corrector ? step_size / 24. * (9. * D->eval(c_time + step_size, c_disp + i_disp) + 19. * history_step[3] - 5. * history_step[2] + history_step[1]) : i_disp));
        history_step.pop_front();
        i_disp = step_size / 24. * (55. * history_step[3] - 59. * history_step[2] + 37. * history_step[1] - 9. * history_step[0]);
    }

    D->update_trial_variable(c_disp);

    return 0;
}

void ABM4::print() { suanpan_info("A Bogacki--Shampine 2/3 Pair Solver object.\n"); }
