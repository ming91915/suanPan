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

#include "T3D2.h"
#include <Domain/DomainBase.h>
#include <Domain/Node.h>
#include <Material/Material1D/Material1D.h>

const unsigned T3D2::t_node = 2;
const unsigned T3D2::t_dof = 3;

T3D2::T3D2(const unsigned& T, const uvec& N, const unsigned& M, const double& A, const bool& F, const bool& UA, const bool& LS)
    : MaterialElement(T, ET_T3D2, t_node, t_dof, N, uvec{ M }, F)
    , area(A)
    , direction_cosine(3)
    , update_area(UA)
    , log_strain(LS) {}

void T3D2::initialize(const shared_ptr<DomainBase>& D) {
    auto& coord_i = node_ptr.at(0).lock()->get_coordinate();
    auto& coord_j = node_ptr.at(1).lock()->get_coordinate();

    if(coord_i.size() != t_dof || coord_j.size() != t_dof) {
        suanpan_error("initialize() finds incompatible nodes.\n");
        return;
    }

    const vec pos_diff = coord_j - coord_i;

    length = norm(pos_diff);

    t_material = D->get_material(unsigned(material_tag(0)))->get_copy();
}

int T3D2::update_status() {
    const auto& node_i = node_ptr.at(0).lock();
    const auto& node_j = node_ptr.at(1).lock();

    // in a truss-beam system a node may have either 2 or 3 dofs depends on the type of elements connected
    // resize the displacement vectors to make sure they are compatiable with the truss formulation
    auto& disp_i = node_i->get_trial_displacement();
    auto& disp_j = node_j->get_trial_displacement();
    vec disp_diff(3);
    disp_diff(0) = disp_j(0) - disp_i(0);
    disp_diff(1) = disp_j(1) - disp_i(1);
    disp_diff(2) = disp_j(2) - disp_i(2);

    return 0;
}

int T3D2::commit_status() { return t_material->commit_status(); }

int T3D2::clear_status() { return t_material->clear_status(); }

int T3D2::reset_status() { return t_material->reset_status(); }

void T3D2::print() {}
