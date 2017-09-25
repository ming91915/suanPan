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

#include "MaxDisplacement.h"
#include <Domain/DomainBase.h>
#include <Domain/Factory.hpp>
#include <Domain/Node.h>

MaxDisplacement::MaxDisplacement(const unsigned& T, const unsigned& ST, const unsigned& NT, const unsigned& DT, const double& MA)
    : Criterion(T, CT_MAXDISPLACEMENT, ST)
    , node(NT)
    , dof(DT)
    , limit(MA) {}

int MaxDisplacement::process(const shared_ptr<DomainBase>& D) {
    auto& t_dof = D->get_node(node)->get_reordered_dof().at(dof - 1);

    auto& t_disp = D->get_factory().lock()->get_current_displacement();

    return t_disp(t_dof) > limit ? -1 : 0;
}
