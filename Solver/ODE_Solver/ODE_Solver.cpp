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
#include <Converger/Converger.h>
#include <Domain/Factory.hpp>
#include <Solver/ODE_Solver/ODE.h>

ODE_Solver::ODE_Solver(const unsigned& T, const unsigned& CT)
    : Solver(T, CT) {}

int ODE_Solver::initialize() {
    if(ode_system == nullptr) {
        suanpan_error("initialize() needs a valid ODE.\n");
        return -1;
    }

    auto& ode_size = ode_system->get_dimension();

    if(factory == nullptr)
        factory = make_shared<Factory<double>>(ode_size, AnalysisType::DISP);
    else if(ode_size != factory->get_size()) {
        factory->set_size(ode_size);
        factory->set_analysis_type(AnalysisType::DISP);
    }

    factory->initialize_displacement();

    auto& tmp_converger = get_converger();
    if(tmp_converger == nullptr) {
        suanpan_error("initialize() needs a valid Converger.\n");
        return -1;
    }

    return 0;
}

int ODE_Solver::update_status() { return -1; }

int ODE_Solver::analyze() {
    auto& tmp_converger = get_converger();

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

    auto time_left = factory->get_incre_time();
    auto step = time_left;

    while(time_left > 0.) {
        factory->update_incre_time(step);
        if(update_status() != 0) return -1;
        if(tmp_converger->is_converged()) {
            factory->commit_status();
            time_left -= step;
        }
        step *= .8 * pow(tmp_converger->get_tolerance() / tmp_converger->get_error(), factor);
        if(step > time_left) step = time_left;
    }

    return 0;
}

void ODE_Solver::set_ode(const shared_ptr<ODE>& E) { ode_system = E; }

const shared_ptr<ODE>& ODE_Solver::get_ode() const { return ode_system; }

void ODE_Solver::set_factory(const shared_ptr<Factory<double>>& W) { factory = W; }

const shared_ptr<Factory<double>>& ODE_Solver::get_factory() const { return factory; }
