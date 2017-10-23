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

#include "RK23.h"
#include <Solver/ODE_Solver/ODE.h>

RK23::RK23(const unsigned& T, const shared_ptr<ODE>& O)
    : ODE_Explicit(T, CT_RK23, O) {}

unique_ptr<ODE_Solver> RK23::get_copy() { return make_unique<RK23>(*this); }

int RK23::update_status() {
    auto& D = get_ode();

    const auto c_time = D->get_current_time();
    const auto t_time = D->get_trial_time();
    const auto i_time = D->get_incre_time();

    auto& c_disp = D->get_current_variable();

    S1 = D->eval(c_time, c_disp);
    S2 = D->eval(t_time, c_disp + i_time * S1);
    S3 = D->eval(c_time + .5 * i_time, c_disp + i_time * .25 * (S1 + S2));

    D->update_incre_variable(i_time * (S1 + S2 + 4. * S3) / 6.);

    D->set_error(norm(i_time * (S1 + S2 - 2. * S3) / 3.));

    return 0;
}

void RK23::print() { suanpan_info("A Runge--Kutta 2/3 Solver Object.\n"); }
