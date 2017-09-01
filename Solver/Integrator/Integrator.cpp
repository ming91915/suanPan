#include "Integrator.h"
#include "Domain/Domain.h"

Integrator::Integrator(const unsigned& T, const unsigned& CT, const shared_ptr<Domain>& D)
    : Tag(T, CT)
    , database(D)
{
    suanpan_debug("Integrator %u ctor() called.\n", T);
}

Integrator::~Integrator() { suanpan_debug("Integrator %u dtor() called.\n", get_tag()); }

const bool& Integrator::is_initialized() const { return initialized; }

int Integrator::initialize()
{
    if(!initialized) initialized = true;

    if(database == nullptr) {
        suanpan_error("initialize() needs a valid Domain.\n");
        return -1;
    }

    return 0;
}

void Integrator::process(const unsigned& ST) const { database->process(ST); }

void Integrator::record() const { database->record(); }

void Integrator::update_resistance() { database->update_resistance(); }

void Integrator::update_stiffness() { database->update_stiffness(); }

void Integrator::update_trial_time(const double& T) const
{
    database->update_trial_time(T);
}

void Integrator::update_incre_time(const double& T) const
{
    database->update_incre_time(T);
}

void Integrator::update_trial_status() { database->update_trial_status(); }

void Integrator::update_incre_status() { database->update_incre_status(); }

void Integrator::commit_status() const { database->commit_status(); }

void Integrator::clear_status() const { database->clear_status(); }

void Integrator::reset_status() const { database->reset_status(); }

void Integrator::set_domain(const shared_ptr<Domain>& D) { database = D; }

const shared_ptr<Domain>& Integrator::get_domain() const { return database; }
