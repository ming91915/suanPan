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

#include "GeneralizedAlpha.h"
#include <Domain/DomainBase.h>
#include <Domain/Factory.hpp>

GeneralizedAlpha::GeneralizedAlpha(const unsigned& T, const double& AF, const double& AM)
    : Integrator(T, CT_GENERALIZEDALPHA)
    , alpha_f(AF)
    , alpha_m(AM)
    , gamma(.5 - alpha_m + alpha_f)
    , beta(.25 * (gamma + .5) * (gamma + .5)) {
    if(alpha_m > alpha_f || alpha_f > .5) suanpan_error("GeneralizedAlpha() parameters are not acceptable.\n");

    C9 = alpha_f;
    C8 = 1. - C9;
    C4 = C8 / beta * gamma - 1.;
    C3 = (1. - alpha_m) / 2. / beta - 1.;
    C12 = 1. - .5 / beta;
}

void GeneralizedAlpha::assemble_resistance() {
    update_parameter();

    const auto& D = get_domain().lock();
    const auto& W = D->get_factory();

    D->assemble_resistance();

    D->assemble_mass();
    D->assemble_damping();

    auto& t_sushi = get_sushi(W);

    t_sushi *= C8;

    t_sushi -= get_mass(W) * (C2 * W->get_current_velocity() + C3 * W->get_current_acceleration() - C0 * W->get_incre_displacement()) + get_damping(W) * (C4 * W->get_current_velocity() + C5 * W->get_current_acceleration() - C1 * W->get_incre_displacement()) - C9 * W->get_current_resistance();
}

void GeneralizedAlpha::assemble_matrix() {
    update_parameter();

    const auto& D = get_domain().lock();
    const auto& W = D->get_factory();

    D->assemble_stiffness();

    auto& t_stiffness = get_stiffness(W);

    t_stiffness *= C8;

    C0 == 1. ? t_stiffness += get_mass(W) : C0 == -1. ? t_stiffness -= get_mass(W) : t_stiffness += C0 * get_mass(W);

    C1 == 1. ? t_stiffness += get_damping(W) : C1 == -1. ? t_stiffness -= get_damping(W) : t_stiffness += C1 * get_damping(W);
}

int GeneralizedAlpha::process_load() const {
    const auto& D = get_domain().lock();
    auto& W = D->get_factory();

    const auto current_time = W->get_current_time();
    const auto trial_time = W->get_trial_time();

    const auto new_time = C8 * trial_time + C9 * current_time;

    W->update_trial_time(new_time);

    const auto code = D->process_load();

    W->update_trial_time(trial_time);

    return code;
}

void GeneralizedAlpha::commit_status() const {
    const auto& D = get_domain().lock();
    const auto& W = D->get_factory();

    W->update_trial_acceleration(C10 * W->get_incre_displacement() + C11 * W->get_current_velocity() + C12 * W->get_current_acceleration());
    W->update_trial_velocity(W->get_current_velocity() + C6 * W->get_current_acceleration() + C7 * W->get_trial_acceleration());

    D->commit_status();
}

void GeneralizedAlpha::update_parameter() {
    const auto& NT = get_domain().lock()->get_factory()->get_incre_time();

    if(DT != NT) {
        DT = NT;
        C7 = DT * gamma;
        C6 = DT - C7;
        C11 = -1. / beta / DT;
        C10 = -C11 / DT;
        C1 = -gamma * C11 * C8;
        C2 = (alpha_m - 1.) * C11;
        C0 = C2 / DT;
        C5 = C8 * (C7 / 2. / beta - DT);
    }
}

void GeneralizedAlpha::print() { suanpan_info("A GeneralizedAlpha solver.\n"); }
