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

#include "Newmark.h"
#include <Domain/DomainBase.h>
#include <Domain/Factory.hpp>

Newmark::Newmark(const unsigned& T, const double& A, const double& B)
    : Integrator(T, CT_NEWMARK)
    , alpha(A)
    , beta(B) {
    if(beta < .5 || alpha < .25 * (.5 + beta) * (.5 + beta)) suanpan_error("Newmark() parameters are not acceptable.\n");
}

Newmark::Newmark(const double& A, const double& B)
    : Integrator(0, CT_NEWMARK)
    , alpha(A)
    , beta(B) {
    if(beta < .5 || alpha < .25 * (.5 + beta) * (.5 + beta)) suanpan_error("Newmark() parameters are not acceptable.\n");
}

Newmark::~Newmark() {}

void Newmark::assemble_resistance() {
    update_parameter();

    const auto& D = get_domain().lock();
    const auto& W = D->get_factory();

    D->assemble_resistance();

    D->assemble_mass();
    D->assemble_damping();

    get_trial_resistance(W) -= get_mass(W) * (C2 * W->get_current_velocity() + C3 * W->get_current_acceleration() - C0 * W->get_incre_displacement()) + get_damping(W) * (C4 * W->get_current_velocity() + C5 * W->get_current_acceleration() - C1 * W->get_incre_displacement());
}

void Newmark::assemble_matrix() {
    update_parameter();

    const auto& D = get_domain().lock();
    const auto& W = D->get_factory();

    D->assemble_stiffness();

    get_stiffness(W) += C0 * get_mass(W) + C1 * get_damping(W);
}

void Newmark::commit_status() const {
    const auto& D = get_domain().lock();
    const auto& W = D->get_factory();

    W->update_trial_acceleration(C0 * W->get_incre_displacement() - C2 * W->get_current_velocity() - C3 * W->get_current_acceleration());
    W->update_trial_velocity(W->get_current_velocity() + C6 * W->get_current_acceleration() + C7 * W->get_trial_acceleration());

    D->commit_status();
}

void Newmark::update_parameter() {
    const auto& NT = get_domain().lock()->get_factory()->get_incre_time();

    if(DT != NT) {
        DT = NT;
        C2 = 1. / alpha / DT;
        C0 = C2 / DT;
        C1 = C2 * beta;
        C3 = .5 / alpha - 1.;
        C4 = beta / alpha - 1.;
        C5 = (C4 - 1.) * DT / 2.;
        C7 = DT * beta;
        C6 = DT - C7;
    }
}

void Newmark::print() { suanpan_info("A Newmark solver.\n"); }
