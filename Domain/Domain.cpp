#include "Domain.h"
#include <Constraint/BC/BC.h>
#include <Domain/Node.h>
#include <Domain/Workroom.h>
#include <Element/Element.h>
#include <Load/Load.h>
#include <Material/Material.h>
#include <Recorder/Recorder.h>
#include <Toolbox/RCM.h>

Domain::Domain(const unsigned& T)
    : Tag(T, CT_DOMAIN)
{
    suanpan_debug("Domain %u ctor() called.\n", T);
}

Domain::~Domain() { suanpan_debug("Domain %u dtor() called.\n", get_tag()); }

const bool& Domain::is_updated() const { return updated; }

const bool& Domain::is_initialized() const { return initialized; }

int Domain::initialize()
{
    if(!initialized) initialized = true;

    if(is_updated()) return 0;

    updated = true;

    // RESET STATUS
    for(const auto& tmp_node : node_pool) tmp_node.second->set_dof_number(0);

    // SET DOF NUMBER FOR ACTIVE NODES
    for(const auto& tmp_element : element_pool)
        if(tmp_element.second->is_active())
            tmp_element.second->Element::initialize(shared_from_this());

    // ASSIGN DOF LABEL FOR ACTIVE DOF
    unsigned dof_counter = 0;
    for(const auto& tmp_node : node_pool) {
        tmp_node.second->initialize();
        tmp_node.second->set_original_dof(dof_counter);
    }

    // PUSH IN ALL VALID ACTIVE CONSTRAINTS
    tmp_constraint_pool.clear();
    for(const auto& tmp_constraint : constraint_pool)
        if(tmp_constraint.second->is_active())
            tmp_constraint_pool.push_back(tmp_constraint.second);

    // PUSH IN ALL VALID ACTIVE ELEMENTS
    tmp_element_pool.clear();
    for(const auto& tmp_element : element_pool)
        if(tmp_element.second->is_active())
            tmp_element_pool.push_back(tmp_element.second);

    // PUSH IN ALL VALID ACTIVE LOADS
    tmp_load_pool.clear();
    for(const auto& I : load_pool)
        if(I.second->is_active()) tmp_load_pool.push_back(I.second);

    // PUSH IN ALL VALID ACTIVE NODES
    tmp_node_pool.clear();
    for(const auto& tmp_node : node_pool)
        if(tmp_node.second->is_active()) tmp_node_pool.push_back(tmp_node.second);

    // RCM OPTIMIZATION
    vector<unordered_set<uword>> adjacency(dof_counter);
    for(const auto& tmp_element : tmp_element_pool) {
        tmp_element->update_dof_encoding();
        auto& tmp_encoding = tmp_element->get_dof_encoding();
        for(const auto& i : tmp_encoding)
            for(const auto& j : tmp_encoding) adjacency[i].insert(j);
    }

    uvec num_degree(dof_counter);
    for(unsigned i = 0; i < dof_counter; ++i) num_degree(i) = adjacency[i].size();

    vector<uvec> adjacency_sorted(dof_counter);
    for(unsigned i = 0; i < dof_counter; ++i) {
        uvec tmp_vec(num_degree(i));
        unsigned j = 0;
        for(const auto& k : adjacency[i]) tmp_vec(j++) = k;
        adjacency_sorted[i] = tmp_vec(sort_index(num_degree(tmp_vec)));
    }

    auto idx_rcm = RCM(adjacency_sorted, num_degree);
    uvec idx_sorted = sort_index(idx_rcm);

    auto low_bandwidth = 1, up_bandwidth = 1;
    for(unsigned i = 0; i < dof_counter; ++i) {
        for(const auto& j : adjacency[idx_rcm(i)]) {
            const int tmp_bandwidth = static_cast<int>(idx_sorted(j)) - i;
            if(tmp_bandwidth > low_bandwidth)
                low_bandwidth = tmp_bandwidth;
            else if(tmp_bandwidth < up_bandwidth)
                up_bandwidth = tmp_bandwidth;
        }
    }

    // ASSIGN NEW LABELS TO ACTIVE NODES
    for(const auto& tmp_node : tmp_node_pool)
        tmp_node->set_reordered_dof(idx_sorted(tmp_node->get_original_dof()));

    // INITIALIZE DERIVED ELEMENTS
    for(const auto& tmp_element : tmp_element_pool) {
        tmp_element->initialize(shared_from_this());
        tmp_element->update_dof_encoding();
    }

    if(factory == nullptr)
        factory = make_shared<Workroom>(dof_counter);
    else
        factory->set_dof_number(dof_counter);

    factory->set_bandwidth(low_bandwidth, -up_bandwidth);

    return 0;
}

void Domain::process(const unsigned& ST)
{
    loaded_dofs.clear();
    restrained_dofs.clear();
    constrained_dofs.clear();

    get_trial_load(factory).zeros();

    for(const auto& I : tmp_load_pool)
        if(I->get_step_tag() <= ST) I->process(shared_from_this());
    for(const auto& I : tmp_constraint_pool)
        if(I->get_step_tag() <= ST) I->process(shared_from_this());
}

void Domain::record()
{
    for(const auto& I : recorder_pool)
        if(I.second->is_active()) I.second->record(shared_from_this());
}

void Domain::set_workroom(const shared_ptr<Workroom>& W) { factory = W; }

const shared_ptr<Workroom>& Domain::get_workroom() const { return factory; }

bool Domain::insert(const shared_ptr<Constraint>& C)
{
    auto F = constraint_pool.insert({ C->get_tag(), C });
    if(!F.second)
        suanpan_error("insert() fails to insert Constraint %u.\n", C->get_tag());
    if(updated) updated = false;
    return F.second;
}

bool Domain::insert(const shared_ptr<Element>& E)
{
    auto F = element_pool.insert({ E->get_tag(), E });
    if(!F.second) suanpan_error("insert() fails to insert Element %u.\n", E->get_tag());
    if(updated) updated = false;
    return F.second;
}

bool Domain::insert(const shared_ptr<Load>& L)
{
    auto F = load_pool.insert({ L->get_tag(), L });
    if(!F.second) suanpan_error("insert() fails to insert Load %u.\n", L->get_tag());
    if(updated) updated = false;
    return F.second;
}

bool Domain::insert(const shared_ptr<Material>& M)
{
    auto F = material_pool.insert({ M->get_tag(), M });
    if(!F.second) suanpan_error("insert() fails to insert Material %u.\n", M->get_tag());
    if(updated) updated = false;
    return F.second;
}

bool Domain::insert(const shared_ptr<Node>& N)
{
    auto F = node_pool.insert({ N->get_tag(), N });
    if(!F.second) suanpan_error("insert() fails to insert Node %u.\n", N->get_tag());
    if(updated) updated = false;
    return F.second;
}

bool Domain::insert(const shared_ptr<Recorder>& R)
{
    auto F = recorder_pool.insert({ R->get_tag(), R });
    if(!F.second) suanpan_error("insert() fails to insert Recorder %u.\n", R->get_tag());
    if(updated) updated = false;
    return F.second;
}

void Domain::erase_constraint(const unsigned& T)
{
    if(constraint_pool.erase(T) != 1)
        suanpan_info("erase_constraint() cannot find Constraint %u.\n", T);
    else {
        disabled_constraint.erase(T);
        if(updated) updated = false;
        suanpan_debug("erase_constraint() erases Constraint %u.\n", T);
    }
}

void Domain::erase_element(const unsigned& T)
{
    if(element_pool.erase(T) != 1)
        suanpan_error("erase_element() cannot find Element %u.\n", T);
    else {
        disabled_element.erase(T);
        if(updated) updated = false;
        suanpan_debug("erase_element() erases Element %u.\n", T);
    }
}

void Domain::erase_load(const unsigned& T)
{
    if(load_pool.erase(T) != 1)
        suanpan_info("erase_load() cannot find Load %u.\n", T);
    else {
        disabled_load.erase(T);
        if(updated) updated = false;
        suanpan_debug("erase_load() erases Load %u.\n", T);
    }
}

void Domain::erase_material(const unsigned& T)
{
    if(material_pool.erase(T) != 1)
        suanpan_info("erase_material() cannot find Material %u.\n", T);
    else {
        disabled_material.erase(T);
        if(updated) updated = false;
        suanpan_debug("erase_material() erases Material %u.\n", T);
    }
}

void Domain::erase_node(const unsigned& T)
{
    if(node_pool.erase(T) != 1)
        suanpan_info("erase_node() cannot find Node %u.\n", T);
    else {
        disabled_node.erase(T);
        if(updated) updated = false;
        suanpan_debug("erase_node() erases Node %u.\n", T);
    }
}

void Domain::erase_recorder(const unsigned& T)
{
    if(recorder_pool.erase(T) != 1)
        suanpan_info("erase_recorder() cannot find Recorder %u.\n", T);
    else {
        disabled_recorder.erase(T);
        if(updated) updated = false;
        suanpan_debug("erase_recorder() erases Recorder %u.\n", T);
    }
}

void Domain::disable_constraint(const unsigned& T)
{
    if(constraint_pool.find(T) != constraint_pool.end()) {
        disabled_constraint.insert(T);
        constraint_pool.at(T)->disable();
        suanpan_info("disable_constraint() disables Constraint %u.\n", T);
    } else
        suanpan_debug("disable_constraint() cannot find Constraint %u.\n", T);
}

void Domain::disable_element(const unsigned& T)
{
    if(element_pool.find(T) != element_pool.end()) {
        disabled_element.insert(T);
        element_pool.at(T)->disable();
        suanpan_info("disable_element() disables Element %u.\n", T);
    } else
        suanpan_debug("disable_element() cannot find Element %u.\n", T);
}

void Domain::disable_load(const unsigned& T)
{
    if(load_pool.find(T) != load_pool.end()) {
        disabled_load.insert(T);
        load_pool.at(T)->disable();
        suanpan_info("disable_load() disables Load %u.\n", T);
    } else
        suanpan_debug("disable_load() cannot find Load %u.\n", T);
}

void Domain::disable_material(const unsigned& T)
{
    if(material_pool.find(T) != material_pool.end()) {
        disabled_material.insert(T);
        material_pool.at(T)->disable();
        suanpan_info("disable_material() disables Material %u.\n", T);
    } else
        suanpan_debug("disable_material() cannot find Material %u.\n", T);
}

void Domain::disable_node(const unsigned& T)
{
    if(node_pool.find(T) != node_pool.end()) {
        disabled_node.insert(T);
        node_pool.at(T)->disable();
        suanpan_info("disable_node() disables Node %u.\n", T);
    } else
        suanpan_debug("disable_node() cannot find Node %u.\n", T);
}

void Domain::disable_recorder(const unsigned& T)
{
    if(recorder_pool.find(T) != recorder_pool.end()) {
        disabled_recorder.insert(T);
        recorder_pool.at(T)->disable();
        suanpan_info("disable_recorder() disables Recorder %u.\n", T);
    } else
        suanpan_debug("disable_recorder() cannot find Recorder %u.\n", T);
}

void Domain::enable_all()
{
    for(const auto& I : disabled_constraint) constraint_pool[I]->enable();
    for(const auto& I : disabled_element) element_pool[I]->enable();
    for(const auto& I : disabled_load) load_pool[I]->enable();
    for(const auto& I : disabled_material) material_pool[I]->enable();
    for(const auto& I : disabled_node) node_pool[I]->enable();
    for(const auto& I : disabled_recorder) recorder_pool[I]->enable();

    disabled_constraint.clear();
    disabled_element.clear();
    disabled_load.clear();
    disabled_material.clear();
    disabled_node.clear();
    disabled_recorder.clear();
}

void Domain::enable_constraint(const unsigned& T)
{
    if(disabled_constraint.erase(T) == 1) {
        constraint_pool[T]->enable();
        suanpan_info("enable_constraint() enables Element %u.\n", T);
    }
}

void Domain::enable_element(const unsigned& T)
{
    if(disabled_element.erase(T) == 1) {
        element_pool[T]->enable();
        suanpan_info("enable_element() enables Constraint %u.\n", T);
    }
}

void Domain::enable_load(const unsigned& T)
{
    if(disabled_load.erase(T) == 1) {
        load_pool[T]->enable();
        suanpan_info("enable_load() enables Load %u.\n", T);
    }
}

void Domain::enable_material(const unsigned& T)
{
    if(disabled_material.erase(T) == 1) {
        material_pool[T]->enable();
        suanpan_info("enable_material() enables Material %u.\n", T);
    }
}

void Domain::enable_node(const unsigned& T)
{
    if(disabled_node.erase(T) == 1) {
        node_pool[T]->enable();
        suanpan_info("enable_node() enables Node %u.\n", T);
    }
}

void Domain::enable_recorder(const unsigned& T)
{
    if(disabled_recorder.erase(T) == 1) {
        recorder_pool[T]->enable();
        suanpan_info("enable_recorder() enables Recorder %u.\n", T);
    }
}

const shared_ptr<Constraint>& Domain::get_constraint(const unsigned& T) const
{
    return constraint_pool.at(T);
}

const shared_ptr<Element>& Domain::get_element(const unsigned& T) const
{
    return element_pool.at(T);
}

const shared_ptr<Load>& Domain::get_load(const unsigned& T) const
{
    return load_pool.at(T);
}

const shared_ptr<Material>& Domain::get_material(const unsigned& T) const
{
    return material_pool.at(T);
}

const shared_ptr<Node>& Domain::get_node(const unsigned& T) const
{
    return node_pool.at(T);
}

const shared_ptr<Recorder>& Domain::get_recorder(const unsigned& T) const
{
    return recorder_pool.at(T);
}

unsigned Domain::get_constraint() const
{
    return static_cast<unsigned>(constraint_pool.size());
}

unsigned Domain::get_element() const
{
    return static_cast<unsigned>(element_pool.size());
}

unsigned Domain::get_load() const { return static_cast<unsigned>(load_pool.size()); }

unsigned Domain::get_material() const
{
    return static_cast<unsigned>(material_pool.size());
}

unsigned Domain::get_node() const { return static_cast<unsigned>(node_pool.size()); }

unsigned Domain::get_recorder() const
{
    return static_cast<unsigned>(recorder_pool.size());
}

bool Domain::find_constraint(const unsigned& T) const
{
    return constraint_pool.find(T) != constraint_pool.end();
}

bool Domain::find_element(const unsigned& T) const
{
    return element_pool.find(T) != element_pool.end();
}

bool Domain::find_load(const unsigned& T) const
{
    return load_pool.find(T) != load_pool.end();
}

bool Domain::find_material(const unsigned& T) const
{
    return material_pool.find(T) != material_pool.end();
}

bool Domain::find_node(const unsigned& T) const
{
    return node_pool.find(T) != node_pool.end();
}

bool Domain::find_recorder(const unsigned& T) const
{
    return recorder_pool.find(T) != recorder_pool.end();
}

void Domain::update_resistance() const
{
    factory->clear_resistance();
    for(const auto& I : tmp_element_pool)
        factory->assemble_resistance(I->get_resistance(), I->get_dof_encoding());
}

void Domain::update_mass() const
{
    factory->clear_mass();
    for(const auto& I : tmp_element_pool)
        factory->assemble_mass(I->get_mass(), I->get_dof_encoding());
}

void Domain::update_initial_stiffness() const
{
    factory->clear_stiffness();
    for(const auto& I : tmp_element_pool)
        factory->assemble_stiffness(I->get_initial_stiffness(), I->get_dof_encoding());
}

void Domain::update_stiffness() const
{
    factory->clear_stiffness();
    for(const auto& I : tmp_element_pool)
        factory->assemble_stiffness(I->get_stiffness(), I->get_dof_encoding());
}

void Domain::update_damping() const
{
    factory->clear_damping();
    for(const auto& I : tmp_element_pool)
        factory->assemble_damping(I->get_damping(), I->get_dof_encoding());
}

void Domain::update_trial_time(const double& T) const { factory->update_trial_time(T); }

void Domain::update_incre_time(const double& T) const { factory->update_incre_time(T); }

void Domain::update_trial_status() const
{
    auto& trial_dsp = factory->get_trial_displacement();
    auto& trial_vel = factory->get_trial_velocity();
    auto& trial_acc = factory->get_trial_acceleration();

    if(!trial_dsp.is_empty()) {
        if(!trial_acc.is_empty() && !trial_vel.is_empty()) {
#ifdef SUANPAN_OPENMP
#pragma omp parallel for
            for(auto I = 0; I < tmp_node_pool.size(); ++I)
                tmp_node_pool[I]->update_trial_status(trial_dsp, trial_vel, trial_acc);
#else
            for(const auto& I : tmp_node_pool)
                I->update_trial_status(trial_dsp, trial_vel, trial_acc);
#endif
        } else
#ifdef SUANPAN_OPENMP
#pragma omp parallel for
            for(auto I = 0; I < tmp_node_pool.size(); ++I)
                tmp_node_pool[I]->update_trial_status(trial_dsp);
#else
            for(const auto& I : tmp_node_pool) I->update_trial_status(trial_dsp);
#endif
#ifdef SUANPAN_OPENMP
#pragma omp parallel for
        for(auto I = 0; I < tmp_element_pool.size(); ++I)
            tmp_element_pool[I]->update_status();
#else
        for(const auto& I : tmp_element_pool) I->update_status();
#endif
    }
}

void Domain::update_incre_status() const
{
    auto& incre_dsp = factory->get_incre_displacement();
    auto& incre_vel = factory->get_incre_velocity();
    auto& incre_acc = factory->get_incre_acceleration();

    if(!incre_dsp.is_empty()) {
        if(!incre_acc.is_empty() && !incre_vel.is_empty()) {
#ifdef SUANPAN_OPENMP
#pragma omp parallel for
            for(auto I = 0; I < tmp_node_pool.size(); ++I)
                tmp_node_pool.at(I)->update_trial_status(incre_dsp, incre_vel, incre_acc);
#else
            for(const auto& I : tmp_node_pool)
                I->update_incre_status(incre_dsp, incre_vel, incre_acc);
#endif
        } else
#ifdef SUANPAN_OPENMP
#pragma omp parallel for
            for(auto I = 0; I < tmp_node_pool.size(); ++I)
                tmp_node_pool.at(I)->update_trial_status(incre_dsp);
#else
            for(const auto& I : tmp_node_pool) I->update_incre_status(incre_dsp);
#endif
#ifdef SUANPAN_OPENMP
#pragma omp parallel for
        for(auto I = 0; I < tmp_element_pool.size(); ++I)
            tmp_element_pool.at(I)->update_status();
#else
        for(const auto& I : tmp_element_pool) I->update_status();
#endif
    }
}

void Domain::commit_status()
{
    factory->commit_status();

#ifdef SUANPAN_OPENMP
#pragma omp parallel for
    for(auto I = 0; I < tmp_node_pool.size(); ++I) tmp_node_pool.at(I)->commit_status();
#pragma omp parallel for
    for(auto I = 0; I < tmp_element_pool.size(); ++I)
        tmp_element_pool.at(I)->commit_status();
#else
    for(const auto& I : tmp_node_pool) I->commit_status();
    for(const auto& I : tmp_element_pool) I->commit_status();
#endif
}

void Domain::clear_status()
{
    factory->clear_status();

#ifdef SUANPAN_OPENMP
#pragma omp parallel for
    for(auto I = 0; I < tmp_node_pool.size(); ++I) tmp_node_pool.at(I)->clear_status();
#pragma omp parallel for
    for(auto I = 0; I < tmp_element_pool.size(); ++I) {
        tmp_element_pool.at(I)->clear_status();
        tmp_element_pool.at(I)->update_status();
    }
#else
    for(const auto& I : tmp_node_pool) I->clear_status();
    for(const auto& I : tmp_element_pool) {
        I->clear_status();
        I->update_status();
    }
#endif
}

void Domain::reset_status()
{
    factory->reset_status();

#ifdef SUANPAN_OPENMP
#pragma omp parallel for
    for(auto I = 0; I < tmp_node_pool.size(); ++I) tmp_node_pool.at(I)->reset_status();
#pragma omp parallel for
    for(auto I = 0; I < tmp_element_pool.size(); ++I) {
        tmp_element_pool.at(I)->reset_status();
        tmp_element_pool.at(I)->update_status();
    }
#else
    for(const auto& I : tmp_node_pool) I->reset_status();
    for(const auto& I : tmp_element_pool) {
        I->reset_status();
        I->update_status();
    }
#endif
}

bool Domain::insert_loaded_dof(const unsigned& T)
{
    auto R = loaded_dofs.insert(T);
    return R.second;
}

bool Domain::insert_restrained_dof(const unsigned& T)
{
    auto R = restrained_dofs.insert(T);
    return R.second;
}

bool Domain::insert_constrained_dof(const unsigned& T)
{
    auto R = constrained_dofs.insert(T);
    return R.second;
}

const unordered_set<unsigned>& Domain::get_loaded_dof() const { return loaded_dofs; }

const unordered_set<unsigned>& Domain::get_restrained_dof() const
{
    return restrained_dofs;
}

const unordered_set<unsigned>& Domain::get_constrained_dof() const
{
    return constrained_dofs;
}

void Domain::summary() const
{
    suanpan_info("Domain %u contains:\n\t%u nodes, %u elements, %u materials,\n",
        get_tag(), get_node(), get_element(), get_material());
    suanpan_info("\t%u loads, %u constraints and %u recorders.\n", get_load(),
        get_constraint(), get_recorder());
}

shared_ptr<Constraint>& get_constraint(const shared_ptr<Domain>& D, const unsigned& T)
{
    return D->constraint_pool[T];
}

shared_ptr<Element>& get_element(const shared_ptr<Domain>& D, const unsigned& T)
{
    return D->element_pool[T];
}

shared_ptr<Load>& get_load(const shared_ptr<Domain>& D, const unsigned& T)
{
    return D->load_pool[T];
}

shared_ptr<Material>& get_material(const shared_ptr<Domain>& D, const unsigned& T)
{
    return D->material_pool[T];
}

shared_ptr<Node>& get_node(const shared_ptr<Domain>& D, const unsigned& T)
{
    return D->node_pool[T];
}

shared_ptr<Recorder>& get_recorder(const shared_ptr<Domain>& D, const unsigned& T)
{
    return D->recorder_pool[T];
}

const vector<shared_ptr<Node>>& get_node_pool(const shared_ptr<Domain>& D)
{
    return D->tmp_node_pool;
}
