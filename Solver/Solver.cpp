#include "Solver.h"
#include <Convergence/Convergence.h>
#include <Domain/Domain.h>

Solver::Solver(const unsigned& T,
    const unsigned& CT,
    const shared_ptr<Domain>& D,
    const shared_ptr<Convergence>& C)
    : Tag(T, CT)
    , database(D)
    , converger(C)
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
        suanpan_error("initialize() needs a valid Convergence.\n");
        return -1;
    }

    auto code = 0;

    if(!database->is_initialized()) code += database->initialize();

    converger->set_domain(database);

    if(!converger->is_initialized()) code += converger->initialize();

    return code;
}

void Solver::set_domain(const shared_ptr<Domain>& D) { database = D; }

const shared_ptr<Domain>& Solver::get_domain() const { return database; }

void Solver::set_convergence(const shared_ptr<Convergence>& C) { converger = C; }

const shared_ptr<Convergence>& Solver::get_convergence() const { return converger; }
