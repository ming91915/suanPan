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

DP45::DP45(const unsigned& T, const shared_ptr<ODE>& D, const shared_ptr<Converger>& C, const shared_ptr<Factory<double>>& W)
    : ODE_Solver(T, CT_DP45, D, C, W) {}

DP45::DP45(const shared_ptr<ODE>& D, const shared_ptr<Converger>& C, const shared_ptr<Factory<double>>& W)
    : ODE_Solver(0, CT_DP45, D, C, W) {}

int DP45::update_status() {
    auto& D = get_ode();
    auto& W = get_factory();

    S1 = D->eval(W->get_current_time(), W->get_current_displacement());
    S2 = D->eval(W->get_current_time() + F[0] * W->get_incre_time(), W->get_current_displacement() + W->get_incre_time() * F[0] * S1);
    S3 = D->eval(W->get_current_time() + F[1] * W->get_incre_time(), W->get_current_displacement() + W->get_incre_time() * (F[2] * S1 + F[3] * S2));
    S4 = D->eval(W->get_current_time() + F[4] * W->get_incre_time(), W->get_current_displacement() + W->get_incre_time() * (F[5] * S1 + F[6] * S2 + F[7] * S3));
    S5 = D->eval(W->get_current_time() + F[8] * W->get_incre_time(), W->get_current_displacement() + W->get_incre_time() * (F[9] * S1 + F[10] * S2 + F[11] * S3 + F[12] * S4));
    S6 = D->eval(W->get_trial_time(), W->get_current_displacement() + W->get_incre_time() * (F[13] * S1 + F[14] * S2 + F[15] * S3 + F[16] * S4 + F[17] * S5));

    W->update_incre_displacement(W->get_incre_time() * (F[18] * S1 + F[19] * S3 + F[20] * S4 + F[21] * S5 + F[22] * S6));

    S7 = D->eval(W->get_trial_time(), W->get_trial_displacement());

    W->set_error(norm(W->get_incre_time() * (F[23] * S1 + F[24] * S3 + F[25] * S4 + F[26] * S5 + F[27] * S6 + F[28] * S7)));

    return 0;
}

void DP45::print() { suanpan_info("A Dormand--Prince 4/5 Pair Solver object.\n"); }
