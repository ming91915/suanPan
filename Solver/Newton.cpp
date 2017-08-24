#include "Newton.h"
#include <Domain/Domain.h>
#include <Domain/Workroom.h>

Newton::Newton(const unsigned& T, const shared_ptr<Domain>& D)
    : Solver(T, CT_NEWTON, D)
{
}

Newton::Newton(const shared_ptr<Domain>& D)
    : Solver(0, CT_NEWTON, D)
{
}

int Newton::update_status()
{
    auto& tmp_workroom = get_domain()->get_workroom();

    if(tmp_workroom->is_symm() && tmp_workroom->is_band())
        return pb_solve(get_ninja(tmp_workroom), get_stiffness(tmp_workroom),
            tmp_workroom->get_trial_load() - tmp_workroom->get_trial_resistance());

    if(tmp_workroom->is_symm() && !tmp_workroom->is_band())
        return sy_solve(get_ninja(tmp_workroom), get_stiffness(tmp_workroom),
            tmp_workroom->get_trial_load() - tmp_workroom->get_trial_resistance());

    if(!tmp_workroom->is_symm() && !tmp_workroom->is_band())
        return ge_solve(get_ninja(tmp_workroom), get_stiffness(tmp_workroom),
            tmp_workroom->get_trial_load() - tmp_workroom->get_trial_resistance());

    if(!tmp_workroom->is_symm() && tmp_workroom->is_band()) {
        unsigned L, U;
        tmp_workroom->get_bandwidth(L, U);
        return gb_solve(get_ninja(tmp_workroom), get_stiffness(tmp_workroom),
            tmp_workroom->get_trial_load() - tmp_workroom->get_trial_resistance(), L, U);
    }

    return -1;
}

int Newton::analyze(const unsigned& ST)
{
    auto& tmp_domain = get_domain();
    auto& tmp_workroom = tmp_domain->get_workroom();

    // FORM GLOBAL STIFFNESS
    tmp_domain->update_stiffness();
    // FORM GLOBAL RESISTANCE
    tmp_domain->update_resistance();
    // PROCESS BC AND LOAD
    tmp_domain->process(ST);
    // CALL SOLVER
    auto flag = update_status();
    if(flag != 0) {
        suanpan_error("analyze() recieves error code %u from base driver.\n", flag);
        return flag;
    }
    // UPDATE TRIAL STATUS FOR WORKROOM
    tmp_workroom->update_trial_displacement(
        tmp_workroom->get_trial_displacement() + tmp_workroom->get_ninja());
    // UPDATE FOR ELEMENTS AND CONTINUE THE LOOP IF NOT CONVERGED
    tmp_domain->update_trial_status();

    return 0;
}

void Newton::print() { printf("A solver using Newton--Raphson iteration method.\n"); }
