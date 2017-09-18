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
 * @class Domain
 * @brief A Domain class holds all FE model components.
 * @author T
 * @date 27/07/2017
 * @version 0.2.0
 * @file Domain.h
 * @addtogroup Domain
 * @{
 */

#ifndef DOMAIN_H
#define DOMAIN_H

#include <Domain/DomainBase.h>
#include <Domain/Storage.hpp>

class Domain : public DomainBase, public enable_shared_from_this<Domain> {
    bool updated = false;

    weak_ptr<map<unsigned, shared_ptr<Step>>> step_anchor;

    weak_ptr<Factory<double>> factory; /**< working room */

    AmplitudeStorage amplitude_pond;
    ConstraintStorage constraint_pond;
    ElementStorage element_pond;
    LoadStorage load_pond;
    MaterialStorage material_pond;
    NodeStorage node_pond;
    RecorderStorage recorder_pond;

    unordered_set<unsigned> restrained_dofs;  /**< data storage */
    unordered_set<unsigned> constrained_dofs; /**< data storage */
    unordered_set<unsigned> loaded_dofs;      /**< data storage */
public:
    explicit Domain(const unsigned& = 0);
    ~Domain();

    void set_factory(const weak_ptr<Factory<double>>&) override;
    const weak_ptr<Factory<double>>& get_factory() const override;

    void set_step_anchor(const weak_ptr<map<unsigned, shared_ptr<Step>>>&) override;
    const weak_ptr<map<unsigned, shared_ptr<Step>>>& get_step_anchor() override;

    bool insert(const shared_ptr<Amplitude>&) override;
    bool insert(const shared_ptr<Constraint>&) override;
    bool insert(const shared_ptr<Element>&) override;
    bool insert(const shared_ptr<Load>&) override;
    bool insert(const shared_ptr<Material>&) override;
    bool insert(const shared_ptr<Node>&) override;
    bool insert(const shared_ptr<Recorder>&) override;

    bool erase_amplitude(const unsigned&) override;
    bool erase_constraint(const unsigned&) override;
    bool erase_element(const unsigned&) override;
    bool erase_load(const unsigned&) override;
    bool erase_material(const unsigned&) override;
    bool erase_node(const unsigned&) override;
    bool erase_recorder(const unsigned&) override;

    void disable_amplitude(const unsigned&) override;
    void disable_constraint(const unsigned&) override;
    void disable_element(const unsigned&) override;
    void disable_load(const unsigned&) override;
    void disable_material(const unsigned&) override;
    void disable_node(const unsigned&) override;
    void disable_recorder(const unsigned&) override;

    void enable_amplitude(const unsigned&) override;
    void enable_constraint(const unsigned&) override;
    void enable_element(const unsigned&) override;
    void enable_load(const unsigned&) override;
    void enable_material(const unsigned&) override;
    void enable_node(const unsigned&) override;
    void enable_recorder(const unsigned&) override;

    const shared_ptr<Amplitude>& get_amplitude(const unsigned&) const override;
    const shared_ptr<Constraint>& get_constraint(const unsigned&) const override;
    const shared_ptr<Element>& get_element(const unsigned&) const override;
    const shared_ptr<Load>& get_load(const unsigned&) const override;
    const shared_ptr<Material>& get_material(const unsigned&) const override;
    const shared_ptr<Node>& get_node(const unsigned&) const override;
    const shared_ptr<Recorder>& get_recorder(const unsigned&) const override;

    const vector<shared_ptr<Amplitude>>& get_amplitude_pool() const override;
    const vector<shared_ptr<Constraint>>& get_constraint_pool() const override;
    const vector<shared_ptr<Element>>& get_element_pool() const override;
    const vector<shared_ptr<Load>>& get_load_pool() const override;
    const vector<shared_ptr<Material>>& get_material_pool() const override;
    const vector<shared_ptr<Node>>& get_node_pool() const override;
    const vector<shared_ptr<Recorder>>& get_recorder_pool() const override;

    friend shared_ptr<Amplitude>& get_amplitude(const shared_ptr<Domain>&, const unsigned&);
    friend shared_ptr<Constraint>& get_constraint(const shared_ptr<Domain>&, const unsigned&);
    friend shared_ptr<Element>& get_element(const shared_ptr<Domain>&, const unsigned&);
    friend shared_ptr<Load>& get_load(const shared_ptr<Domain>&, const unsigned&);
    friend shared_ptr<Material>& get_material(const shared_ptr<Domain>&, const unsigned&);
    friend shared_ptr<Node>& get_node(const shared_ptr<Domain>&, const unsigned&);
    friend shared_ptr<Recorder>& get_recorder(const shared_ptr<Domain>&, const unsigned&);

    friend shared_ptr<Amplitude>& get_amplitude(const shared_ptr<DomainBase>&, const unsigned&);
    friend shared_ptr<Constraint>& get_constraint(const shared_ptr<DomainBase>&, const unsigned&);
    friend shared_ptr<Element>& get_element(const shared_ptr<DomainBase>&, const unsigned&);
    friend shared_ptr<Load>& get_load(const shared_ptr<DomainBase>&, const unsigned&);
    friend shared_ptr<Material>& get_material(const shared_ptr<DomainBase>&, const unsigned&);
    friend shared_ptr<Node>& get_node(const shared_ptr<DomainBase>&, const unsigned&);
    friend shared_ptr<Recorder>& get_recorder(const shared_ptr<DomainBase>&, const unsigned&);

    size_t get_amplitude() const override;
    size_t get_constraint() const override;
    size_t get_element() const override;
    size_t get_load() const override;
    size_t get_material() const override;
    size_t get_node() const override;
    size_t get_recorder() const override;

    bool find_amplitude(const unsigned&) const override;
    bool find_constraint(const unsigned&) const override;
    bool find_element(const unsigned&) const override;
    bool find_load(const unsigned&) const override;
    bool find_material(const unsigned&) const override;
    bool find_node(const unsigned&) const override;
    bool find_recorder(const unsigned&) const override;

    bool insert_loaded_dof(const unsigned&) override;
    bool insert_restrained_dof(const unsigned&) override;
    bool insert_constrained_dof(const unsigned&) override;

    const unordered_set<unsigned>& get_loaded_dof() const override;
    const unordered_set<unsigned>& get_restrained_dof() const override;
    const unordered_set<unsigned>& get_constrained_dof() const override;

    const bool& is_updated() const override;

    // initialize the domain
    int initialize() override;
    // process loads and constraints
    void process() override;
    // record response
    void record() override;
    // enable all objects
    void enable_all() override;
    // print out domain summary
    void summary() const override;

    void assemble_resistance() const override;
    void assemble_mass() const override;
    void assemble_initial_stiffness() const override;
    void assemble_stiffness() const override;
    void assemble_damping() const override;

    void erase_machine_error() const override;

    void update_current_status() const override;
    void update_incre_status() const override;
    void update_trial_status() const override;

    void commit_status() const override;
    void clear_status() const override;
    void reset_status() const override;
};

#endif

//! @}
