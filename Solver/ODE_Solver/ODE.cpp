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

#include "ODE.h"

ODE::ODE(const unsigned T, const unsigned CT, const unsigned D)
    : Tag(T, CT)
    , trial_displacement(D, fill::zeros)
    , incre_displacement(D, fill::zeros)
    , current_displacement(D, fill::zeros)
    , n_size(D) {}

vec ODE::eval(const double, const vec&) { throw; }

vec ODE::operator()(const double t_time, const vec& t_displacement) { return eval(t_time, t_displacement); }

bool ODE::is_converged() const { return error < tolerance; }

double ODE::get_error() const { return error; }

double ODE::get_tolerance() const { return tolerance; }

void ODE::set_error(const double E) { error = E; }

void ODE::set_tolerance(const double T) { tolerance = T; }

double ODE::get_trial_time() const { return trial_time; }

double ODE::get_incre_time() const { return incre_time; }

double ODE::get_current_time() const { return current_time; }

const vec& ODE::get_trial_displacement() const { return trial_displacement; }

const vec& ODE::get_incre_displacement() const { return incre_displacement; }

const vec& ODE::get_current_displacement() const { return current_displacement; }

void ODE::update_incre_time(const double i_time) {
    incre_time = i_time;
    trial_time = current_time + incre_time;
}

void ODE::update_trial_time(const double t_time) {
    trial_time = t_time;
    incre_time = trial_time - current_time;
}

void ODE::update_incre_displacement(const vec& i_displacement) {
    incre_displacement = i_displacement;
    trial_displacement = current_displacement + incre_displacement;
}

void ODE::update_trial_displacement(const vec& t_displacement) {
    trial_displacement = t_displacement;
    incre_displacement = trial_displacement - current_displacement;
}

void ODE::commit_status() {
    current_time = trial_time;
    incre_time = 0.;
    current_displacement = trial_displacement;
    incre_displacement.zeros();
}

void ODE::clear_status() {
    trial_time = incre_time = current_time = 0.;
    trial_displacement.zeros();
    incre_displacement.zeros();
    current_displacement.zeros();
}

void ODE::reset_status() {
    trial_time = current_time;
    incre_time = 0.;
    trial_displacement = current_displacement;
    incre_displacement.zeros();
}
