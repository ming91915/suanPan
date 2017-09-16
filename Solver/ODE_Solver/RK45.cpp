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

RK45::RK45(const unsigned& T, const shared_ptr<ODE>& D, const shared_ptr<Converger>& C, const shared_ptr<Factory<double>>& W)
    : ODE_Solver(T, CT_RK45, D, C, W) {}

RK45::RK45(const shared_ptr<ODE>& D, const shared_ptr<Converger>& C, const shared_ptr<Factory<double>>& W)
    : ODE_Solver(0, CT_RK45, D, C, W) {}

int RK45::update_status() {
    auto& D = get_ode();
    auto& W = get_factory();

    S1 = D->eval(W->get_current_time(), W->get_current_displacement());
    S2 = D->eval(W->get_current_time() + F[0] * W->get_incre_time(), W->get_current_displacement() + W->get_incre_time() * F[0] * S1);
    S3 = D->eval(W->get_current_time() + F[1] * W->get_incre_time(), W->get_current_displacement() + W->get_incre_time() * (F[2] * S1 + F[3] * S2));
    S4 = D->eval(W->get_current_time() + F[4] * W->get_incre_time(), W->get_current_displacement() + W->get_incre_time() * (F[5] * S1 + F[6] * S2 + F[7] * S3));
    S5 = D->eval(W->get_trial_time(), W->get_current_displacement() + W->get_incre_time() * (F[8] * S1 + F[9] * S2 + F[10] * S3 + F[11] * S4));
    S6 = D->eval(W->get_current_time() + F[12] * W->get_incre_time(), W->get_current_displacement() + W->get_incre_time() * (F[13] * S1 + F[14] * S2 + F[15] * S3 + F[16] * S4 + F[17] * S5));

    W->update_incre_displacement(W->get_incre_time() * (F[18] * S1 + F[19] * S3 + F[20] * S4 + F[21] * S5 + F[22] * S6));

    W->set_error(norm(W->get_incre_time() * (F[23] * S1 + F[24] * S3 + F[25] * S4 + F[26] * S5 + F[27] * S6)));

    return 0;
}

void RK45::print() { suanpan_info("A Runge--Kutta--Fehlberg 4/5 Solver Object.\n"); }
