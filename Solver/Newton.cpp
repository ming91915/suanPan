#include "Newton.h"
#include <Converger/Converger.h>
#include <Domain/DomainBase.h>
#include <Domain/Factory.hpp>
#include <Solver/Integrator/Integrator.h>

Newton::Newton(const unsigned& T, const shared_ptr<Converger>& C, const shared_ptr<Integrator>& G)
    : Solver(T, CT_NEWTON, C, G) {}

int Newton::update_status() {
    auto& W = get_integrator()->get_domain()->get_factory();
    return W->get_stiffness()->solve(get_ninja(W), W->get_trial_load() - W->get_trial_resistance());
}

int Newton::analyze(const unsigned& ST) {
    auto& C = get_converger();
    auto& G = get_integrator();
    auto& W = G->get_domain()->get_factory();

    unsigned counter = 0;

    while(true) {
        // update for nodes and elements
        G->update_trial_status();
        // assemble resistance and stiffness
        G->update_resistance();
        G->update_stiffness();
        // process constraints and loads
        G->process(ST);

        // call solver
        const auto flag = update_status();
        if(flag != 0) return flag;
        // update trial status for factory
        W->update_trial_displacement(W->get_trial_displacement() + W->get_ninja());

        // test convergence
        if(C->if_converged()) return 0;
        if(++counter > C->get_max_iteration()) return -1;
    }
}

void Newton::print() { suanpan_info("A solver using Newton--Raphson iteration method.\n"); }
