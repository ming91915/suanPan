#include "Domain.h"
#include "Node.h"
#include "Workroom.h"
#include <Constraint/BC/BC.h>
#include <Element/Element.h>
#include <Load/Load.h>
#include <Load/RCM.h>
#include <Material/Material.h>

Domain::Domain(const unsigned& T)
    : Tag(T, CT_DOMAIN)
{
}

Domain::~Domain()
{
#ifdef SUANPAN_DEBUG
    printf("Domain %u dtor() Called.\n", getTag());
#endif
}

const bool& Domain::is_updated() const { return updated; }

void Domain::initialize()
{
    if(!updated) {
        updated = true;

        // RESET STATUS
        for(const auto& I : node_pool) I.second->setNumberDOF(0);

        // SET DOF NUMBER FOR ACTIVE NODES
        for(const auto& I : element_pool)
            if(I.second->getStatus()) I.second->Element::initialize(shared_from_this());

        // ASSIGN DOF LABEL FOR ACTIVE DOF
        unsigned dof_idx = 0;
        for(const auto& I : node_pool) {
            I.second->initialize();
            I.second->setOriginalDOF(dof_idx);
        }

        tmp_node_pool.clear();
        for(const auto& I : node_pool)
            if(I.second->getStatus()) tmp_node_pool.push_back(I.second);

        tmp_element_pool.clear();
        for(const auto& I : element_pool)
            if(I.second->getStatus()) tmp_element_pool.push_back(I.second);

        // ADJACENCY MATRIX
        sp_umat tmp_mat(dof_idx, dof_idx);

        // LABEL ADJACENCY
        for(const auto& I : tmp_element_pool) {
            I->updateEncodingDOF();
            auto& J = I->getEncodingDOF();
            for(const auto& K : J)
                for(const auto& L : J)
                    if(tmp_mat(K, L) != 1) tmp_mat(K, L) = 1;
        }

        // RCM OPTIMIZATION
        auto idx_rcm = RCM(tmp_mat);
        uvec idx_sorted = sort_index(idx_rcm);

        // ASSIGN NEW LABELS TO ACTIVE NODES
        for(const auto& I : tmp_node_pool)
            I->setReorderDOF(idx_sorted(I->getOriginalDOF()));

        // INITIALIZE DERIVED ELEMENTS
        for(const auto& I : tmp_element_pool) {
            I->initialize(shared_from_this());
            I->updateEncodingDOF();
        }

        //! TODO: FACTORY SHOULD BE INITIALIZED ACCORDING TO THE PROBLEM TYPE.
        if(factory == nullptr)
            factory = make_shared<Workroom>(dof_idx);
        else
            factory->setNumberDOF(dof_idx);

        // FIND BANDWIDTH
        unsigned L = 1, U = 1;
        for(unsigned I = 0; I < dof_idx; ++I) {
            auto TL = I, TU = I;

            for(auto J = I + 1; J < dof_idx; ++J)
                if(tmp_mat(idx_rcm(J), idx_rcm(I)) != 0) TL = J;
            auto TTL = TL - I;
            if(TTL > L) L = TTL;

            for(auto J = static_cast<int>(I) - 1; J >= 0; --J)
                if(tmp_mat(idx_rcm(J), idx_rcm(I)) != 0) TU = J;
            auto TTU = I - TU;
            if(TTU > U) U = TTU;
        }

        factory->setBandwidth(L, U);
    }
}

void Domain::process(const unsigned& ST)
{
    loaded_dofs.clear();
    restrained_dofs.clear();
    constrained_dofs.clear();

    factory->updateTrialLoad(zeros(factory->getNumberDOF()));

    for(const auto& I : load_pool)
        if(I.second->getStepTag() <= ST && I.second->getStatus())
            I.second->process(shared_from_this());
    for(const auto& I : constraint_pool)
        if(I.second->getStepTag() <= ST && I.second->getStatus())
            I.second->process(shared_from_this());
}

void Domain::setWorkroom(const shared_ptr<Workroom>& W) { factory = W; }

const shared_ptr<Workroom>& Domain::getWorkroom() const { return factory; }

void Domain::insert(const shared_ptr<Constraint>& ITEM)
{
    auto F = constraint_pool.insert({ ITEM->getTag(), ITEM });
    if(!F.second) {
        printf("Domain::insert() fails to insert the constraint with tag %u as the "
               "object with the same tag already exists in the model.\n",
            ITEM->getTag());
    }
    updated = false;
}

void Domain::insert(const shared_ptr<Element>& ITEM)
{
    auto F = element_pool.insert({ ITEM->getTag(), ITEM });
    if(!F.second) {
        printf("Domain::insert() fails to insert the element with tag %u as the object "
               "with the same tag already exists in the model.\n",
            ITEM->getTag());
    }
    updated = false;
}

void Domain::insert(const shared_ptr<Load>& ITEM)
{
    auto F = load_pool.insert({ ITEM->getTag(), ITEM });
    if(!F.second) {
        printf("Domain::insert() fails to insert the load with tag %u as the object with "
               "the same tag already exists in the model.\n",
            ITEM->getTag());
    }
    updated = false;
}

void Domain::insert(const shared_ptr<Material>& ITEM)
{
    auto F = material_pool.insert({ ITEM->getTag(), ITEM });
    if(!F.second) {
        printf("Domain::insert() fails to insert the material with tag %u as the object "
               "with the same tag already exists in the model.\n",
            ITEM->getTag());
    }
}

void Domain::insert(const shared_ptr<Node>& ITEM)
{
    auto F = node_pool.insert({ ITEM->getTag(), ITEM });
    if(!F.second) {
        printf("Domain::insert() fails to insert the node with tag %u as the object with "
               "the same tag already exists in the model.\n",
            ITEM->getTag());
    }
}

void Domain::erase_constraint(const unsigned& T)
{
    if(constraint_pool.erase(T) != 1)
        suanpan_error("erase() cannot find the object with given tag.\n");
    updated = false;
}

void Domain::erase_element(const unsigned& T)
{
    if(element_pool.erase(T) != 1)
        suanpan_error("erase() cannot find the object with given tag.\n");
    updated = false;
}

void Domain::erase_load(const unsigned& T)
{
    if(load_pool.erase(T) != 1)
        suanpan_error("erase() cannot find the object with given tag.\n");
    updated = false;
}

void Domain::erase_material(const unsigned& T)
{
    if(material_pool.erase(T) != 1)
        suanpan_error("erase() cannot find the object with given tag.\n");
    updated = false;
}

void Domain::erase_node(const unsigned& T)
{
    if(node_pool.erase(T) != 1)
        suanpan_error("erase() cannot find the object with given tag.\n");
    updated = false;
}

void Domain::disable_constraint(const unsigned& T) { constraint_pool.at(T)->disable(); }

void Domain::disable_element(const unsigned& T)
{
    disabled_element.insert(T);
    element_pool.at(T)->disable();
}

void Domain::disable_load(const unsigned& T) { load_pool.at(T)->disable(); }

void Domain::disable_material(const unsigned& T) { material_pool.at(T)->disable(); }

void Domain::disable_node(const unsigned& T)
{
    disabled_node.insert(T);
    node_pool.at(T)->disable();
}

const shared_ptr<Constraint>& Domain::getConstraint(const unsigned& T) const
{
    return constraint_pool.at(T);
}

const shared_ptr<Element>& Domain::getElement(const unsigned& T) const
{
    return element_pool.at(T);
}

const shared_ptr<Load>& Domain::getLoad(const unsigned& T) const
{
    return load_pool.at(T);
}

const shared_ptr<Material>& Domain::getMaterial(const unsigned& T) const
{
    return material_pool.at(T);
}

const shared_ptr<Node>& Domain::getNode(const unsigned& T) const
{
    return node_pool.at(T);
}

shared_ptr<Constraint>& Domain::getConstraint(const unsigned& T)
{
    return constraint_pool[T];
}

shared_ptr<Element>& Domain::getElement(const unsigned& T) { return element_pool[T]; }

shared_ptr<Load>& Domain::getLoad(const unsigned& T) { return load_pool[T]; }

shared_ptr<Material>& Domain::getMaterial(const unsigned& T) { return material_pool[T]; }

shared_ptr<Node>& Domain::getNode(const unsigned& T) { return node_pool[T]; }

unsigned Domain::getNumberConstraint() const
{
    return static_cast<unsigned>(constraint_pool.size());
}

unsigned Domain::getNumberElement() const
{
    return static_cast<unsigned>(element_pool.size());
}

unsigned Domain::getNumberLoad() const { return static_cast<unsigned>(load_pool.size()); }

unsigned Domain::getNumberMaterial() const
{
    return static_cast<unsigned>(material_pool.size());
}

unsigned Domain::getNumberNode() const { return static_cast<unsigned>(node_pool.size()); }

void Domain::updateResistance() const
{
    factory->clearResistance();
    for(const auto& I : tmp_element_pool)
        factory->assembleResistance(I->getResistance(), I->getEncodingDOF());
}

void Domain::updateMass() const
{
    factory->clearMass();
    for(const auto& I : tmp_element_pool)
        factory->assembleMass(I->getMass(), I->getEncodingDOF());
}

void Domain::updateInitialStiffness() const
{
    factory->clearStiffness();
    for(const auto& I : tmp_element_pool)
        factory->assembleStiffness(I->getInitialStiffness(), I->getEncodingDOF());
}

void Domain::updateStiffness() const
{
    factory->clearStiffness();
    for(const auto& I : tmp_element_pool)
        factory->assembleStiffness(I->getStiffness(), I->getEncodingDOF());
}

void Domain::updateDamping() const
{
    factory->clearDamping();
    for(const auto& I : tmp_element_pool)
        factory->assembleDamping(I->getDamping(), I->getEncodingDOF());
}

void Domain::updateTrialStatus() const
{
    auto& trial_dsp = factory->getTrialDisplacement();
    auto& trial_vel = factory->getTrialVelocity();
    auto& trial_acc = factory->getTrialAcceleration();

    if(!trial_dsp.is_empty()) {
        if(!trial_acc.is_empty() && !trial_vel.is_empty()) {
#ifdef SUANPAN_OPENMP
#pragma omp parallel for
            for(auto I = 0; I < tmp_node_pool.size(); ++I)
                tmp_node_pool.at(I)->updateTrialStatus(trial_dsp, trial_vel, trial_acc);
#else
            for(const auto& I : tmp_node_pool)
                I->updateTrialStatus(trial_dsp, trial_vel, trial_acc);
#endif
        } else
#ifdef SUANPAN_OPENMP
#pragma omp parallel for
            for(auto I = 0; I < tmp_node_pool.size(); ++I)
                tmp_node_pool.at(I)->updateTrialStatus(trial_dsp);
#else
            for(const auto& I : tmp_node_pool) I->updateTrialStatus(trial_dsp);
#endif
#ifdef SUANPAN_OPENMP
#pragma omp parallel for
        for(auto I = 0; I < tmp_element_pool.size(); ++I)
            tmp_element_pool.at(I)->updateStatus();
#else
        for(const auto& I : tmp_element_pool) I->updateStatus();
#endif
    }
}

void Domain::updateIncreStatus() const
{
    auto& incre_dsp = factory->getIncreDisplacement();
    auto& incre_vel = factory->getIncreVelocity();
    auto& incre_acc = factory->getIncreAcceleration();

    if(!incre_dsp.is_empty()) {
        if(!incre_acc.is_empty() && !incre_vel.is_empty()) {
#ifdef SUANPAN_OPENMP
#pragma omp parallel for
            for(auto I = 0; I < tmp_node_pool.size(); ++I)
                tmp_node_pool.at(I)->updateTrialStatus(incre_dsp, incre_vel, incre_acc);
#else
            for(const auto& I : tmp_node_pool)
                I->updateIncreStatus(incre_dsp, incre_vel, incre_acc);
#endif
        } else
#ifdef SUANPAN_OPENMP
#pragma omp parallel for
            for(auto I = 0; I < tmp_node_pool.size(); ++I)
                tmp_node_pool.at(I)->updateTrialStatus(incre_dsp);
#else
            for(const auto& I : tmp_node_pool) I->updateIncreStatus(incre_dsp);
#endif
#ifdef SUANPAN_OPENMP
#pragma omp parallel for
        for(auto I = 0; I < tmp_element_pool.size(); ++I)
            tmp_element_pool.at(I)->updateStatus();
#else
        for(const auto& I : tmp_element_pool) I->updateStatus();
#endif
    }
}

void Domain::commitStatus()
{
    factory->commitStatus();

#ifdef SUANPAN_OPENMP
#pragma omp parallel for
    for(auto I = 0; I < tmp_node_pool.size(); ++I) tmp_node_pool.at(I)->commitStatus();
#pragma omp parallel for
    for(auto I = 0; I < tmp_element_pool.size(); ++I)
        tmp_element_pool.at(I)->commitStatus();
#else
    for(const auto& I : tmp_node_pool) I->commitStatus();
    for(const auto& I : tmp_element_pool) I->commitStatus();
#endif
}

void Domain::clearStatus()
{
    updated = false;

    factory->clearStatus();

    disabled_constraint.clear();
    disabled_element.clear();
    disabled_load.clear();
    disabled_material.clear();
    disabled_node.clear();

    loaded_dofs.clear();
    restrained_dofs.clear();
    constrained_dofs.clear();

#ifdef SUANPAN_OPENMP
#pragma omp parallel for
    for(auto I = 0; I < tmp_node_pool.size(); ++I) tmp_node_pool.at(I)->clearStatus();
#pragma omp parallel for
    for(auto I = 0; I < tmp_element_pool.size(); ++I) {
        tmp_element_pool.at(I)->resetStatus();
        tmp_element_pool.at(I)->updateStatus();
    }
#else
    for(const auto& I : tmp_node_pool) I->clearStatus();
    for(const auto& I : tmp_element_pool) {
        I->resetStatus();
        I->updateStatus();
    }
#endif
}

void Domain::resetStatus()
{
    factory->resetStatus();

#ifdef SUANPAN_OPENMP
#pragma omp parallel for
    for(auto I = 0; I < tmp_node_pool.size(); ++I) tmp_node_pool.at(I)->resetStatus();
#pragma omp parallel for
    for(auto I = 0; I < tmp_element_pool.size(); ++I) {
        tmp_element_pool.at(I)->resetStatus();
        tmp_element_pool.at(I)->updateStatus();
    }
#else
    for(const auto& I : tmp_node_pool) I->resetStatus();
    for(const auto& I : tmp_element_pool) {
        I->resetStatus();
        I->updateStatus();
    }
#endif
}

bool Domain::insertLoadedDOF(const unsigned& T)
{
    auto R = loaded_dofs.insert(T);
    return R.second;
}

bool Domain::insertRestrainedDOF(const unsigned& T)
{
    auto R = restrained_dofs.insert(T);
    return R.second;
}

bool Domain::insertConstrainedDOF(const unsigned& T)
{
    auto R = constrained_dofs.insert(T);
    return R.second;
}

const unordered_set<unsigned>& Domain::getLoadedDOF() const { return loaded_dofs; }

const unordered_set<unsigned>& Domain::getRestrainedDOF() const
{
    return restrained_dofs;
}

const unordered_set<unsigned>& Domain::getConstrainedDOF() const
{
    return constrained_dofs;
}
