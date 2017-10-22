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

#include "ODE_Explicit.h"
#include <Solver/ODE_Solver/ODE.h>

ODE_Explicit::ODE_Explicit(const unsigned& T, const unsigned& CT, const shared_ptr<ODE>& D)
    : ODE_Solver(T, CT, D) {
    if(CT == CT_BS23 || CT == CT_RK23) factor = 1. / 3.;
}

ODE_Explicit::~ODE_Explicit() {}

int ODE_Explicit::analyze() {
    const auto& D = get_ode();

    auto time_left = D->get_incre_time();
    auto step = time_left;
    auto counter = 0;

    while(true) {
        if(update_status() != 0) return -1;
        if(D->is_converged()) {
            D->commit_status();
            time_left -= step;
        }
        if(time_left <= 0.) return 0;
        if(++counter > 20) return -1;
        step *= .8 * pow(D->get_tolerance() / D->get_error(), factor);
        if(step > time_left) step = time_left;
        D->update_incre_time(step);
    }
}
