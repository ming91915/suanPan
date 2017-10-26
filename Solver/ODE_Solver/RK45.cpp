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

#include "RK45.h"
#include <Solver/ODE_Solver/ODE.h>
#include <array>

const std::array<double, 28> RK45::F = { .25, .375, .09375, .28125, 12. / 13., 1932. / 2197., -7200. / 2197., 7296. / 2197., 439. / 216., -8., 3680. / 513., -845. / 4104., .5, -8. / 27., 2., -3544. / 2565., 1859. / 4104., -11. / 40., 16. / 135., 6656. / 12825., 28561. / 56430., -9. / 50., 2. / 55., 1. / 360., -128. / 4275., -2197. / 75240., 1. / 50., 2. / 55. };

RK45::RK45(const unsigned T, ODE* O)
    : ODE_Explicit(T, CT_RK45, O) {}

unique_ptr<ODE_Solver> RK45::get_copy() { return make_unique<RK45>(*this); }

int RK45::update_status() {
    const auto c_time = ode_system->get_current_time();
    const auto t_time = ode_system->get_trial_time();
    const auto i_time = ode_system->get_incre_time();

    auto& c_disp = ode_system->get_current_variable();

    S1 = ode_system->eval(c_time, c_disp);
    S2 = ode_system->eval(c_time + F[0] * i_time, c_disp + i_time * F[0] * S1);
    S3 = ode_system->eval(c_time + F[1] * i_time, c_disp + i_time * (F[2] * S1 + F[3] * S2));
    S4 = ode_system->eval(c_time + F[4] * i_time, c_disp + i_time * (F[5] * S1 + F[6] * S2 + F[7] * S3));
    S5 = ode_system->eval(t_time, c_disp + i_time * (F[8] * S1 + F[9] * S2 + F[10] * S3 + F[11] * S4));
    S6 = ode_system->eval(c_time + F[12] * i_time, c_disp + i_time * (F[13] * S1 + F[14] * S2 + F[15] * S3 + F[16] * S4 + F[17] * S5));

    ode_system->set_incre_variable(i_time * (F[18] * S1 + F[19] * S3 + F[20] * S4 + F[21] * S5 + F[22] * S6));

    ode_system->set_error(norm(i_time * (F[23] * S1 + F[24] * S3 + F[25] * S4 + F[26] * S5 + F[27] * S6)));

    return 0;
}

void RK45::print() { suanpan_info("A Runge--Kutta--Fehlberg 4/5 Solver Object.\n"); }
