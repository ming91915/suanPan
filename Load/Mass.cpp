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

#include "Mass.h"
#include <Domain/DomainBase.h>
#include <Domain/Factory.hpp>
#include <Domain/Node.h>

Mass::Mass(const unsigned& T, const unsigned& S, const double& L, const uvec& N, const unsigned& D)
    : Load(T, CT_MASS, S, 0, N, uvec{ D }, L) {}

int Mass::process(const shared_ptr<DomainBase>& D) {
    const auto& t_step = D->get_current_step_tag();
    if(t_step < start_step || t_step >= end_step) return 0;

    auto& t_mass = D->get_factory()->get_mass();

    for(const auto& I : nodes) {
        auto& t_node = D->get_node(unsigned(I));
        if(t_node != nullptr && t_node->is_active()) {
            auto& t_dof = t_node->get_reordered_dof();
            for(const auto& J : dofs)
                if(J <= t_dof.n_elem) t_mass->at(t_dof(J - 1), t_dof(J - 1)) += pattern;
        }
    }

    return 0;
}
