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

#include "Domain.h"
#include <Constraint/Constraint.h>
#include <Constraint/Criterion/Criterion.h>
#include <Converger/Converger.h>
#include <Domain/Factory.hpp>
#include <Domain/Node.h>
#include <Element/Element.h>
#include <Load/Amplitude/Amplitude.h>
#include <Load/Load.h>
#include <Material/Material.h>
#include <Recorder/Recorder.h>
#include <Section/Section.h>
#include <Solver/Integrator/Integrator.h>
#include <Solver/Solver.h>
#include <Step/Step.h>
#include <Toolbox/RCM.h>

#ifdef SUANPAN_MT
#ifdef SUANPAN_MSVC
#include <ppl.h>
#define suanpan_for_each Concurrency::parallel_for_each
#elif defined(SUANPAN_INTEL)
#include <tbb/parallel_for_each.h>
#define suanpan_for_each tbb::parallel_for_each
#else
#define suanpan_for_each std::for_each
#endif
#else
#define suanpan_for_each std::for_each
#endif

Domain::Domain(const unsigned& T)
    : DomainBase(T)
    , factory(make_shared<Factory<double>>()) {}

void Domain::set_factory(const shared_ptr<LongFactory>& F) {
    if(factory != F) {
        factory = F;
        updated = false;
    }
}

const shared_ptr<LongFactory>& Domain::get_factory() const { return factory; }

bool Domain::insert(const shared_ptr<ExternalModule>& E) {
    external_module_pond.emplace_back(E);
    return true;
}

const ExternalModuleQueue& Domain::get_external_module_pool() const { return external_module_pond; }

bool Domain::insert(const shared_ptr<Amplitude>& A) {
    if(updated) updated = false;
    return amplitude_pond.insert(A);
}

bool Domain::insert(const shared_ptr<Constraint>& C) {
    if(updated) updated = false;
    return constraint_pond.insert(C);
}

bool Domain::insert(const shared_ptr<Converger>& C) {
    if(updated) updated = false;
    return converger_pond.insert(C);
}

bool Domain::insert(const shared_ptr<Criterion>& C) {
    if(updated) updated = false;
    return criterion_pond.insert(C);
}

bool Domain::insert(const shared_ptr<Element>& E) {
    if(updated) updated = false;
    return element_pond.insert(E);
}

bool Domain::insert(const shared_ptr<Integrator>& I) {
    if(updated) updated = false;
    return integrator_pond.insert(I);
}

bool Domain::insert(const shared_ptr<Load>& L) {
    if(updated) updated = false;
    return load_pond.insert(L);
}

bool Domain::insert(const shared_ptr<Material>& M) {
    if(updated) updated = false;
    return material_pond.insert(M);
}

bool Domain::insert(const shared_ptr<Node>& N) {
    if(updated) updated = false;
    return node_pond.insert(N);
}

bool Domain::insert(const shared_ptr<Recorder>& R) {
    if(updated) updated = false;
    return recorder_pond.insert(R);
}

bool Domain::insert(const shared_ptr<Section>& S) {
    if(updated) updated = false;
    return section_pond.insert(S);
}

bool Domain::insert(const shared_ptr<Solver>& S) {
    if(updated) updated = false;
    return solver_pond.insert(S);
}

bool Domain::insert(const shared_ptr<Step>& S) {
    if(updated) updated = false;
    return step_pond.insert({ S->get_tag(), S }).second;
}

bool Domain::erase_amplitude(const unsigned& T) {
    if(updated) updated = false;
    return amplitude_pond.erase(T);
}

bool Domain::erase_constraint(const unsigned& T) {
    if(updated) updated = false;
    return constraint_pond.erase(T);
}

bool Domain::erase_converger(const unsigned& T) {
    if(updated) updated = false;
    return converger_pond.erase(T);
}

bool Domain::erase_criterion(const unsigned& T) {
    if(updated) updated = false;
    return criterion_pond.erase(T);
}

bool Domain::erase_element(const unsigned& T) {
    if(updated) updated = false;
    return element_pond.erase(T);
}

bool Domain::erase_integrator(const unsigned& T) {
    if(updated) updated = false;
    return integrator_pond.erase(T);
}

bool Domain::erase_load(const unsigned& T) {
    if(updated) updated = false;
    return load_pond.erase(T);
}

bool Domain::erase_material(const unsigned& T) {
    if(updated) updated = false;
    return material_pond.erase(T);
}

bool Domain::erase_node(const unsigned& T) {
    if(updated) updated = false;
    return node_pond.erase(T);
}

bool Domain::erase_recorder(const unsigned& T) {
    if(updated) updated = false;
    return recorder_pond.erase(T);
}

bool Domain::erase_section(const unsigned& T) {
    if(updated) updated = false;
    return section_pond.erase(T);
}

bool Domain::erase_solver(const unsigned& T) {
    if(updated) updated = false;
    return solver_pond.erase(T);
}

bool Domain::erase_step(const unsigned& T) {
    if(updated) updated = false;
    return step_pond.erase(T);
}

void Domain::disable_amplitude(const unsigned& T) {
    if(updated) updated = false;
    amplitude_pond.disable(T);
}

void Domain::disable_constraint(const unsigned& T) {
    if(updated) updated = false;
    constraint_pond.disable(T);
}

void Domain::disable_converger(const unsigned& T) {
    if(updated) updated = false;
    converger_pond.disable(T);
}

void Domain::disable_criterion(const unsigned& T) {
    if(updated) updated = false;
    criterion_pond.disable(T);
}

void Domain::disable_element(const unsigned& T) {
    if(updated) updated = false;
    element_pond.disable(T);
}

void Domain::disable_integrator(const unsigned& T) {
    if(updated) updated = false;
    integrator_pond.disable(T);
}

void Domain::disable_load(const unsigned& T) {
    if(updated) updated = false;
    load_pond.disable(T);
}

void Domain::disable_material(const unsigned& T) {
    if(updated) updated = false;
    material_pond.disable(T);
}

void Domain::disable_node(const unsigned& T) {
    if(updated) updated = false;
    node_pond.disable(T);
}

void Domain::disable_recorder(const unsigned& T) {
    if(updated) updated = false;
    recorder_pond.disable(T);
}

void Domain::disable_section(const unsigned& T) {
    if(updated) updated = false;
    section_pond.disable(T);
}

void Domain::disable_solver(const unsigned& T) {
    if(updated) updated = false;
    solver_pond.disable(T);
}

void Domain::disable_step(const unsigned& T) {
    if(updated) updated = false;
    if(step_pond.find(T) != step_pond.end()) step_pond.at(T)->disable();
}

void Domain::enable_amplitude(const unsigned& T) {
    if(updated) updated = false;
    amplitude_pond.enable(T);
}

void Domain::enable_constraint(const unsigned& T) {
    if(updated) updated = false;
    constraint_pond.enable(T);
}

void Domain::enable_converger(const unsigned& T) {
    if(updated) updated = false;
    converger_pond.enable(T);
}

void Domain::enable_criterion(const unsigned& T) {
    if(updated) updated = false;
    criterion_pond.enable(T);
}

void Domain::enable_element(const unsigned& T) {
    if(updated) updated = false;
    element_pond.enable(T);
}

void Domain::enable_integrator(const unsigned& T) {
    if(updated) updated = false;
    integrator_pond.enable(T);
}

void Domain::enable_load(const unsigned& T) {
    if(updated) updated = false;
    load_pond.enable(T);
}

void Domain::enable_material(const unsigned& T) {
    if(updated) updated = false;
    material_pond.enable(T);
}

void Domain::enable_node(const unsigned& T) {
    if(updated) updated = false;
    node_pond.enable(T);
}

void Domain::enable_recorder(const unsigned& T) {
    if(updated) updated = false;
    recorder_pond.enable(T);
}

void Domain::enable_section(const unsigned& T) {
    if(updated) updated = false;
    section_pond.enable(T);
}

void Domain::enable_solver(const unsigned& T) {
    if(updated) updated = false;
    solver_pond.enable(T);
}

void Domain::enable_step(const unsigned& T) {
    if(updated) updated = false;
    if(step_pond.find(T) != step_pond.end()) step_pond.at(T)->enable();
}

const shared_ptr<Amplitude>& Domain::get_amplitude(const unsigned& T) const { return amplitude_pond.at(T); }

const shared_ptr<Constraint>& Domain::get_constraint(const unsigned& T) const { return constraint_pond.at(T); }

const shared_ptr<Converger>& Domain::get_converger(const unsigned& T) const { return converger_pond.at(T); }

const shared_ptr<Criterion>& Domain::get_criterion(const unsigned& T) const { return criterion_pond.at(T); }

const shared_ptr<Element>& Domain::get_element(const unsigned& T) const { return element_pond.at(T); }

const shared_ptr<Integrator>& Domain::get_integrator(const unsigned& T) const { return integrator_pond.at(T); }

const shared_ptr<Load>& Domain::get_load(const unsigned& T) const { return load_pond.at(T); }

const shared_ptr<Material>& Domain::get_material(const unsigned& T) const { return material_pond.at(T); }

const shared_ptr<Node>& Domain::get_node(const unsigned& T) const { return node_pond.at(T); }

const shared_ptr<Recorder>& Domain::get_recorder(const unsigned& T) const { return recorder_pond.at(T); }

const shared_ptr<Section>& Domain::get_section(const unsigned& T) const { return section_pond.at(T); }

const shared_ptr<Solver>& Domain::get_solver(const unsigned& T) const { return solver_pond.at(T); }

const shared_ptr<Step>& Domain::get_step(const unsigned& T) const { return step_pond.at(T); }

const AmplitudeQueue& Domain::get_amplitude_pool() const { return amplitude_pond.get(); }

const ConstraintQueue& Domain::get_constraint_pool() const { return constraint_pond.get(); }

const ConvergerQueue& Domain::get_converger_pool() const { return converger_pond.get(); }

const CriterionQueue& Domain::get_criterion_pool() const { return criterion_pond.get(); }

const ElementQueue& Domain::get_element_pool() const { return element_pond.get(); }

const IntegratorQueue& Domain::get_integrator_pool() const { return integrator_pond.get(); }

const LoadQueue& Domain::get_load_pool() const { return load_pond.get(); }

const MaterialQueue& Domain::get_material_pool() const { return material_pond.get(); }

const NodeQueue& Domain::get_node_pool() const { return node_pond.get(); }

const RecorderQueue& Domain::get_recorder_pool() const { return recorder_pond.get(); }

const SectionQueue& Domain::get_section_pool() const { return section_pond.get(); }

const SolverQueue& Domain::get_solver_pool() const { return solver_pond.get(); }

const StepQueue& Domain::get_step_pool() const { return step_pond; }

size_t Domain::get_amplitude() const { return amplitude_pond.size(); }

size_t Domain::get_constraint() const { return constraint_pond.size(); }

size_t Domain::get_converger() const { return converger_pond.size(); }

size_t Domain::get_criterion() const { return criterion_pond.size(); }

size_t Domain::get_element() const { return element_pond.size(); }

size_t Domain::get_integrator() const { return integrator_pond.size(); }

size_t Domain::get_load() const { return load_pond.size(); }

size_t Domain::get_material() const { return material_pond.size(); }

size_t Domain::get_node() const { return node_pond.size(); }

size_t Domain::get_recorder() const { return recorder_pond.size(); }

size_t Domain::get_section() const { return section_pond.size(); }

size_t Domain::get_solver() const { return solver_pond.size(); }

size_t Domain::get_step() const { return step_pond.size(); }

bool Domain::find_amplitude(const unsigned& T) const { return amplitude_pond.find(T); }

bool Domain::find_constraint(const unsigned& T) const { return constraint_pond.find(T); }

bool Domain::find_converger(const unsigned& T) const { return converger_pond.find(T); }

bool Domain::find_criterion(const unsigned& T) const { return criterion_pond.find(T); }

bool Domain::find_element(const unsigned& T) const { return element_pond.find(T); }

bool Domain::find_integrator(const unsigned& T) const { return integrator_pond.find(T); }

bool Domain::find_load(const unsigned& T) const { return load_pond.find(T); }

bool Domain::find_material(const unsigned& T) const { return material_pond.find(T); }

bool Domain::find_node(const unsigned& T) const { return node_pond.find(T); }

bool Domain::find_recorder(const unsigned& T) const { return recorder_pond.find(T); }

bool Domain::find_section(const unsigned& T) const { return section_pond.find(T); }

bool Domain::find_solver(const unsigned& T) const { return solver_pond.find(T); }

bool Domain::find_step(const unsigned& T) const { return step_pond.find(T) != step_pond.end(); }

void Domain::set_current_step_tag(const unsigned& T) { current_step_tag = T; }

void Domain::set_current_converger_tag(const unsigned& T) { current_converger_tag = T; }

void Domain::set_current_integrator_tag(const unsigned& T) { current_integrator_tag = T; }

void Domain::set_current_solver_tag(const unsigned& T) { current_solver_tag = T; }

const unsigned& Domain::get_current_step_tag() { return current_step_tag; }

const unsigned& Domain::get_current_converger_tag() { return current_converger_tag; }

const unsigned& Domain::get_current_integrator_tag() { return current_integrator_tag; }

const unsigned& Domain::get_current_solver_tag() { return current_solver_tag; }

const shared_ptr<Step>& Domain::get_current_step() const { return get_step(current_step_tag); }

const shared_ptr<Converger>& Domain::get_current_converger() const { return get_converger(current_converger_tag); }

const shared_ptr<Integrator>& Domain::get_current_integrator() const { return get_integrator(current_integrator_tag); }

const shared_ptr<Solver>& Domain::get_current_solver() const { return get_solver(current_solver_tag); }

bool Domain::insert_loaded_dof(const unsigned& T) { return loaded_dofs.insert(T).second; }

bool Domain::insert_restrained_dof(const unsigned& T) { return restrained_dofs.insert(T).second; }

bool Domain::insert_constrained_dof(const unsigned& T) { return constrained_dofs.insert(T).second; }

const unordered_set<unsigned>& Domain::get_loaded_dof() const { return loaded_dofs; }

const unordered_set<unsigned>& Domain::get_restrained_dof() const { return restrained_dofs; }

const unordered_set<unsigned>& Domain::get_constrained_dof() const { return constrained_dofs; }

const bool& Domain::is_updated() const { return updated; }

int Domain::initialize() {
    if(updated) return 0;

    suanpan_for_each(material_pond.cbegin(), material_pond.cend(), [&](const std::pair<unsigned, shared_ptr<Material>>& t_material) {
        if(!t_material.second->initialized) {
            t_material.second->Material::initialize(shared_from_this());
            t_material.second->initialize(shared_from_this());
        }
    });

    suanpan_for_each(section_pond.cbegin(), section_pond.cend(), [&](const std::pair<unsigned, shared_ptr<Section>>& t_section) {
        t_section.second->Section::initialize(shared_from_this());
        if(t_section.second->is_active()) t_section.second->initialize(shared_from_this());
    });

    suanpan_for_each(node_pond.cbegin(), node_pond.cend(), [](const std::pair<unsigned, shared_ptr<Node>>& t_node) { t_node.second->set_dof_number(0); });

    suanpan_for_each(element_pond.cbegin(), element_pond.cend(), [&](const std::pair<unsigned, shared_ptr<Element>>& t_element) {
        if(t_element.second->is_active()) t_element.second->Element::initialize(shared_from_this());
    });

    suanpan_for_each(node_pond.cbegin(), node_pond.cend(), [&](const std::pair<unsigned, shared_ptr<Node>>& t_node) { t_node.second->initialize(shared_from_this()); });

    // ASSIGN DOF LABEL FOR ACTIVE DOF
    unsigned dof_counter = 0;
    for(const auto& t_node : node_pond) t_node.second->set_original_dof(dof_counter);
    if(dof_counter == 0) return -1;

    suanpan_for_each(load_pond.cbegin(), load_pond.cend(), [&](const std::pair<unsigned, shared_ptr<Load>>& t_load) { t_load.second->initialize(shared_from_this()); });

    // ACTIVE FLAG IS NOW PROPERLY SET FOR NODE AND ELEMENT
    amplitude_pond.update();
    constraint_pond.update();
    converger_pond.update();
    criterion_pond.update();
    element_pond.update();
    integrator_pond.update();
    load_pond.update();
    material_pond.update();
    node_pond.update();
    recorder_pond.update();
    section_pond.update();
    solver_pond.update();

    // RCM OPTIMIZATION
    // COLLECT CONNECTIVITY
    vector<unordered_set<uword>> adjacency(dof_counter);
    for(const auto& t_element : element_pond.get()) {
        t_element->update_dof_encoding();
        auto& t_encoding = t_element->get_dof_encoding();
        for(const auto& i : t_encoding)
            for(const auto& j : t_encoding) adjacency[i].insert(j);
    }

    // COUNT NUMBER OF DEGREE
    uvec num_degree(dof_counter);
    for(unsigned i = 0; i < dof_counter; ++i) num_degree(i) = adjacency[i].size();

    // SORT EACH COLUMN ACCORDING TO ITS DEGREE
    vector<uvec> adjacency_sorted;
    adjacency_sorted.reserve(dof_counter);
    for(unsigned i = 0; i < dof_counter; ++i) {
        uvec t_vec(num_degree(i));
        unsigned j = 0;
        for(const auto& k : adjacency[i]) t_vec(j++) = k;
        adjacency_sorted.emplace_back(t_vec(sort_index(num_degree(t_vec))));
    }

    auto idx_rcm = RCM(adjacency_sorted, num_degree);
    uvec idx_sorted = sort_index(idx_rcm);

    // GET BANDWODTH
    auto low_bw = 1, up_bw = 1;
    for(unsigned i = 0; i < dof_counter; ++i) {
        for(const auto& j : adjacency[idx_rcm(i)]) {
            const int t_bw = int(idx_sorted(j)) - i;
            if(t_bw > low_bw)
                low_bw = t_bw;
            else if(t_bw < up_bw)
                up_bw = t_bw;
        }
    }

    // ASSIGN NEW LABELS TO ACTIVE NODES
    auto& t_node_pond = node_pond.get();
    suanpan_for_each(t_node_pond.cbegin(), t_node_pond.cend(), [&](const shared_ptr<Node>& t_node) { t_node->set_reordered_dof(idx_sorted(t_node->get_original_dof())); });

    // INITIALIZE DERIVED ELEMENTS
    auto& t_element_pond = element_pond.get();
    suanpan_for_each(t_element_pond.cbegin(), t_element_pond.cend(), [&](const shared_ptr<Element>& t_element) {
        if(!t_element->initialized) {
            t_element->initialize(shared_from_this());
            access::rw(t_element->initialized) = true;
        }
        t_element->update_dof_encoding();
        t_element->update_status();
    });

    factory->set_size(dof_counter);

    factory->set_bandwidth(unsigned(low_bw), unsigned(-up_bw));

    auto code = 0;
    suanpan_for_each(step_pond.cbegin(), step_pond.cend(), [&](const std::pair<unsigned, shared_ptr<Step>>& t_step) {
        t_step.second->set_domain(shared_from_this());
        code += t_step.second->initialize();
    });
    if(code != 0) return -1;

    updated = true;

    return 0;
}

int Domain::process_load() {
    loaded_dofs.clear();

    get_trial_load(factory).zeros();

    auto code = 0;
    for(const auto& I : load_pond.get()) code += I->process(shared_from_this());

    return code;
}

int Domain::process_constraint() {
    restrained_dofs.clear();
    constrained_dofs.clear();

    auto code = 0;
    for(const auto& I : constraint_pond.get()) code += I->process(shared_from_this());

    return code;
}

int Domain::process_criterion() {
    auto code = 0;
    for(const auto& I : criterion_pond.get()) code += I->process(shared_from_this());

    return code;
}

void Domain::record() {
    for(const auto& I : recorder_pond.get())
        if(I->is_active()) I->record(shared_from_this());
}

void Domain::enable_all() {
    if(updated) updated = false;

    amplitude_pond.enable();
    constraint_pond.enable();
    converger_pond.enable();
    criterion_pond.enable();
    element_pond.enable();
    integrator_pond.enable();
    load_pond.enable();
    material_pond.enable();
    node_pond.enable();
    recorder_pond.enable();
    section_pond.enable();
    solver_pond.enable();
}

void Domain::summary() const {
    suanpan_info("Domain %u contains:\n\t%u nodes, %u elements, %u materials,\n", get_tag(), get_node(), get_element(), get_material());
    suanpan_info("\t%u loads, %u constraints and %u recorders.\n", get_load(), get_constraint(), get_recorder());
}

void Domain::assemble_resistance() const {
    get_trial_resistance(factory).zeros();
    for(const auto& I : element_pond.get()) factory->assemble_resistance(I->get_resistance(), I->get_dof_encoding());
    factory->set_sushi(factory->get_trial_resistance());
}

void Domain::assemble_mass() const {
    factory->clear_mass();
    for(const auto& I : element_pond.get()) factory->assemble_mass(I->get_mass(), I->get_dof_encoding());
}

void Domain::assemble_initial_stiffness() const {
    factory->clear_stiffness();
    for(const auto& I : element_pond.get()) factory->assemble_stiffness(I->get_initial_stiffness(), I->get_dof_encoding());
}

void Domain::assemble_stiffness() const {
    factory->clear_stiffness();
    for(const auto& I : element_pond.get()) factory->assemble_stiffness(I->get_stiffness(), I->get_dof_encoding());
}

void Domain::assemble_damping() const {
    factory->clear_damping();
    for(const auto& I : element_pond.get()) factory->assemble_damping(I->get_damping(), I->get_dof_encoding());
}

void Domain::erase_machine_error() const {
    auto& t_ninja = get_ninja(factory);
    for(const auto& I : restrained_dofs) t_ninja(I) = 0.;
}

int Domain::update_trial_status() const {
    const auto& analysis_type = factory->get_analysis_type();

    auto& trial_dsp = factory->get_trial_displacement();
    auto& trial_vel = factory->get_trial_velocity();
    auto& trial_acc = factory->get_trial_acceleration();
    auto& trial_res = factory->get_trial_resistance();

    auto& t_node_pool = node_pond.get();
    auto& t_element_pool = element_pond.get();

    if(analysis_type == AnalysisType::STATICS)
        suanpan_for_each(t_node_pool.cbegin(), t_node_pool.cend(), [&](const shared_ptr<Node>& t_node) {
            t_node->update_trial_status(trial_dsp);
            t_node->update_trial_resistance(trial_res);
        });
    else if(analysis_type == AnalysisType::DYNAMICS)
        suanpan_for_each(t_node_pool.cbegin(), t_node_pool.cend(), [&](const shared_ptr<Node>& t_node) {
            t_node->update_trial_status(trial_dsp, trial_vel, trial_acc);
            t_node->update_trial_resistance(trial_res);
        });

    auto code = 0;

    suanpan_for_each(t_element_pool.cbegin(), t_element_pool.cend(), [&](const shared_ptr<Element>& t_element) { code += t_element->update_status(); });

    return code;
}

int Domain::update_incre_status() const {
    const auto& analysis_type = factory->get_analysis_type();

    auto& incre_dsp = factory->get_incre_displacement();
    auto& incre_vel = factory->get_incre_velocity();
    auto& incre_acc = factory->get_incre_acceleration();
    auto& incre_res = factory->get_incre_resistance();

    auto& t_node_pool = node_pond.get();
    auto& t_element_pool = element_pond.get();

    if(analysis_type == AnalysisType::STATICS)
        suanpan_for_each(t_node_pool.cbegin(), t_node_pool.cend(), [&](const shared_ptr<Node>& t_node) {
            t_node->update_incre_status(incre_dsp);
            t_node->update_incre_resistance(incre_res);
        });
    else if(analysis_type == AnalysisType::DYNAMICS)
        suanpan_for_each(t_node_pool.cbegin(), t_node_pool.cend(), [&](const shared_ptr<Node>& t_node) {
            t_node->update_incre_status(incre_dsp, incre_vel, incre_acc);
            t_node->update_incre_resistance(incre_res);
        });

    auto code = 0;

    suanpan_for_each(t_element_pool.cbegin(), t_element_pool.cend(), [&](const shared_ptr<Element>& t_element) { code += t_element->update_status(); });

    return code;
}

int Domain::update_current_status() const {
    const auto& analysis_type = factory->get_analysis_type();

    vec c_g_dsp(factory->get_size(), fill::zeros);
    vec c_g_vel(factory->get_size(), fill::zeros);
    vec c_g_acc(factory->get_size(), fill::zeros);

    if(analysis_type == AnalysisType::STATICS) {
        for(const auto& I : node_pond.get()) {
            auto& t_dof = I->get_reordered_dof();
            auto& current_dsp = I->get_current_displacement();
            for(auto J = 0; J < t_dof.size(); ++J) c_g_dsp(t_dof(J)) = current_dsp(J);
        }
        factory->update_current_displacement(c_g_dsp);
    } else if(analysis_type == AnalysisType::DYNAMICS) {
        for(const auto& I : node_pond.get()) {
            auto& t_dof = I->get_reordered_dof();
            auto& current_dsp = I->get_current_displacement();
            auto& current_vel = I->get_current_velocity();
            auto& current_acc = I->get_current_acceleration();
            for(auto J = 0; J < t_dof.size(); ++J) {
                c_g_dsp(t_dof(J)) = current_dsp(J);
                c_g_vel(t_dof(J)) = current_vel(J);
                c_g_acc(t_dof(J)) = current_acc(J);
            }
        }
        factory->update_current_displacement(c_g_dsp);
        factory->update_current_velocity(c_g_vel);
        factory->update_current_acceleration(c_g_acc);
    }

    return 0;
}

void Domain::commit_status() const {
    factory->commit_status();

    auto& t_node_pool = node_pond.get();
    auto& t_element_pool = element_pond.get();

    suanpan_for_each(t_node_pool.cbegin(), t_node_pool.cend(), [](const shared_ptr<Node>& t_node) { t_node->commit_status(); });
    suanpan_for_each(t_element_pool.cbegin(), t_element_pool.cend(), [](const shared_ptr<Element>& t_element) { t_element->commit_status(); });
}

void Domain::clear_status() const {
    factory->clear_status();

    auto& t_node_pool = node_pond.get();
    auto& t_element_pool = element_pond.get();

    suanpan_for_each(t_node_pool.cbegin(), t_node_pool.cend(), [](const shared_ptr<Node>& t_node) { t_node->clear_status(); });
    suanpan_for_each(t_element_pool.cbegin(), t_element_pool.cend(), [](const shared_ptr<Element>& t_element) {
        t_element->clear_status();
        t_element->update_status();
    });
}

void Domain::reset_status() const {
    factory->reset_status();

    auto& t_node_pool = node_pond.get();
    auto& t_element_pool = element_pond.get();

    suanpan_for_each(t_node_pool.cbegin(), t_node_pool.cend(), [](const shared_ptr<Node>& t_node) { t_node->reset_status(); });
    suanpan_for_each(t_element_pool.cbegin(), t_element_pool.cend(), [](const shared_ptr<Element>& t_element) {
        t_element->reset_status();
        t_element->update_status();
    });
}

shared_ptr<Amplitude>& get_amplitude(const shared_ptr<Domain>& D, const unsigned& T) { return D->amplitude_pond[T]; }

shared_ptr<Constraint>& get_constraint(const shared_ptr<Domain>& D, const unsigned& T) { return D->constraint_pond[T]; }

shared_ptr<Converger>& get_converger(const shared_ptr<Domain>& D, const unsigned& T) { return D->converger_pond[T]; }

shared_ptr<Criterion>& get_criterion(const shared_ptr<Domain>& D, const unsigned& T) { return D->criterion_pond[T]; }

shared_ptr<Element>& get_element(const shared_ptr<Domain>& D, const unsigned& T) { return D->element_pond[T]; }

shared_ptr<Integrator>& get_integrator(const shared_ptr<Domain>& D, const unsigned& T) { return D->integrator_pond[T]; }

shared_ptr<Load>& get_load(const shared_ptr<Domain>& D, const unsigned& T) { return D->load_pond[T]; }

shared_ptr<Material>& get_material(const shared_ptr<Domain>& D, const unsigned& T) { return D->material_pond[T]; }

shared_ptr<Node>& get_node(const shared_ptr<Domain>& D, const unsigned& T) { return D->node_pond[T]; }

shared_ptr<Recorder>& get_recorder(const shared_ptr<Domain>& D, const unsigned& T) { return D->recorder_pond[T]; }

shared_ptr<Section>& get_section(const shared_ptr<Domain>& D, const unsigned& T) { return D->section_pond[T]; }

shared_ptr<Solver>& get_solver(const shared_ptr<Domain>& D, const unsigned& T) { return D->solver_pond[T]; }

shared_ptr<Step>& get_step(const shared_ptr<Domain>& D, const unsigned& T) { return D->step_pond[T]; }

shared_ptr<Amplitude>& get_amplitude(const shared_ptr<DomainBase>& D, const unsigned& T) { return std::dynamic_pointer_cast<Domain>(D)->amplitude_pond[T]; }

shared_ptr<Constraint>& get_constraint(const shared_ptr<DomainBase>& D, const unsigned& T) { return std::dynamic_pointer_cast<Domain>(D)->constraint_pond[T]; }

shared_ptr<Converger>& get_converger(const shared_ptr<DomainBase>& D, const unsigned& T) { return std::dynamic_pointer_cast<Domain>(D)->converger_pond[T]; }

shared_ptr<Criterion>& get_criterion(const shared_ptr<DomainBase>& D, const unsigned& T) { return std::dynamic_pointer_cast<Domain>(D)->criterion_pond[T]; }

shared_ptr<Element>& get_element(const shared_ptr<DomainBase>& D, const unsigned& T) { return std::dynamic_pointer_cast<Domain>(D)->element_pond[T]; }

shared_ptr<Integrator>& get_integrator(const shared_ptr<DomainBase>& D, const unsigned& T) { return std::dynamic_pointer_cast<Domain>(D)->integrator_pond[T]; }

shared_ptr<Load>& get_load(const shared_ptr<DomainBase>& D, const unsigned& T) { return std::dynamic_pointer_cast<Domain>(D)->load_pond[T]; }

shared_ptr<Material>& get_material(const shared_ptr<DomainBase>& D, const unsigned& T) { return std::dynamic_pointer_cast<Domain>(D)->material_pond[T]; }

shared_ptr<Node>& get_node(const shared_ptr<DomainBase>& D, const unsigned& T) { return std::dynamic_pointer_cast<Domain>(D)->node_pond[T]; }

shared_ptr<Recorder>& get_recorder(const shared_ptr<DomainBase>& D, const unsigned& T) { return std::dynamic_pointer_cast<Domain>(D)->recorder_pond[T]; }

shared_ptr<Section>& get_section(const shared_ptr<DomainBase>& D, const unsigned& T) { return std::dynamic_pointer_cast<Domain>(D)->section_pond[T]; }

shared_ptr<Solver>& get_solver(const shared_ptr<DomainBase>& D, const unsigned& T) { return std::dynamic_pointer_cast<Domain>(D)->solver_pond[T]; }

shared_ptr<Step>& get_step(const shared_ptr<DomainBase>& D, const unsigned& T) { return std::dynamic_pointer_cast<Domain>(D)->step_pond[T]; }
