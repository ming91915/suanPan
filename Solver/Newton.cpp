#include "Newton.h"
#include <Convergence/Convergence.h>
#include <Domain/Domain.h>
#include <Domain/Workroom.h>

Newton::Newton(const unsigned& T,
    const shared_ptr<Domain>& D,
    const shared_ptr<Convergence>& C)
    : Solver(T, CT_NEWTON, D, C)
{
}

Newton::Newton(const shared_ptr<Domain>& D, const shared_ptr<Convergence>& C)
    : Solver(0, CT_NEWTON, D, C)
{
}

int Newton::updateStatus()
{
    auto& W = getDomain()->get_workroom();

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

int Newton::analyze(const double& T)
{
    auto& D = getDomain();
    auto& W = D->get_workroom();
    auto& C = getConvergence();

    // FORM INITIAL TAGENT STIFFNESS AND RESISTANCE FOR ELEMENTS
    D->update_trial_status();

    auto time_left = T;
    auto step = ini_step_size;

    unsigned num_increment = 0;

    while(time_left > 0. && num_increment++ < max_increment) {
        unsigned counter = 0;
        W->update_incre_time(step);
        do {
            // FORM GLOBAL STIFFNESS
            D->update_stiffness();
            // FORM GLOBAL RESISTANCE
            D->update_resistance();
            // PROCESS BC AND LOAD
            D->process();
            // CALL SOLVER
            auto INFO = updateStatus();
            if(INFO != 0) {
                suanpan_error("analyze() recieves error code %u from solver.\n", INFO);
                return INFO;
            }
            // UPDATE TRIAL STATUS FOR WORKROOM
            W->update_trial_displacement(W->get_trial_displacement() + W->get_ninja());
            // UPDATE FOR ELEMENTS AND CONTINUE THE LOOP IF NOT CONVERGED
            D->update_trial_status();
        } while(!C->if_converged() && ++counter < max_iteration);
        if(C->get_conv_flag()) {
            // EXIT LOOP WITH CONVERGED STATUS COMMIT STATUS
            D->commit_status();
            // RECORD STATUS
            D->record();
            // MOVE TOWARDS
            time_left -= step;
        } else {
            // FAIL TO CONVERGE FOR CURRENT INCREMENT RESET STATUS
            D->reset_status();
            // HALVE THE STEP SIZE
            if(step <= min_step_size) {
                suanpan_warning("analyze() reaches minimum step size.\n");
                return -1;
            }
            step /= 2.;
        }
    }

    if(num_increment > max_increment)
        suanpan_warning("analyze() reaches maximum iteration numbers. The trial status "
                        "may be unconverged.\n");

    return 0;
}

void Newton::setStepSize(const double& INI, const double& MIN, const double& MAX)
{
    if(MIN > MAX) return;

    if(INI != 0) ini_step_size = INI;
    if(MIN != 0) min_step_size = MIN;
    if(MAX != 0) max_step_size = MAX;
}

void Newton::print() { printf("A solver using Newton--Raphson iteration method.\n"); }
