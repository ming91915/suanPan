#include "Solver.h"
#include <Converger/Converger.h>
#include <Solver/Integrator/Integrator.h>

Solver::Solver(const unsigned& T, const unsigned& CT, const shared_ptr<Converger>& C, const shared_ptr<Integrator>& G)
    : Tag(T, CT)
    , converger(C)
    , modifier(G) {
    suanpan_debug("Solver %u ctor() called.\n", get_tag());
}

Solver::~Solver() { suanpan_debug("Solver %u dtor() called.\n", get_tag()); }

const bool& Solver::is_initialized() const { return initialized; }

int Solver::initialize() {
    if(!initialized) initialized = true;

    if(converger == nullptr) {
        suanpan_error("initialize() needs a valid Converger.\n");
        return -1;
    }

    if(modifier == nullptr) {
        suanpan_error("initialize() needs a valid Integrator.\n");
        return -1;
    }

    return 0;
}

void Solver::set_converger(const shared_ptr<Converger>& C) { converger = C; }

const shared_ptr<Converger>& Solver::get_converger() const { return converger; }

void Solver::set_integrator(const shared_ptr<Integrator>& G) { modifier = G; }

const shared_ptr<Integrator>& Solver::get_integrator() const { return modifier; }
