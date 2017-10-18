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

#include "ODE_Solver.h"
#include <Solver/ODE_Solver/ODE.h>

ODE_Solver::ODE_Solver(const unsigned& T, const unsigned& CT, const shared_ptr<ODE>& O)
    : Tag(T, CT)
    , ode_system(O) {}

int ODE_Solver::initialize() const {
    if(ode_system == nullptr) {
        suanpan_error("initialize() needs a valid ODE.\n");
        return -1;
    }

    return 0;
}

int ODE_Solver::update_status() { throw; }

int ODE_Solver::analyze() {
    auto factor = .2;

    switch(get_class_tag()) {
    case CT_BS23:
    case CT_RK23:
        factor = 1. / 3.;
        break;
    case CT_RK45:
    case CT_DP45:
    default:
        break;
    }

    auto time_left = ode_system->get_incre_time();
    auto step = time_left;

    while(time_left > 0.) {
        ode_system->update_incre_time(step);
        if(update_status() != 0) return -1;
        if(ode_system->is_converged()) {
            ode_system->commit_status();
            time_left -= step;
        }
        step *= .8 * pow(ode_system->get_tolerance() / ode_system->get_error(), factor);
        if(step > time_left) step = time_left;
    }

    return 0;
}

void ODE_Solver::set_ode(const shared_ptr<ODE>& E) { ode_system = E; }

const shared_ptr<ODE>& ODE_Solver::get_ode() const { return ode_system; }
