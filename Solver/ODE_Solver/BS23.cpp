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

#include "BS23.h"
#include <Solver/ODE_Solver/ODE.h>

BS23::BS23(const unsigned T)
    : ODE_Explicit(T, CT_BS23) {}

unique_ptr<ODE_Solver> BS23::get_copy() { return make_unique<BS23>(*this); }

int BS23::update_status() {
    const auto c_time = ode_system->get_current_time();
    const auto t_time = ode_system->get_trial_time();
    const auto i_time = ode_system->get_incre_time();

    auto& c_disp = ode_system->get_current_variable();

    S1 = ode_system->eval(c_time, c_disp);
    S2 = ode_system->eval(c_time + .5 * i_time, c_disp + i_time * .5 * S1);
    S3 = ode_system->eval(c_time + .75 * i_time, c_disp + i_time * .75 * S2);

    ode_system->set_incre_variable(i_time * (2. * S1 + 3. * S2 + 4. * S3) / 9.);

    S4 = ode_system->eval(t_time, ode_system->get_trial_variable());

    ode_system->set_error(norm(i_time * (-5. * S1 + 6. * S2 + 8. * S3 - 9. * S4) / 72.));

    return 0;
}

void BS23::print() { suanpan_info("A Bogacki-Shampine 2/3 Pair Solver object.\n"); }
