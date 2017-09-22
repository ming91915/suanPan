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
#include <Domain/Factory.hpp>
#include <Domain/Node.h>
#include <Element/Element.h>
#include <Load/Amplitude/Amplitude.h>
#include <Load/Load.h>
#include <Recorder/Recorder.h>
#include <Toolbox/RCM.h>
#ifdef SUANPAN_MSVC
#include <ppl.h>
#endif

Domain::Domain(const unsigned& T)
    : DomainBase(T) {
    suanpan_debug("Domain %u ctor() called.\n", T);
}

Domain::~Domain() { suanpan_debug("Domain %u dtor() called.\n", get_tag()); }

void Domain::set_factory(const FacotoryAnchor& F) {
    if(factory.lock() != F.lock()) {
        factory = F;
        updated = false;
    }
}

const FacotoryAnchor& Domain::get_factory() const { return factory; }

void Domain::set_step_anchor(const StepAnchor& S) { step_anchor = S; }

const StepAnchor& Domain::get_step_anchor() { return step_anchor; }

bool Domain::insert(const shared_ptr<Amplitude>& A) {
    if(updated) updated = false;
    return amplitude_pond.insert(A);
}

bool Domain::insert(const shared_ptr<Constraint>& C) {
    if(updated) updated = false;
    return constraint_pond.insert(C);
}

bool Domain::insert(const shared_ptr<Element>& E) {
    if(updated) updated = false;
    return element_pond.insert(E);
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

bool Domain::erase_amplitude(const unsigned& T) {
    if(updated) updated = false;
    return amplitude_pond.erase(T);
}

bool Domain::erase_constraint(const unsigned& T) {
    if(updated) updated = false;
    return constraint_pond.erase(T);
}

bool Domain::erase_element(const unsigned& T) {
    if(updated) updated = false;
    return element_pond.erase(T);
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

void Domain::disable_amplitude(const unsigned& T) {
    if(updated) updated = false;
    amplitude_pond.disable(T);
}

void Domain::disable_constraint(const unsigned& T) {
    if(updated) updated = false;
    constraint_pond.disable(T);
}

void Domain::disable_element(const unsigned& T) {
    if(updated) updated = false;
    element_pond.disable(T);
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

void Domain::enable_amplitude(const unsigned& T) {
    if(updated) updated = false;
    amplitude_pond.enable(T);
}

void Domain::enable_constraint(const unsigned& T) {
    if(updated) updated = false;
    constraint_pond.enable(T);
}

void Domain::enable_element(const unsigned& T) {
    if(updated) updated = false;
    element_pond.enable(T);
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

const shared_ptr<Amplitude>& Domain::get_amplitude(const unsigned& T) const { return amplitude_pond.at(T); }

const shared_ptr<Constraint>& Domain::get_constraint(const unsigned& T) const { return constraint_pond.at(T); }

const shared_ptr<Element>& Domain::get_element(const unsigned& T) const { return element_pond.at(T); }

const shared_ptr<Load>& Domain::get_load(const unsigned& T) const { return load_pond.at(T); }

const shared_ptr<Material>& Domain::get_material(const unsigned& T) const { return material_pond.at(T); }

const shared_ptr<Node>& Domain::get_node(const unsigned& T) const { return node_pond.at(T); }

const shared_ptr<Recorder>& Domain::get_recorder(const unsigned& T) const { return recorder_pond.at(T); }

const vector<shared_ptr<Amplitude>>& Domain::get_amplitude_pool() const { return amplitude_pond.get(); }

const vector<shared_ptr<Constraint>>& Domain::get_constraint_pool() const { return constraint_pond.get(); }

const vector<shared_ptr<Element>>& Domain::get_element_pool() const { return element_pond.get(); }

const vector<shared_ptr<Load>>& Domain::get_load_pool() const { return load_pond.get(); }

const vector<shared_ptr<Material>>& Domain::get_material_pool() const { return material_pond.get(); }

const vector<shared_ptr<Node>>& Domain::get_node_pool() const { return node_pond.get(); }

const vector<shared_ptr<Recorder>>& Domain::get_recorder_pool() const { return recorder_pond.get(); }

size_t Domain::get_amplitude() const { return amplitude_pond.size(); }

size_t Domain::get_constraint() const { return constraint_pond.size(); }

size_t Domain::get_element() const { return element_pond.size(); }

size_t Domain::get_load() const { return load_pond.size(); }

size_t Domain::get_material() const { return material_pond.size(); }

size_t Domain::get_node() const { return node_pond.size(); }

size_t Domain::get_recorder() const { return recorder_pond.size(); }

bool Domain::find_amplitude(const unsigned& T) const { return amplitude_pond.find(T); }

bool Domain::find_constraint(const unsigned& T) const { return constraint_pond.find(T); }

bool Domain::find_element(const unsigned& T) const { return element_pond.find(T); }

bool Domain::find_load(const unsigned& T) const { return load_pond.find(T); }

bool Domain::find_material(const unsigned& T) const { return material_pond.find(T); }

bool Domain::find_node(const unsigned& T) const { return node_pond.find(T); }

bool Domain::find_recorder(const unsigned& T) const { return recorder_pond.find(T); }

bool Domain::insert_loaded_dof(const unsigned& T) { return loaded_dofs.insert(T).second; }

bool Domain::insert_restrained_dof(const unsigned& T) { return restrained_dofs.insert(T).second; }

bool Domain::insert_constrained_dof(const unsigned& T) { return constrained_dofs.insert(T).second; }

const unordered_set<unsigned>& Domain::get_loaded_dof() const { return loaded_dofs; }

const unordered_set<unsigned>& Domain::get_restrained_dof() const { return restrained_dofs; }

const unordered_set<unsigned>& Domain::get_constrained_dof() const { return constrained_dofs; }

const bool& Domain::is_updated() const { return updated; }

int Domain::initialize() {
    if(updated) return 0;

    // RESET STATUS
    for(const auto& t_node : node_pond) t_node.second->set_dof_number(0);

    // SET DOF NUMBER FOR ACTIVE NODES
    for(const auto& t_element : element_pond)
        if(t_element.second->is_active()) t_element.second->Element::initialize(shared_from_this());

    // ASSIGN DOF LABEL FOR ACTIVE DOF
    unsigned dof_counter = 0;
    for(const auto& t_node : node_pond) {
        t_node.second->initialize(shared_from_this());
        t_node.second->set_original_dof(dof_counter);
    }

    for(const auto& t_load : load_pond) t_load.second->initialize(shared_from_this());

    // ACTIVE FLAG IS NOW PROPERLY SET FOR NODE AND ELEMENT
    amplitude_pond.update();
    constraint_pond.update();
    element_pond.update();
    load_pond.update();
    material_pond.update();
    node_pond.update();
    recorder_pond.update();

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
            const int t_bw = static_cast<int>(idx_sorted(j)) - i;
            if(t_bw > low_bw)
                low_bw = t_bw;
            else if(t_bw < up_bw)
                up_bw = t_bw;
        }
    }

    // ASSIGN NEW LABELS TO ACTIVE NODES
    for(const auto& t_node : node_pond.get()) t_node->set_reordered_dof(idx_sorted(t_node->get_original_dof()));

    // INITIALIZE DERIVED ELEMENTS
    for(const auto& t_element : element_pond.get()) {
        if(!t_element->initialized) {
            t_element->initialize(shared_from_this());
            access::rw(t_element->initialized) = true;
        }
        t_element->update_dof_encoding();
    }

    const auto& t_factory = factory.lock();

    if(t_factory == nullptr) return -1;

    t_factory->set_size(dof_counter);

    t_factory->set_bandwidth(unsigned(low_bw), unsigned(-up_bw));

    updated = true;

    return 0;
}

void Domain::process() {
    loaded_dofs.clear();
    restrained_dofs.clear();
    constrained_dofs.clear();

    get_trial_load(factory.lock()).zeros();

    for(const auto& I : load_pond.get()) I->process(shared_from_this());
    for(const auto& I : constraint_pond.get()) I->process(shared_from_this());
}

void Domain::record() {
    for(const auto& I : recorder_pond.get()) I->record(shared_from_this());
}

void Domain::enable_all() {
    if(updated) updated = false;

    constraint_pond.enable();
    element_pond.enable();
    load_pond.enable();
    material_pond.enable();
    node_pond.enable();
    recorder_pond.enable();
}

void Domain::summary() const {
    suanpan_info("Domain %u contains:\n\t%u nodes, %u elements, %u materials,\n", get_tag(), get_node(), get_element(), get_material());
    suanpan_info("\t%u loads, %u constraints and %u recorders.\n", get_load(), get_constraint(), get_recorder());
}

void Domain::assemble_resistance() const {
    const auto& t_factory = factory.lock();
    get_trial_resistance(t_factory).zeros();
    for(const auto& I : element_pond.get()) t_factory->assemble_resistance(I->get_resistance(), I->get_dof_encoding());
}

void Domain::assemble_mass() const {
    const auto& t_factory = factory.lock();
    t_factory->clear_mass();
    for(const auto& I : element_pond.get()) t_factory->assemble_mass(I->get_mass(), I->get_dof_encoding());
}

void Domain::assemble_initial_stiffness() const {
    auto t_factory = factory.lock();
    t_factory->clear_stiffness();
    for(const auto& I : element_pond.get()) t_factory->assemble_stiffness(I->get_initial_stiffness(), I->get_dof_encoding());
}

void Domain::assemble_stiffness() const {
    const auto& t_factory = factory.lock();
    t_factory->clear_stiffness();
    for(const auto& I : element_pond.get()) t_factory->assemble_stiffness(I->get_stiffness(), I->get_dof_encoding());
}

void Domain::assemble_damping() const {
    const auto& t_factory = factory.lock();
    t_factory->clear_damping();
    for(const auto& I : element_pond.get()) t_factory->assemble_damping(I->get_damping(), I->get_dof_encoding());
}

void Domain::erase_machine_error() const {
    const auto& t_factory = factory.lock();
    auto& t_ninja = get_ninja(t_factory);
    for(const auto& I : restrained_dofs) t_ninja(I) = 0.;
}

void Domain::update_trial_status() const {
    const auto& t_factory = factory.lock();
    const auto& analysis_type = t_factory->get_analysis_type();

    auto& trial_dsp = t_factory->get_trial_displacement();
    auto& trial_vel = t_factory->get_trial_velocity();
    auto& trial_acc = t_factory->get_trial_acceleration();

    auto& t_node_pool = node_pond.get();
    auto& t_element_pool = element_pond.get();

#if defined(SUANPAN_MT) && defined(SUANPAN_MSVC)
    if(analysis_type == AnalysisType::STATICS)
        Concurrency::parallel_for_each(t_node_pool.cbegin(), t_node_pool.cend(), [&](const shared_ptr<Node>& t_node) { t_node->update_trial_status(trial_dsp); });
    else if(analysis_type == AnalysisType::DYNAMICS)
        Concurrency::parallel_for_each(t_node_pool.cbegin(), t_node_pool.cend(), [&](const shared_ptr<Node>& t_node) { t_node->update_trial_status(trial_dsp, trial_vel, trial_acc); });

    Concurrency::parallel_for_each(t_element_pool.cbegin(), t_element_pool.cend(), [](const shared_ptr<Element>& t_element) { t_element->update_status(); });
#else
    if(analysis_type == AnalysisType::STATICS)
        std::for_each(t_node_pool.cbegin(), t_node_pool.cend(), [&](const shared_ptr<Node>& t_node) { t_node->update_trial_status(trial_dsp); });
    else if(analysis_type == AnalysisType::DYNAMICS)
        std::for_each(t_node_pool.cbegin(), t_node_pool.cend(), [&](const shared_ptr<Node>& t_node) { t_node->update_trial_status(trial_dsp, trial_vel, trial_acc); });

    std::for_each(t_element_pool.cbegin(), t_element_pool.cend(), [](const shared_ptr<Element>& t_element) { t_element->update_status(); });
#endif
}

void Domain::update_incre_status() const {
    const auto& t_factory = factory.lock();
    const auto& analysis_type = t_factory->get_analysis_type();

    auto& incre_dsp = t_factory->get_incre_displacement();
    auto& incre_vel = t_factory->get_incre_velocity();
    auto& incre_acc = t_factory->get_incre_acceleration();

    if(analysis_type == AnalysisType::STATICS)
        for(const auto& I : node_pond.get()) I->update_incre_status(incre_dsp);
    else if(analysis_type == AnalysisType::DYNAMICS)
        for(const auto& I : node_pond.get()) I->update_incre_status(incre_dsp, incre_vel, incre_acc);
    for(const auto& I : element_pond.get()) I->update_status();
}

void Domain::update_current_status() const {
    const auto& t_factory = factory.lock();
    const auto& analysis_type = t_factory->get_analysis_type();

    vec c_g_dsp(t_factory->get_size(), fill::zeros);
    vec c_g_vel(t_factory->get_size(), fill::zeros);
    vec c_g_acc(t_factory->get_size(), fill::zeros);

    if(analysis_type == AnalysisType::STATICS) {
        for(const auto& I : node_pond.get()) {
            auto& t_dof = I->get_reordered_dof();
            auto& current_dsp = I->get_current_displacement();
            for(auto J = 0; J < t_dof.size(); ++J) c_g_dsp(t_dof(J)) = current_dsp(J);
        }
        t_factory->update_current_displacement(c_g_dsp);
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
        t_factory->update_current_displacement(c_g_dsp);
        t_factory->update_current_velocity(c_g_vel);
        t_factory->update_current_acceleration(c_g_acc);
    }
}

void Domain::commit_status() const {
    const auto& t_factory = factory.lock();

    t_factory->commit_status();

    auto& t_node_pool = node_pond.get();
    auto& t_element_pool = element_pond.get();

#if defined(SUANPAN_MT) && defined(SUANPAN_MSVC)
    Concurrency::parallel_for_each(t_node_pool.cbegin(), t_node_pool.cend(), [](const shared_ptr<Node>& t_node) { t_node->commit_status(); });
    Concurrency::parallel_for_each(t_element_pool.cbegin(), t_element_pool.cend(), [](const shared_ptr<Element>& t_element) { t_element->commit_status(); });
#else
    std::for_each(t_node_pool.cbegin(), t_node_pool.cend(), [](const shared_ptr<Node>& t_node) { t_node->commit_status(); });
    std::for_each(t_element_pool.cbegin(), t_element_pool.cend(), [](const shared_ptr<Element>& t_element) { t_element->commit_status(); });
#endif
}

void Domain::clear_status() const {
    const auto& t_factory = factory.lock();

    t_factory->clear_status();

    auto& t_node_pool = node_pond.get();
    auto& t_element_pool = element_pond.get();

#if defined(SUANPAN_MT) && defined(SUANPAN_MSVC)
    Concurrency::parallel_for_each(t_node_pool.cbegin(), t_node_pool.cend(), [](const shared_ptr<Node>& t_node) { t_node->clear_status(); });
    Concurrency::parallel_for_each(t_element_pool.cbegin(), t_element_pool.cend(), [](const shared_ptr<Element>& t_element) {
        t_element->clear_status();
        t_element->update_status();
    });
#else
    std::for_each(t_node_pool.cbegin(), t_node_pool.cend(), [](const shared_ptr<Node>& t_node) { t_node->clear_status(); });
    std::for_each(t_element_pool.cbegin(), t_element_pool.cend(), [](const shared_ptr<Element>& t_element) {
        t_element->clear_status();
        t_element->update_status();
    });
#endif
}

void Domain::reset_status() const {
    const auto& t_factory = factory.lock();

    t_factory->reset_status();

    auto& t_node_pool = node_pond.get();
    auto& t_element_pool = element_pond.get();

#if defined(SUANPAN_MT) && defined(SUANPAN_MSVC)
    Concurrency::parallel_for_each(t_node_pool.cbegin(), t_node_pool.cend(), [](const shared_ptr<Node>& t_node) { t_node->reset_status(); });
    Concurrency::parallel_for_each(t_element_pool.cbegin(), t_element_pool.cend(), [](const shared_ptr<Element>& t_element) {
        t_element->reset_status();
        t_element->update_status();
    });
#else
    std::for_each(t_node_pool.cbegin(), t_node_pool.cend(), [](const shared_ptr<Node>& t_node) { t_node->reset_status(); });
    std::for_each(t_element_pool.cbegin(), t_element_pool.cend(), [](const shared_ptr<Element>& t_element) {
        t_element->reset_status();
        t_element->update_status();
    });
#endif
}

shared_ptr<Amplitude>& get_amplitude(const shared_ptr<Domain>& D, const unsigned& T) { return D->amplitude_pond[T]; }

shared_ptr<Constraint>& get_constraint(const shared_ptr<Domain>& D, const unsigned& T) { return D->constraint_pond[T]; }

shared_ptr<Element>& get_element(const shared_ptr<Domain>& D, const unsigned& T) { return D->element_pond[T]; }

shared_ptr<Load>& get_load(const shared_ptr<Domain>& D, const unsigned& T) { return D->load_pond[T]; }

shared_ptr<Material>& get_material(const shared_ptr<Domain>& D, const unsigned& T) { return D->material_pond[T]; }

shared_ptr<Node>& get_node(const shared_ptr<Domain>& D, const unsigned& T) { return D->node_pond[T]; }

shared_ptr<Recorder>& get_recorder(const shared_ptr<Domain>& D, const unsigned& T) { return D->recorder_pond[T]; }

shared_ptr<Amplitude>& get_amplitude(const shared_ptr<DomainBase>& D, const unsigned& T) { return std::dynamic_pointer_cast<Domain>(D)->amplitude_pond[T]; }

shared_ptr<Constraint>& get_constraint(const shared_ptr<DomainBase>& D, const unsigned& T) { return std::dynamic_pointer_cast<Domain>(D)->constraint_pond[T]; }

shared_ptr<Element>& get_element(const shared_ptr<DomainBase>& D, const unsigned& T) { return std::dynamic_pointer_cast<Domain>(D)->element_pond[T]; }

shared_ptr<Load>& get_load(const shared_ptr<DomainBase>& D, const unsigned& T) { return std::dynamic_pointer_cast<Domain>(D)->load_pond[T]; }

shared_ptr<Material>& get_material(const shared_ptr<DomainBase>& D, const unsigned& T) { return std::dynamic_pointer_cast<Domain>(D)->material_pond[T]; }

shared_ptr<Node>& get_node(const shared_ptr<DomainBase>& D, const unsigned& T) { return std::dynamic_pointer_cast<Domain>(D)->node_pond[T]; }

shared_ptr<Recorder>& get_recorder(const shared_ptr<DomainBase>& D, const unsigned& T) { return std::dynamic_pointer_cast<Domain>(D)->recorder_pond[T]; }
