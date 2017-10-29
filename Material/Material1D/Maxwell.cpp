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

#include "Maxwell.h"
#include <Domain/DomainBase.h>
#include <Domain/Factory.hpp>
#include <Solver/ODE_Solver/DP45.h>
#include <Toolbox/utility.h>

Maxwell::Damper::Damper(const double E, const double A, const double C1, const double C2)
    : ODE(0, 1)
    , elastic_modulus(E)
    , alpha(A == 0. ? 1. : 1. / A)
    , damping_positive(C1)
    , damping_negative(C2) {}

unique_ptr<ODE> Maxwell::Damper::get_copy() { return make_unique<Damper>(*this); }

vec Maxwell::Damper::eval(const double t_time, const vec& t_stress) {
    const auto trial_strain_rate = current_strain_rate + t_time * current_strain_acceleration;

    vec t_stress_rate{ trial_strain_rate };

    const auto t_factor = t_stress(0) / trial_strain_rate > 0. ? damping_positive : damping_negative;

    t_stress_rate += suanpan::sign(t_factor) * pow(fabs(t_factor), alpha);
    t_stress_rate *= elastic_modulus;

    return t_stress_rate;
}

void Maxwell::Damper::set_current_status(const vec& t_strain_rate, const vec& t_strain_accleration) {
    current_strain_rate = t_strain_rate(0);
    current_strain_acceleration = t_strain_accleration(0);
}

Maxwell::Maxwell(const unsigned T, const double E, const double A, const double C1, const double C2)
    : Material1D(T, MT_MAXWELL, 0.)
    , viscosity(make_unique<Damper>(E, A, C1, C2))
    , solver(make_unique<DP45>(0, viscosity.get())) {}

Maxwell::Maxwell(const Maxwell& old_obj)
    : Material1D(old_obj.get_tag(), MT_MAXWELL, 0.)
    , incre_time(old_obj.incre_time)
    , viscosity(old_obj.viscosity->get_copy())
    , solver(old_obj.solver->get_copy()) {
    solver->set_ode(viscosity.get());
}

void Maxwell::initialize(const shared_ptr<DomainBase>& D) { incre_time = &D->get_factory()->get_incre_time(); }

unique_ptr<Material> Maxwell::get_copy() { return make_unique<Maxwell>(*this); }

int Maxwell::update_trial_status(const vec& t_strain, const vec& t_strain_rate) {
    trial_strain = t_strain;
    trial_strain_rate = t_strain_rate;

    damper_ptr->clear_status();
    damper_ptr->set_incre_time(*incre_time);
    damper_ptr->set_current_status(current_strain_rate, (trial_strain_rate - current_strain_rate) / *incre_time);
    damper_ptr->set_current_variable(current_stress);

    if(solver->analyze() != 0 || !damper_ptr->is_converged()) return -1;

    trial_stress = damper_ptr->get_trial_variable();

    return 0;
}

int Maxwell::clear_status() {
    trial_strain.zeros();
    current_strain.zeros();
    trial_stress.zeros();
    current_stress.zeros();
    trial_strain_rate.zeros();
    current_strain_rate.zeros();
    return 0;
}

int Maxwell::commit_status() {
    current_strain = trial_strain;
    current_stress = trial_stress;
    current_strain_rate = trial_strain_rate;
    return 0;
}

int Maxwell::reset_status() {
    trial_strain = current_strain;
    trial_stress = current_stress;
    trial_strain_rate = current_strain_rate;
    return 0;
}

void Maxwell::print() { suanpan_info("A Maxwell material model.\n"); }
