/*******************************************************************************
 * Copyright (C) 2017 Theodore Chang
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/
/**
 * @class DomainBase
 * @brief A DomainBase class is a template.
 * @author T
 * @date 16/09/2017
 * @version 0.1.0
 * @file DomainBase.h
 * @{
 */

#ifndef DOMAINBASE_H
#define DOMAINBASE_H

#include <Domain/Tag.h>
#include <unordered_map>
#include <unordered_set>

using std::unordered_map;
using std::unordered_set;
using std::vector;

template <typename T> class Storage;
template <typename T> class Factory;
class Constraint;
class Element;
class Load;
class Material;
class Node;
class Recorder;

class DomainBase : public Tag {
public:
    explicit DomainBase(const unsigned&);
    virtual ~DomainBase();

    virtual void set_factory(const shared_ptr<Factory<double>>&);
    virtual const shared_ptr<Factory<double>>& get_factory() const;

    virtual bool insert(const shared_ptr<Constraint>&);
    virtual bool insert(const shared_ptr<Element>&);
    virtual bool insert(const shared_ptr<Load>&);
    virtual bool insert(const shared_ptr<Material>&);
    virtual bool insert(const shared_ptr<Node>&);
    virtual bool insert(const shared_ptr<Recorder>&);

    virtual bool erase_constraint(const unsigned&);
    virtual bool erase_element(const unsigned&);
    virtual bool erase_load(const unsigned&);
    virtual bool erase_material(const unsigned&);
    virtual bool erase_node(const unsigned&);
    virtual bool erase_recorder(const unsigned&);

    virtual void disable_constraint(const unsigned&);
    virtual void disable_element(const unsigned&);
    virtual void disable_load(const unsigned&);
    virtual void disable_material(const unsigned&);
    virtual void disable_node(const unsigned&);
    virtual void disable_recorder(const unsigned&);

    virtual void enable_constraint(const unsigned&);
    virtual void enable_element(const unsigned&);
    virtual void enable_load(const unsigned&);
    virtual void enable_material(const unsigned&);
    virtual void enable_node(const unsigned&);
    virtual void enable_recorder(const unsigned&);

    virtual const shared_ptr<Constraint>& get_constraint(const unsigned&) const;
    virtual const shared_ptr<Element>& get_element(const unsigned&) const;
    virtual const shared_ptr<Load>& get_load(const unsigned&) const;
    virtual const shared_ptr<Material>& get_material(const unsigned&) const;
    virtual const shared_ptr<Node>& get_node(const unsigned&) const;
    virtual const shared_ptr<Recorder>& get_recorder(const unsigned&) const;

    virtual const vector<shared_ptr<Constraint>>& get_constraint_pool() const;
    virtual const vector<shared_ptr<Element>>& get_element_pool() const;
    virtual const vector<shared_ptr<Load>>& get_load_pool() const;
    virtual const vector<shared_ptr<Material>>& get_material_pool() const;
    virtual const vector<shared_ptr<Node>>& get_node_pool() const;
    virtual const vector<shared_ptr<Recorder>>& get_recorder_pool() const;

    virtual size_t get_constraint() const;
    virtual size_t get_element() const;
    virtual size_t get_load() const;
    virtual size_t get_material() const;
    virtual size_t get_node() const;
    virtual size_t get_recorder() const;

    virtual bool find_constraint(const unsigned&) const;
    virtual bool find_element(const unsigned&) const;
    virtual bool find_load(const unsigned&) const;
    virtual bool find_material(const unsigned&) const;
    virtual bool find_node(const unsigned&) const;
    virtual bool find_recorder(const unsigned&) const;

    virtual bool insert_loaded_dof(const unsigned&);
    virtual bool insert_restrained_dof(const unsigned&);
    virtual bool insert_constrained_dof(const unsigned&);

    virtual const unordered_set<unsigned>& get_loaded_dof() const;
    virtual const unordered_set<unsigned>& get_restrained_dof() const;
    virtual const unordered_set<unsigned>& get_constrained_dof() const;

    virtual const bool& is_updated() const;

    virtual int initialize();
    virtual void process(const unsigned& = 0);
    virtual void record();
    virtual void enable_all();
    virtual void summary() const;

    virtual void assemble_resistance() const;
    virtual void assemble_mass() const;
    virtual void assemble_initial_stiffness() const;
    virtual void assemble_stiffness() const;
    virtual void assemble_damping() const;

    virtual void set_current_status() const;

    virtual void update_trial_status() const;
    virtual void update_incre_status() const;

    virtual void commit_status() const;
    virtual void clear_status() const;
    virtual void reset_status() const;
};

#endif

//! @}
