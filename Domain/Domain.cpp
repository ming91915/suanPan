#include "Domain.h"
#include <Constraint/Constraint.h>
#include <Domain/Factory.hpp>
#include <Domain/Node.h>
#include <Domain/Workshop.h>
#include <Element/Element.h>
#include <Load/Load.h>
#include <Material/Material.h>
#include <Recorder/Recorder.h>
#include <Toolbox/RCM.h>

Domain::Domain(const unsigned& T)
    : Tag(T, CT_DOMAIN) {
    suanpan_debug("Domain %u ctor() called.\n", T);
}

Domain::~Domain() { suanpan_debug("Domain %u dtor() called.\n", get_tag()); }

const bool& Domain::is_updated() const { return updated; }

const bool& Domain::is_initialized() const { return initialized; }

int Domain::initialize() {
    if(!initialized) initialized = true;

    if(updated) return 0;

    updated = true;

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

    constraint_pond.update();
    element_pond.update();
    load_pond.update();
    node_pond.update();

    // RCM OPTIMIZATION
    vector<unordered_set<uword>> adjacency(dof_counter);
    for(const auto& t_element : element_pond.get()) {
        t_element->update_dof_encoding();
        auto& t_encoding = t_element->get_dof_encoding();
        for(const auto& i : t_encoding)
            for(const auto& j : t_encoding) adjacency[i].insert(j);
    }

    uvec num_degree(dof_counter);
    for(unsigned i = 0; i < dof_counter; ++i) num_degree(i) = adjacency[i].size();

    vector<uvec> adjacency_sorted(dof_counter);
    for(unsigned i = 0; i < dof_counter; ++i) {
        uvec t_vec(num_degree(i));
        unsigned j = 0;
        for(const auto& k : adjacency[i]) t_vec(j++) = k;
        adjacency_sorted[i] = t_vec(sort_index(num_degree(t_vec)));
    }

    auto idx_rcm = RCM(adjacency_sorted, num_degree);
    uvec idx_sorted = sort_index(idx_rcm);

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
        t_element->initialize(shared_from_this());
        t_element->update_dof_encoding();
    }

    if(workroom == nullptr)
        workroom = make_shared<Workshop>(dof_counter);
    else
        workroom->set_dof_number(dof_counter);

    workroom->set_bandwidth(static_cast<unsigned>(low_bw), static_cast<unsigned>(-up_bw));

    if(factory == nullptr)
        factory = make_shared<Factory<double>>(dof_counter);
    else
        factory->set_size(dof_counter);

    factory->set_bandwidth(static_cast<unsigned>(low_bw), static_cast<unsigned>(-up_bw));

    return 0;
}

void Domain::process(const unsigned& ST) {
    loaded_dofs.clear();
    restrained_dofs.clear();
    constrained_dofs.clear();

    get_trial_load(workroom).zeros();

    for(const auto& I : load_pond.get())
        if(I->get_step_tag() <= ST) I->process(shared_from_this());
    for(const auto& I : constraint_pond.get())
        if(I->get_step_tag() <= ST) I->process(shared_from_this());
}

void Domain::record() {
    for(const auto& I : recorder_pond.get()) I->record(shared_from_this());
}

void Domain::set_workshop(const shared_ptr<Workshop>& W) {
    workroom = W;
    updated = false;
}

const shared_ptr<Workshop>& Domain::get_workshop() const { return workroom; }

void Domain::set_factory(const shared_ptr<Factory<double>>& F) {
    factory = F;
    updated = false;
}

const shared_ptr<Factory<double>>& Domain::get_factory() const { return factory; }

bool Domain::insert(const shared_ptr<Constraint>& C) { return constraint_pond.insert(C); }

bool Domain::insert(const shared_ptr<Element>& E) { return element_pond.insert(E); }

bool Domain::insert(const shared_ptr<Load>& L) { return load_pond.insert(L); }

bool Domain::insert(const shared_ptr<Material>& M) { return material_pond.insert(M); }

bool Domain::insert(const shared_ptr<Node>& N) { return node_pond.insert(N); }

bool Domain::insert(const shared_ptr<Recorder>& R) { return recorder_pond.insert(R); }

bool Domain::erase_constraint(const unsigned& T) { return constraint_pond.erase(T); }

bool Domain::erase_element(const unsigned& T) { return element_pond.erase(T); }

bool Domain::erase_load(const unsigned& T) { return load_pond.erase(T); }

bool Domain::erase_material(const unsigned& T) { return material_pond.erase(T); }

bool Domain::erase_node(const unsigned& T) { return node_pond.erase(T); }

bool Domain::erase_recorder(const unsigned& T) { return recorder_pond.erase(T); }

void Domain::disable_constraint(const unsigned& T) { constraint_pond.disable(T); }

void Domain::disable_element(const unsigned& T) { element_pond.disable(T); }

void Domain::disable_load(const unsigned& T) { load_pond.disable(T); }

void Domain::disable_material(const unsigned& T) { material_pond.disable(T); }

void Domain::disable_node(const unsigned& T) { node_pond.disable(T); }

void Domain::disable_recorder(const unsigned& T) { recorder_pond.disable(T); }

void Domain::enable_all() {
    constraint_pond.enable();
    element_pond.enable();
    load_pond.enable();
    material_pond.enable();
    node_pond.enable();
    recorder_pond.enable();
}

void Domain::enable_constraint(const unsigned& T) { constraint_pond.enable(T); }

void Domain::enable_element(const unsigned& T) { element_pond.enable(T); }

void Domain::enable_load(const unsigned& T) { load_pond.enable(T); }

void Domain::enable_material(const unsigned& T) { material_pond.enable(T); }

void Domain::enable_node(const unsigned& T) { node_pond.enable(T); }

void Domain::enable_recorder(const unsigned& T) { recorder_pond.enable(T); }

const shared_ptr<Constraint>& Domain::get_constraint(const unsigned& T) const { return constraint_pond.at(T); }

const shared_ptr<Element>& Domain::get_element(const unsigned& T) const { return element_pond.at(T); }

const shared_ptr<Load>& Domain::get_load(const unsigned& T) const { return load_pond.at(T); }

const shared_ptr<Material>& Domain::get_material(const unsigned& T) const { return material_pond.at(T); }

const shared_ptr<Node>& Domain::get_node(const unsigned& T) const { return node_pond.at(T); }

const shared_ptr<Recorder>& Domain::get_recorder(const unsigned& T) const { return recorder_pond.at(T); }

size_t Domain::get_constraint() const { return constraint_pond.size(); }

size_t Domain::get_element() const { return element_pond.size(); }

size_t Domain::get_load() const { return load_pond.size(); }

size_t Domain::get_material() const { return material_pond.size(); }

size_t Domain::get_node() const { return node_pond.size(); }

size_t Domain::get_recorder() const { return recorder_pond.size(); }

bool Domain::find_constraint(const unsigned& T) const { return constraint_pond.find(T); }

bool Domain::find_element(const unsigned& T) const { return element_pond.find(T); }

bool Domain::find_load(const unsigned& T) const { return load_pond.find(T); }

bool Domain::find_material(const unsigned& T) const { return material_pond.find(T); }

bool Domain::find_node(const unsigned& T) const { return node_pond.find(T); }

bool Domain::find_recorder(const unsigned& T) const { return recorder_pond.find(T); }

void Domain::update_resistance() const {
    workroom->clear_resistance();
    for(const auto& I : element_pond.get()) workroom->assemble_resistance(I->get_resistance(), I->get_dof_encoding());
}

void Domain::update_mass() const {
    workroom->clear_mass();
    for(const auto& I : element_pond.get()) workroom->assemble_mass(I->get_mass(), I->get_dof_encoding());
}

void Domain::update_initial_stiffness() const {
    workroom->clear_stiffness();
    for(const auto& I : element_pond.get()) workroom->assemble_stiffness(I->get_initial_stiffness(), I->get_dof_encoding());
}

void Domain::update_stiffness() const {
    workroom->clear_stiffness();
    for(const auto& I : element_pond.get()) workroom->assemble_stiffness(I->get_stiffness(), I->get_dof_encoding());
    factory->clear_stiffness();
    for(const auto& I : element_pond.get()) factory->assemble_stiffness(I->get_stiffness(), I->get_dof_encoding());
}

void Domain::update_damping() const {
    workroom->clear_damping();
    for(const auto& I : element_pond.get()) workroom->assemble_damping(I->get_damping(), I->get_dof_encoding());
}

void Domain::update_trial_time(const double& T) const { workroom->update_trial_time(T); }

void Domain::update_incre_time(const double& T) const { workroom->update_incre_time(T); }

void Domain::update_trial_status() const {
    auto& trial_dsp = workroom->get_trial_displacement();
    auto& trial_vel = workroom->get_trial_velocity();
    auto& trial_acc = workroom->get_trial_acceleration();

    if(!trial_dsp.is_empty()) {
        if(!trial_acc.is_empty() && !trial_vel.is_empty()) {
#ifdef SUANPAN_OPENMP
            auto& tmp_pond = node_pond.get();
#pragma omp parallel for
            for(auto I = 0; I < tmp_pond.size(); ++I) tmp_pond[I]->update_trial_status(trial_dsp, trial_vel, trial_acc);
#else
            for(const auto& I : node_pond.get()) I->update_trial_status(trial_dsp, trial_vel, trial_acc);
#endif
        } else {
#ifdef SUANPAN_OPENMP
            auto& tmp_pond = node_pond.get();
#pragma omp parallel for
            for(auto I = 0; I < tmp_pond.size(); ++I) tmp_pond[I]->update_trial_status(trial_dsp);
#else
            for(const auto& I : node_pond.get()) I->update_trial_status(trial_dsp);
#endif
        }
#ifdef SUANPAN_OPENMP
        auto& tmp_pond = element_pond.get();
#pragma omp parallel for
        for(auto I = 0; I < tmp_pond.size(); ++I) tmp_pond[I]->update_status();
#else
        for(const auto& I : element_pond.get()) I->update_status();
#endif
    }
}

void Domain::update_incre_status() const {
    auto& incre_dsp = workroom->get_incre_displacement();
    auto& incre_vel = workroom->get_incre_velocity();
    auto& incre_acc = workroom->get_incre_acceleration();

    if(!incre_dsp.is_empty()) {
        if(!incre_acc.is_empty() && !incre_vel.is_empty()) {
#ifdef SUANPAN_OPENMP
            auto& tmp_pond = node_pond.get();
#pragma omp parallel for
            for(auto I = 0; I < tmp_pond.size(); ++I) tmp_pond[I]->update_trial_status(incre_dsp, incre_vel, incre_acc);
#else
            for(const auto& I : node_pond.get()) I->update_incre_status(incre_dsp, incre_vel, incre_acc);
#endif
        } else {
#ifdef SUANPAN_OPENMP
            auto& tmp_pond = node_pond.get();
#pragma omp parallel for
            for(auto I = 0; I < tmp_pond.size(); ++I) tmp_pond[I]->update_trial_status(incre_dsp);
#else
            for(const auto& I : node_pond.get()) I->update_incre_status(incre_dsp);
#endif
        }
#ifdef SUANPAN_OPENMP
        auto& tmp_pond = element_pond.get();
#pragma omp parallel for
        for(auto I = 0; I < tmp_pond.size(); ++I) tmp_pond[I]->update_status();
#else
        for(const auto& I : element_pond.get()) I->update_status();
#endif
    }
}

void Domain::commit_status() const {
    workroom->commit_status();

#ifdef SUANPAN_OPENMP
    auto& tmp_pond_n = node_pond.get();
#pragma omp parallel for
    for(auto I = 0; I < tmp_pond_n.size(); ++I) tmp_pond_n[I]->commit_status();

    auto& tmp_pond_e = element_pond.get();
#pragma omp parallel for
    for(auto I = 0; I < tmp_pond_e.size(); ++I) tmp_pond_e[I]->commit_status();
#else
    for(const auto& I : node_pond.get()) I->commit_status();
    for(const auto& I : element_pond.get()) I->commit_status();
#endif
}

void Domain::clear_status() const {
    workroom->clear_status();

#ifdef SUANPAN_OPENMP
    auto& tmp_pond_n = node_pond.get();
#pragma omp parallel for
    for(auto I = 0; I < tmp_pond_n.size(); ++I) tmp_pond_n[I]->clear_status();

    auto& tmp_pond_e = element_pond.get();
#pragma omp parallel for
    for(auto I = 0; I < tmp_pond_e.size(); ++I) {
        tmp_pond_e[I]->clear_status();
        tmp_pond_e[I]->update_status();
    }
#else
    for(const auto& I : node_pond.get()) I->clear_status();
    for(const auto& I : element_pond.get()) {
        I->clear_status();
        I->update_status();
    }
#endif
}

void Domain::reset_status() const {
    workroom->reset_status();

#ifdef SUANPAN_OPENMP
    auto& tmp_pond_n = node_pond.get();
#pragma omp parallel for
    for(auto I = 0; I < tmp_pond_n.size(); ++I) tmp_pond_n[I]->reset_status();
    auto& tmp_pond_e = element_pond.get();
#pragma omp parallel for
    for(auto I = 0; I < tmp_pond_e.size(); ++I) {
        tmp_pond_e[I]->reset_status();
        tmp_pond_e[I]->update_status();
    }
#else
    for(const auto& I : node_pond.get()) I->reset_status();
    for(const auto& I : element_pond.get()) {
        I->reset_status();
        I->update_status();
    }
#endif
}

bool Domain::insert_loaded_dof(const unsigned& T) {
    auto R = loaded_dofs.insert(T);
    return R.second;
}

bool Domain::insert_restrained_dof(const unsigned& T) {
    auto R = restrained_dofs.insert(T);
    return R.second;
}

bool Domain::insert_constrained_dof(const unsigned& T) {
    auto R = constrained_dofs.insert(T);
    return R.second;
}

const unordered_set<unsigned>& Domain::get_loaded_dof() const { return loaded_dofs; }

const unordered_set<unsigned>& Domain::get_restrained_dof() const { return restrained_dofs; }

const unordered_set<unsigned>& Domain::get_constrained_dof() const { return constrained_dofs; }

void Domain::summary() const {
    suanpan_info("Domain %u contains:\n\t%u nodes, %u elements, %u materials,\n", get_tag(), get_node(), get_element(), get_material());
    suanpan_info("\t%u loads, %u constraints and %u recorders.\n", get_load(), get_constraint(), get_recorder());
}

shared_ptr<Constraint>& get_constraint(const shared_ptr<Domain>& D, const unsigned& T) { return D->constraint_pond[T]; }

shared_ptr<Element>& get_element(const shared_ptr<Domain>& D, const unsigned& T) { return D->element_pond[T]; }

shared_ptr<Load>& get_load(const shared_ptr<Domain>& D, const unsigned& T) { return D->load_pond[T]; }

shared_ptr<Material>& get_material(const shared_ptr<Domain>& D, const unsigned& T) { return D->material_pond[T]; }

shared_ptr<Node>& get_node(const shared_ptr<Domain>& D, const unsigned& T) { return D->node_pond[T]; }

shared_ptr<Recorder>& get_recorder(const shared_ptr<Domain>& D, const unsigned& T) { return D->recorder_pond[T]; }

const vector<shared_ptr<Node>>& get_node_pool(const shared_ptr<Domain>& D) { return D->node_pond.get(); }

const vector<shared_ptr<Element>>& get_element_pool(const shared_ptr<Domain>& D) { return D->element_pond.get(); }