#include "Newton.h"
#include "lapack_wrapper.h"
#include <Converger/Converger.h>
#include <Domain/Domain.h>
#include <Domain/Workshop.h>
#include <Solver/Integrator/Integrator.h>

Newton::Newton(const unsigned& T,
    const shared_ptr<Converger>& C,
    const shared_ptr<Integrator>& G)
    : Solver(T, CT_NEWTON, C, G)
{
}

int Newton::update_status()
{
    auto& W = get_integrator()->get_domain()->get_workshop();

    if(W->is_symm() && W->is_band())
        return pb_solve(get_ninja(W), get_stiffness(W),
            W->get_trial_load() - W->get_trial_resistance());

    if(W->is_symm() && !W->is_band())
        return sy_solve(get_ninja(W), get_stiffness(W),
            W->get_trial_load() - W->get_trial_resistance());

    if(!W->is_symm() && !W->is_band())
        return ge_solve(get_ninja(W), get_stiffness(W),
            W->get_trial_load() - W->get_trial_resistance());

    if(!W->is_symm() && W->is_band()) {
        unsigned L, U;
        W->get_bandwidth(L, U);
        return gb_solve(get_ninja(W), get_stiffness(W),
            W->get_trial_load() - W->get_trial_resistance(), L, U);
    }

    return -1;
}

int Newton::analyze(const unsigned& ST)
{
    auto& C = get_converger();
    auto& G = get_integrator();
    auto& W = G->get_domain()->get_workshop();

    unsigned counter = 0;

    while(true) {
        G->update_resistance();
        G->update_stiffness();
        // PROCESS BC AND LOAD
        G->process(ST);
        // CALL SOLVER
        const auto flag = update_status();
        if(flag != 0) {
            suanpan_error("analyze() recieves error code %u from base driver.\n", flag);
            return flag;
        }
        // UPDATE TRIAL STATUS FOR WORKSHOP
        W->update_trial_displacement(W->get_trial_displacement() + W->get_ninja());
        // UPDATE FOR ELEMENTS AND CONTINUE THE LOOP IF NOT CONVERGED
        G->update_trial_status();

        if(C->if_converged()) return 0;
        if(++counter > C->get_max_iteration()) return -1;
    }
}

void Newton::print()
{
    suanpan_info("A solver using Newton--Raphson iteration method.\n");
}
