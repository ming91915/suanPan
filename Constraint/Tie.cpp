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

#include "Tie.h"
#include <Domain/DomainBase.h>
#include <Domain/Factory.hpp>
#include <Domain/Node.h>

Tie::Tie(const unsigned& T, const unsigned& S, const unsigned& NA, const unsigned& DA, const unsigned& NB, const unsigned& DB)
    : MPC(T, CT_TIE, S)
    , node_i(NA)
    , dof_i(DA)
    , node_j(NB)
    , dof_j(DB) {}

Tie::Tie(const unsigned& S, const unsigned& NA, const unsigned& DA, const unsigned& NB, const unsigned& DB)
    : MPC(0, CT_TIE, S)
    , node_i(NA)
    , dof_i(DA)
    , node_j(NB)
    , dof_j(DB) {}

int Tie::process(const shared_ptr<DomainBase>& D) {
    const auto& t_factory = D->get_factory();

    const auto t_max = t_factory->get_stiffness()->max();

    auto& t_dof_i = D->get_node(node_i)->get_reordered_dof().at(dof_i - 1);
    auto& t_dof_j = D->get_node(node_j)->get_reordered_dof().at(dof_j - 1);

    auto& t_matrix = t_factory->get_stiffness();

    t_matrix->at(t_dof_i, t_dof_i) += t_max;
    t_matrix->at(t_dof_j, t_dof_j) += t_max;
    t_matrix->at(t_dof_i, t_dof_j) -= t_max;
    t_matrix->at(t_dof_j, t_dof_i) -= t_max;

    return 0;
}
