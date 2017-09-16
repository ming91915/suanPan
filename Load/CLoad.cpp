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
#include "CLoad.h"
#include <Domain/DomainBase.h>
#include <Domain/Factory.hpp>
#include <Domain/Node.h>
#include <Load/Amplitude/Amplitude.h>

CLoad::CLoad(const unsigned& T, const unsigned& S, const double& L, const uvec& N, const unsigned& D, const shared_ptr<Amplitude>& A)
    : Load(T, CT_CLOAD, S, A)
    , pattern(L)
    , nodes(N)
    , dofs({ D }) {}

CLoad::CLoad(const unsigned& S, const double& L, const uvec& N, const unsigned& D, const shared_ptr<Amplitude>& A)
    : Load(0, CT_CLOAD, S, A)
    , pattern(L)
    , nodes(N)
    , dofs({ D }) {}

CLoad::CLoad(const unsigned& T, const unsigned& S, const double& L, const uvec& N, const uvec& D, const shared_ptr<Amplitude>& A)
    : Load(T, CT_CLOAD, S, A)
    , pattern(L)
    , nodes(N)
    , dofs(D) {}

CLoad::CLoad(const unsigned& S, const double& L, const uvec& N, const uvec& D, const shared_ptr<Amplitude>& A)
    : Load(0, CT_CLOAD, S, A)
    , pattern(L)
    , nodes(N)
    , dofs(D) {}

int CLoad::process(const shared_ptr<DomainBase>& D) {
    auto& tmp_workroom = D->get_factory();

    const auto final_load = pattern * magnitude->get_amplitude(tmp_workroom->get_trial_time());

    auto& tmp_load = get_trial_load(tmp_workroom);

    for(const auto& I : nodes) {
        auto& tmp_node = D->get_node(static_cast<unsigned>(I));
        if(tmp_node != nullptr && tmp_node->is_active()) {
            auto& tmp_dof = tmp_node->get_reordered_dof();
            for(const auto& J : dofs)
                if(J <= tmp_dof.n_elem) {
                    tmp_load(tmp_dof(J - 1)) += final_load;
                    D->insert_loaded_dof(unsigned(tmp_dof(J - 1)));
                }
        }
    }

    tmp_workroom->set_incre_load(tmp_load - tmp_workroom->get_current_load());

    return 0;
}
