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

#include "Acceleration.h"
#include <Domain/DomainBase.h>
#include <Domain/Factory.hpp>
#include <Domain/Node.h>
#include <Load/Amplitude/Amplitude.h>

Acceleration::Acceleration(const unsigned& T, const unsigned& ST, const double& L, const uvec& N, const unsigned& D, const unsigned& AT)
    : Load(T, CT_ACCELERATION, ST, AT, N, uvec{ D }, L) {}

Acceleration::Acceleration(const unsigned& T, const unsigned& ST, const double& L, const uvec& N, const uvec& D, const unsigned& AT)
    : Load(T, CT_ACCELERATION, ST, AT, N, D, L) {}

int Acceleration::process(const shared_ptr<DomainBase>& D) {
    const auto& t_step = D->get_current_step_tag();
    if(t_step < start_step || t_step >= end_step) return 0;

    const auto& t_factory = D->get_factory();

    vec ref_acc(t_factory->get_size(), fill::zeros);
    for(const auto& I : nodes) {
        auto& t_node = D->get_node(unsigned(I));
        if(t_node != nullptr && t_node->is_active()) {
            auto& t_dof = t_node->get_reordered_dof();
            for(const auto& J : dofs)
                if(J <= t_dof.n_elem) ref_acc(t_dof(J - 1)) = 1.;
        }
    }

    const vec ref_load = get_mass(t_factory) * ref_acc;

    t_factory->update_trial_load(t_factory->get_trial_load() - ref_load * pattern * magnitude->get_amplitude(t_factory->get_trial_time()));

    return 0;
}
