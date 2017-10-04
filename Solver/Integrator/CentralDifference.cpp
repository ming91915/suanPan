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

#include "CentralDifference.h"
#include <Domain/DomainBase.h>
#include <Domain/Factory.hpp>
#include <Domain/Node.h>

CentralDifference::CentralDifference(const unsigned& T)
    : Integrator(T, CT_CENTRALDIFFERENCE) {}

int CentralDifference::initialize() {
    const auto code = Integrator::initialize();

    if(code == 0) {
        const auto& W = get_domain().lock()->get_factory();

        // const auto eig_val = eig_sym(W->get_mass().i() * W->get_stiffness());

        // max_dt = datum::pi / sqrt(eig_val.max());
    }

    return code;
}

void CentralDifference::update_parameter() {
    const auto& W = get_domain().lock()->get_factory();

    if(DT != W->get_incre_time() || W->get_pre_displacement().is_empty()) {
        DT = W->get_incre_time();
        if(DT > max_dt) suanpan_error("update_status() requires a smaller time increment.\n");

        C0 = 1. / DT / DT;
        C1 = .5 / DT;
        C2 = 2. * C0;
        C3 = 1. / C2;

        W->set_pre_displacement(W->get_current_displacement() - DT * W->get_current_velocity() + C3 * W->get_current_acceleration());
    }
}

void CentralDifference::assemble_resistance() {
    update_parameter();

    const auto& D = get_domain().lock();
    const auto& W = D->get_factory();

    D->assemble_resistance();

    get_trial_resistance(W) += (get_stiffness(W) - C2 * get_mass(W)) * W->get_current_displacement() + (C0 * get_mass(W) - C1 * get_damping(W)) * W->get_pre_displacement();
}

void CentralDifference::assemble_matrix() {
    update_parameter();

    const auto& D = get_domain().lock();
    const auto& W = D->get_factory();

    D->assemble_mass();
    D->assemble_stiffness();
    D->assemble_damping();

    get_stiffness(W) = C0 * get_mass(W) + C1 * get_damping(W);
}

void CentralDifference::commit_status() const {
    const auto& D = get_domain().lock();
    const auto& W = D->get_factory();

    W->set_current_velocity(C1 * (W->get_trial_displacement() - W->get_pre_displacement()));

    W->set_current_acceleration(C0 * (W->get_pre_displacement() - 2 * W->get_current_displacement() + W->get_trial_displacement()));

    D->commit_status();

    auto& tmp_velocity = W->get_current_velocity();
    auto& tmp_acceleration = W->get_current_acceleration();
    for(const auto& I : D->get_node_pool()) {
        I->set_current_velocity(tmp_velocity(I->get_reordered_dof()));
        I->set_current_acceleration(tmp_acceleration(I->get_reordered_dof()));
    }
}
