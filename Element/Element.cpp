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

#include "Element.h"

Element::Element(const unsigned& T, const unsigned& CT, const unsigned& NN, const unsigned& ND, const uvec& NT, const uvec& MT, const bool& F)
    : Tag(T, CT)
    , num_node(NN)
    , num_dof(ND)
    , node_encoding(NT)
    , material_tag(MT)
    , nlgeom(F) {
    suanpan_debug("Element %u ctor() called.\n", T);
}

Element::~Element() { suanpan_debug("Element %u dtor() called.\n", get_tag()); }

void Element::initialize(const shared_ptr<DomainBase>& D) {
    // initialized before check node vadality
    if(node_ptr.size() == num_node) {
        for(const auto& I : node_ptr) {
            const auto& t_node = I.lock();
            if(t_node == nullptr || !t_node->is_active()) {
                D->disable_element(get_tag());
                return;
            }
        }
        return;
    }

    // first initiliazation
    const auto total_dof = num_node * num_dof;

    if(total_dof == 0) {
        D->disable_element(get_tag());
        return;
    }

    dof_encoding.zeros(total_dof);
    resistance.zeros(total_dof);
    mass.zeros(total_dof, total_dof);
    damping.zeros(total_dof, total_dof);
    stiffness.zeros(total_dof, total_dof);
    initial_stiffness.zeros(total_dof, total_dof);

    // check if nodes are still valid
    node_ptr.clear();
    node_ptr.reserve(num_node);
    for(const auto& tmp_tag : node_encoding) {
        auto& tmp_node = D->get_node(unsigned(tmp_tag));
        if(tmp_node == nullptr || !tmp_node->is_active()) {
            suanpan_debug("Element %u finds an invalid node %u, now disable it.\n", get_tag(), tmp_tag);
            D->disable_element(get_tag());
            return;
        }
        if(tmp_node->get_dof_number() < num_dof) tmp_node->set_dof_number(num_dof);
        node_ptr.emplace_back(tmp_node);
    }

    // check if material models are valid
    for(const auto& tmp_materail : material_tag)
        if(!D->find_material(unsigned(tmp_materail))) {
            suanpan_debug("Element %u cannot find valid material %u, now disable it.\n", get_tag(), unsigned(tmp_materail));
            D->disable_element(get_tag());
            return;
        }
}

void Element::update_dof_encoding() {
    auto idx = 0;
    for(const auto& tmp_ptr : node_ptr) {
        auto& node_dof = tmp_ptr.lock()->get_reordered_dof();
        for(unsigned i = 0; i < num_dof; ++i) dof_encoding(idx++) = node_dof(i);
    }
}

const unsigned& Element::get_dof_number() const { return num_dof; }

const unsigned& Element::get_node_number() const { return num_node; }

const uvec& Element::get_dof_encoding() const { return dof_encoding; }

const uvec& Element::get_node_encoding() const { return node_encoding; }

const vector<weak_ptr<Node>>& Element::get_node_ptr() const { return node_ptr; }

const vec& Element::get_resistance() const { return resistance; }

const mat& Element::get_mass() const { return mass; }

const mat& Element::get_damping() const { return damping; }

const mat& Element::get_stiffness() const { return stiffness; }

const mat& Element::get_initial_stiffness() const { return initial_stiffness; }

const mat& Element::get_geometry() const { return geometry; }

int Element::update_status() { throw; }

int Element::commit_status() { throw; }

int Element::clear_status() { throw; }

int Element::reset_status() { throw; }

vector<vec> Element::record(const OutputList&) { return {}; }
