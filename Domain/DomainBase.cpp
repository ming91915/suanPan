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
#include "DomainBase.h"

DomainBase::DomainBase(const unsigned& T)
    : Tag(T, CT_DOMAIN) {}

DomainBase::~DomainBase() {}

void DomainBase::set_factory(const shared_ptr<Factory<double>>&) { throw; }

const shared_ptr<Factory<double>>& DomainBase::get_factory() const { throw; }

bool DomainBase::insert(const shared_ptr<Constraint>&) { throw; }

bool DomainBase::insert(const shared_ptr<Element>&) { throw; }

bool DomainBase::insert(const shared_ptr<Load>&) { throw; }

bool DomainBase::insert(const shared_ptr<Material>&) { throw; }

bool DomainBase::insert(const shared_ptr<Node>&) { throw; }

bool DomainBase::insert(const shared_ptr<Recorder>&) { throw; }

bool DomainBase::erase_constraint(const unsigned&) { throw; }

bool DomainBase::erase_element(const unsigned&) { throw; }

bool DomainBase::erase_load(const unsigned&) { throw; }

bool DomainBase::erase_material(const unsigned&) { throw; }

bool DomainBase::erase_node(const unsigned&) { throw; }

bool DomainBase::erase_recorder(const unsigned&) { throw; }

void DomainBase::disable_constraint(const unsigned&) { throw; }

void DomainBase::disable_element(const unsigned&) { throw; }

void DomainBase::disable_load(const unsigned&) { throw; }

void DomainBase::disable_material(const unsigned&) { throw; }

void DomainBase::disable_node(const unsigned&) { throw; }

void DomainBase::disable_recorder(const unsigned&) { throw; }

void DomainBase::enable_constraint(const unsigned&) { throw; }

void DomainBase::enable_element(const unsigned&) { throw; }

void DomainBase::enable_load(const unsigned&) { throw; }

void DomainBase::enable_material(const unsigned&) { throw; }

void DomainBase::enable_node(const unsigned&) { throw; }

void DomainBase::enable_recorder(const unsigned&) { throw; }

const shared_ptr<Constraint>& DomainBase::get_constraint(const unsigned&) const { throw; }

const shared_ptr<Element>& DomainBase::get_element(const unsigned&) const { throw; }

const shared_ptr<Load>& DomainBase::get_load(const unsigned&) const { throw; }

const shared_ptr<Material>& DomainBase::get_material(const unsigned&) const { throw; }

const shared_ptr<Node>& DomainBase::get_node(const unsigned&) const { throw; }

const shared_ptr<Recorder>& DomainBase::get_recorder(const unsigned&) const { throw; }

const vector<shared_ptr<Constraint>>& DomainBase::get_constraint_pool() const { throw; }

const vector<shared_ptr<Element>>& DomainBase::get_element_pool() const { throw; }

const vector<shared_ptr<Load>>& DomainBase::get_load_pool() const { throw; }

const vector<shared_ptr<Material>>& DomainBase::get_material_pool() const { throw; }

const vector<shared_ptr<Node>>& DomainBase::get_node_pool() const { throw; }

const vector<shared_ptr<Recorder>>& DomainBase::get_recorder_pool() const { throw; }

size_t DomainBase::get_constraint() const { throw; }

size_t DomainBase::get_element() const { throw; }

size_t DomainBase::get_load() const { throw; }

size_t DomainBase::get_material() const { throw; }

size_t DomainBase::get_node() const { throw; }

size_t DomainBase::get_recorder() const { throw; }

bool DomainBase::find_constraint(const unsigned&) const { throw; }

bool DomainBase::find_element(const unsigned&) const { throw; }

bool DomainBase::find_load(const unsigned&) const { throw; }

bool DomainBase::find_material(const unsigned&) const { throw; }

bool DomainBase::find_node(const unsigned&) const { throw; }

bool DomainBase::find_recorder(const unsigned&) const { throw; }

bool DomainBase::insert_loaded_dof(const unsigned&) { throw; }

bool DomainBase::insert_restrained_dof(const unsigned&) { throw; }

bool DomainBase::insert_constrained_dof(const unsigned&) { throw; }

const unordered_set<unsigned>& DomainBase::get_loaded_dof() const { throw; }

const unordered_set<unsigned>& DomainBase::get_restrained_dof() const { throw; }

const unordered_set<unsigned>& DomainBase::get_constrained_dof() const { throw; }

const bool& DomainBase::is_updated() const { throw; }

int DomainBase::initialize() { throw; }

void DomainBase::process(const unsigned&) { throw; }

void DomainBase::record() { throw; }

void DomainBase::enable_all() { throw; }

void DomainBase::summary() const { throw; }

void DomainBase::assemble_resistance() const { throw; }

void DomainBase::assemble_mass() const { throw; }

void DomainBase::assemble_initial_stiffness() const { throw; }

void DomainBase::assemble_stiffness() const { throw; }

void DomainBase::assemble_damping() const { throw; }

void DomainBase::set_current_status() const { throw; }

void DomainBase::update_trial_status() const { throw; }

void DomainBase::update_incre_status() const { throw; }

void DomainBase::commit_status() const { throw; }

void DomainBase::clear_status() const { throw; }

void DomainBase::reset_status() const { throw; }
