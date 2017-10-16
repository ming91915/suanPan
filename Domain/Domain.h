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
 * @date 01/10/2017
 * @version 0.3.0
 * @file Domain.h
 * @addtogroup Domain
 * @{
 */

#ifndef DOMAIN_H
#define DOMAIN_H

#include <Domain/DomainBase.h>
#include <Domain/Storage.hpp>

using ExternalModuleQueue = vector<shared_ptr<ExternalModule>>;

class Domain : public DomainBase, public std::enable_shared_from_this<Domain> {
    bool updated = false;

    unsigned current_step_tag = 0;
    unsigned current_converger_tag = 0;
    unsigned current_integrator_tag = 0;
    unsigned current_solver_tag = 0;

    // dynamic libraries should be destroyed after all dependent objects are destroyed
    ExternalModuleQueue external_module_pond;

    shared_ptr<LongFactory> factory; /**< working room */

    StepQueue step_pond;

    AmplitudeStorage amplitude_pond;
    ConstraintStorage constraint_pond;
    ConvergerStorage converger_pond;
    CriterionStorage criterion_pond;
    ElementStorage element_pond;
    IntegratorStorage integrator_pond;
    LoadStorage load_pond;
    MaterialStorage material_pond;
    NodeStorage node_pond;
    RecorderStorage recorder_pond;
    SectionStorage section_pond;
    SolverStorage solver_pond;

    unordered_set<unsigned> constrained_dofs; /**< data storage */
    unordered_set<unsigned> loaded_dofs;      /**< data storage */
    unordered_set<unsigned> restrained_dofs;  /**< data storage */
public:
    explicit Domain(const unsigned& = 0);

    void set_factory(const shared_ptr<LongFactory>&) override;
    const shared_ptr<LongFactory>& get_factory() const override;

    bool insert(const shared_ptr<ExternalModule>&) override;
    const ExternalModuleQueue& get_external_module_pool() const override;

    bool insert(const shared_ptr<Amplitude>&) override;
    bool insert(const shared_ptr<Constraint>&) override;
    bool insert(const shared_ptr<Converger>&) override;
    bool insert(const shared_ptr<Criterion>&) override;
    bool insert(const shared_ptr<Element>&) override;
    bool insert(const shared_ptr<Integrator>&) override;
    bool insert(const shared_ptr<Load>&) override;
    bool insert(const shared_ptr<Material>&) override;
    bool insert(const shared_ptr<Node>&) override;
    bool insert(const shared_ptr<Recorder>&) override;
    bool insert(const shared_ptr<Section>&) override;
    bool insert(const shared_ptr<Solver>&) override;
    bool insert(const shared_ptr<Step>&) override;

    bool erase_amplitude(const unsigned&) override;
    bool erase_constraint(const unsigned&) override;
    bool erase_converger(const unsigned&) override;
    bool erase_criterion(const unsigned&) override;
    bool erase_element(const unsigned&) override;
    bool erase_integrator(const unsigned&) override;
    bool erase_load(const unsigned&) override;
    bool erase_material(const unsigned&) override;
    bool erase_node(const unsigned&) override;
    bool erase_recorder(const unsigned&) override;
    bool erase_section(const unsigned&) override;
    bool erase_solver(const unsigned&) override;
    bool erase_step(const unsigned&) override;

    void disable_amplitude(const unsigned&) override;
    void disable_constraint(const unsigned&) override;
    void disable_converger(const unsigned&) override;
    void disable_criterion(const unsigned&) override;
    void disable_element(const unsigned&) override;
    void disable_integrator(const unsigned&) override;
    void disable_load(const unsigned&) override;
    void disable_material(const unsigned&) override;
    void disable_node(const unsigned&) override;
    void disable_recorder(const unsigned&) override;
    void disable_section(const unsigned&) override;
    void disable_solver(const unsigned&) override;
    void disable_step(const unsigned&) override;

    void enable_amplitude(const unsigned&) override;
    void enable_constraint(const unsigned&) override;
    void enable_converger(const unsigned&) override;
    void enable_criterion(const unsigned&) override;
    void enable_element(const unsigned&) override;
    void enable_integrator(const unsigned&) override;
    void enable_load(const unsigned&) override;
    void enable_material(const unsigned&) override;
    void enable_node(const unsigned&) override;
    void enable_recorder(const unsigned&) override;
    void enable_section(const unsigned&) override;
    void enable_solver(const unsigned&) override;
    void enable_step(const unsigned&) override;

    const shared_ptr<Amplitude>& get_amplitude(const unsigned&) const override;
    const shared_ptr<Constraint>& get_constraint(const unsigned&) const override;
    const shared_ptr<Converger>& get_converger(const unsigned&) const override;
    const shared_ptr<Criterion>& get_criterion(const unsigned&) const override;
    const shared_ptr<Element>& get_element(const unsigned&) const override;
    const shared_ptr<Integrator>& get_integrator(const unsigned&) const override;
    const shared_ptr<Load>& get_load(const unsigned&) const override;
    const shared_ptr<Material>& get_material(const unsigned&) const override;
    const shared_ptr<Node>& get_node(const unsigned&) const override;
    const shared_ptr<Recorder>& get_recorder(const unsigned&) const override;
    const shared_ptr<Section>& get_section(const unsigned&) const override;
    const shared_ptr<Solver>& get_solver(const unsigned&) const override;
    const shared_ptr<Step>& get_step(const unsigned&) const override;

    const AmplitudeQueue& get_amplitude_pool() const override;
    const ConstraintQueue& get_constraint_pool() const override;
    const ConvergerQueue& get_converger_pool() const override;
    const CriterionQueue& get_criterion_pool() const override;
    const ElementQueue& get_element_pool() const override;
    const IntegratorQueue& get_integrator_pool() const override;
    const LoadQueue& get_load_pool() const override;
    const MaterialQueue& get_material_pool() const override;
    const NodeQueue& get_node_pool() const override;
    const RecorderQueue& get_recorder_pool() const override;
    const SectionQueue& get_section_pool() const override;
    const SolverQueue& get_solver_pool() const override;
    const StepQueue& get_step_pool() const override;

    friend shared_ptr<Amplitude>& get_amplitude(const shared_ptr<Domain>&, const unsigned&);
    friend shared_ptr<Constraint>& get_constraint(const shared_ptr<Domain>&, const unsigned&);
    friend shared_ptr<Converger>& get_converger(const shared_ptr<Domain>&, const unsigned&);
    friend shared_ptr<Criterion>& get_criterion(const shared_ptr<Domain>&, const unsigned&);
    friend shared_ptr<Element>& get_element(const shared_ptr<Domain>&, const unsigned&);
    friend shared_ptr<Integrator>& get_integrator(const shared_ptr<Domain>&, const unsigned&);
    friend shared_ptr<Load>& get_load(const shared_ptr<Domain>&, const unsigned&);
    friend shared_ptr<Material>& get_material(const shared_ptr<Domain>&, const unsigned&);
    friend shared_ptr<Node>& get_node(const shared_ptr<Domain>&, const unsigned&);
    friend shared_ptr<Recorder>& get_recorder(const shared_ptr<Domain>&, const unsigned&);
    friend shared_ptr<Section>& get_section(const shared_ptr<Domain>&, const unsigned&);
    friend shared_ptr<Solver>& get_solver(const shared_ptr<Domain>&, const unsigned&);
    friend shared_ptr<Step>& get_step(const shared_ptr<Domain>&, const unsigned&);

    friend shared_ptr<Amplitude>& get_amplitude(const shared_ptr<DomainBase>&, const unsigned&);
    friend shared_ptr<Constraint>& get_constraint(const shared_ptr<DomainBase>&, const unsigned&);
    friend shared_ptr<Converger>& get_converger(const shared_ptr<DomainBase>&, const unsigned&);
    friend shared_ptr<Criterion>& get_criterion(const shared_ptr<DomainBase>&, const unsigned&);
    friend shared_ptr<Element>& get_element(const shared_ptr<DomainBase>&, const unsigned&);
    friend shared_ptr<Integrator>& get_integrator(const shared_ptr<DomainBase>&, const unsigned&);
    friend shared_ptr<Load>& get_load(const shared_ptr<DomainBase>&, const unsigned&);
    friend shared_ptr<Material>& get_material(const shared_ptr<DomainBase>&, const unsigned&);
    friend shared_ptr<Node>& get_node(const shared_ptr<DomainBase>&, const unsigned&);
    friend shared_ptr<Recorder>& get_recorder(const shared_ptr<DomainBase>&, const unsigned&);
    friend shared_ptr<Section>& get_section(const shared_ptr<DomainBase>&, const unsigned&);
    friend shared_ptr<Solver>& get_solver(const shared_ptr<DomainBase>&, const unsigned&);
    friend shared_ptr<Step>& get_step(const shared_ptr<DomainBase>&, const unsigned&);

    size_t get_amplitude() const override;
    size_t get_constraint() const override;
    size_t get_converger() const override;
    size_t get_criterion() const override;
    size_t get_element() const override;
    size_t get_integrator() const override;
    size_t get_load() const override;
    size_t get_material() const override;
    size_t get_node() const override;
    size_t get_recorder() const override;
    size_t get_section() const override;
    size_t get_solver() const override;
    size_t get_step() const override;

    bool find_amplitude(const unsigned&) const override;
    bool find_constraint(const unsigned&) const override;
    bool find_converger(const unsigned&) const override;
    bool find_criterion(const unsigned&) const override;
    bool find_element(const unsigned&) const override;
    bool find_integrator(const unsigned&) const override;
    bool find_load(const unsigned&) const override;
    bool find_material(const unsigned&) const override;
    bool find_node(const unsigned&) const override;
    bool find_recorder(const unsigned&) const override;
    bool find_section(const unsigned&) const override;
    bool find_solver(const unsigned&) const override;
    bool find_step(const unsigned&) const override;

    void set_current_step_tag(const unsigned&) override;
    void set_current_converger_tag(const unsigned&) override;
    void set_current_integrator_tag(const unsigned&) override;
    void set_current_solver_tag(const unsigned&) override;

    const unsigned& get_current_step_tag() override;
    const unsigned& get_current_converger_tag() override;
    const unsigned& get_current_integrator_tag() override;
    const unsigned& get_current_solver_tag() override;

    const shared_ptr<Step>& get_current_step() const override;
    const shared_ptr<Converger>& get_current_converger() const override;
    const shared_ptr<Integrator>& get_current_integrator() const override;
    const shared_ptr<Solver>& get_current_solver() const override;

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
    int process_load() override;
    int process_constraint() override;
    int process_criterion() override;
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

    int update_current_status() const override;
    int update_incre_status() const override;
    int update_trial_status() const override;

    void commit_status() const override;
    void clear_status() const override;
    void reset_status() const override;
};

#endif

//! @}
