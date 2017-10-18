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

#include "Damper01.h"
#include <Domain/DomainBase.h>
#include <Domain/Factory.hpp>

Damper01::Damper::Damper(const double D, const double A)
    : damping(D)
    , alpha(A) {}

vec Damper01::Damper::eval(const double time, const vec& displacement) {
    vec response(1);

    return response;
}

Damper01::Damper01(const unsigned T, const double D, const double A)
    : Material1D(T, MT_DAMPER01, 0.)
    , ode_system(D, A) {}

void Damper01::initialize(const shared_ptr<DomainBase>& D) {
    auto& t_factory = D->get_factory();

    current_time_anchor = &t_factory->get_current_time();
    incre_time_anchor = &t_factory->get_incre_time();
    trial_time_anchor = &t_factory->get_trial_time();
}

unique_ptr<Material> Damper01::get_copy() { return make_unique<Damper01>(*this); }

int Damper01::update_trial_status(const vec&) { return 0; }

int Damper01::clear_status() { return 0; }

int Damper01::commit_status() { return 0; }

int Damper01::reset_status() { return 0; }

void Damper01::print() {}
