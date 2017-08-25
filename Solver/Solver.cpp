#include "Solver.h"
#include "Integrator/Integrator.h"
#include <Converger/Converger.h>
#include <Domain/Domain.h>

Solver::Solver(const unsigned& T,
    const unsigned& CT,
    const shared_ptr<Domain>& D,
    const shared_ptr<Converger>& C,
    const shared_ptr<Integrator>& G)
    : Tag(T, CT)
    , database(D)
    , converger(C)
    , modifier(G)
{
    suanpan_debug("Solver %u ctor() called.\n", get_tag());
}

Solver::~Solver() { suanpan_debug("Solver %u dtor() called.\n", get_tag()); }

const bool& Solver::is_initialized() const { return initialized; }

int Solver::initialize()
{
    if(!initialized) initialized = true;

    if(database == nullptr) {
        suanpan_error("initialize() needs a valid Domain.\n");
        return -1;
    }

    if(converger == nullptr) {
        suanpan_error("initialize() needs a valid Converger.\n");
        return -1;
    }

    if(modifier == nullptr) {
        suanpan_error("initialize() needs a valid Integrator.\n");
        return -1;
    }

    auto code = 0;

    if(!database->is_initialized()) code += database->initialize();

    converger->set_domain(database);

    if(!converger->is_initialized()) code += converger->initialize();

    modifier->set_domain(database);

    if(!modifier->is_initialized()) code += modifier->initialize();

    return code;
}

void Solver::set_domain(const shared_ptr<Domain>& D) { database = D; }

const shared_ptr<Domain>& Solver::get_domain() const { return database; }

void Solver::set_converger(const shared_ptr<Converger>& C) { converger = C; }

const shared_ptr<Converger>& Solver::get_converger() const { return converger; }

void Solver::set_integrator(const shared_ptr<Integrator>& G) { modifier = G; }

const shared_ptr<Integrator>& Solver::get_integrator() const { return modifier; }
