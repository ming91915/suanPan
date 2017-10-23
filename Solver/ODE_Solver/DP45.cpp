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

#include "DP45.h"
#include <Solver/ODE_Solver/ODE.h>
#include <array>

const std::array<double, 29> DP45::F = { .2, .3, .075, .225, .8, 44. / 45., -56. / 15., 32. / 9., 8. / 9., 19372. / 6561., -25360. / 2187., 64448. / 6561., -212. / 729., 9017. / 3168., -355. / 33., 46732. / 5247., 49. / 176., -5103. / 18656., 35. / 384., 500. / 1113., 125. / 192., -2187. / 6784., 11. / 84., 71. / 57600., -71. / 16695., 71. / 1920., -17253. / 339200., 22. / 525., -1. / 40. };

DP45::DP45(const unsigned T)
    : ODE_Explicit(T, CT_DP45) {}

unique_ptr<ODE_Solver> DP45::get_copy() { return make_unique<DP45>(*this); }

int DP45::update_status() {
    const auto c_time = ode_system->get_current_time();
    const auto t_time = ode_system->get_trial_time();
    const auto i_time = ode_system->get_incre_time();

    auto& c_disp = ode_system->get_current_variable();

    S1 = ode_system->eval(c_time, c_disp);
    S2 = ode_system->eval(c_time + F[0] * i_time, c_disp + i_time * F[0] * S1);
    S3 = ode_system->eval(c_time + F[1] * i_time, c_disp + i_time * (F[2] * S1 + F[3] * S2));
    S4 = ode_system->eval(c_time + F[4] * i_time, c_disp + i_time * (F[5] * S1 + F[6] * S2 + F[7] * S3));
    S5 = ode_system->eval(c_time + F[8] * i_time, c_disp + i_time * (F[9] * S1 + F[10] * S2 + F[11] * S3 + F[12] * S4));
    S6 = ode_system->eval(t_time, c_disp + i_time * (F[13] * S1 + F[14] * S2 + F[15] * S3 + F[16] * S4 + F[17] * S5));

    ode_system->set_incre_variable(i_time * (F[18] * S1 + F[19] * S3 + F[20] * S4 + F[21] * S5 + F[22] * S6));

    S7 = ode_system->eval(t_time, ode_system->get_trial_variable());

    ode_system->set_error(norm(i_time * (F[23] * S1 + F[24] * S3 + F[25] * S4 + F[26] * S5 + F[27] * S6 + F[28] * S7)));

    return 0;
}

void DP45::print() { suanpan_info("A Dormand--Prince 4/5 Pair Solver object.\n"); }
