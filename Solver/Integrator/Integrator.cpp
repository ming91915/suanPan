#include "Integrator.h"
#include "Domain/Domain.h"

Integrator::Integrator(const unsigned& T, const unsigned& CT, const shared_ptr<Domain>& D)
    : Tag(T, CT)
    , database(D)
{
    suanpan_debug("Integrator %u ctor() called.\n", T);
}

Integrator::~Integrator() { suanpan_debug("Integrator %u dtor() called.\n"); }

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

void Integrator::set_domain(const shared_ptr<Domain>& D) { database = D; }

const shared_ptr<Domain>& Integrator::get_domain() const { return database; }

int Integrator::update_status()
{
    database->update_stiffness();
    database->update_resistance();

    return 0;
}

void Integrator::commit_status() {}
