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
    auto& W = getDomain()->getWorkroom();

    if(W->is_symm() && W->is_band())
        return pb_solve(
            getNinja(W), getStiffness(W), W->getTrialLoad() - W->getTrialResistance());

    if(W->is_symm() && !W->is_band())
        return sy_solve(
            getNinja(W), getStiffness(W), W->getTrialLoad() - W->getTrialResistance());

    if(!W->is_symm() && !W->is_band())
        return ge_solve(
            getNinja(W), getStiffness(W), W->getTrialLoad() - W->getTrialResistance());

    if(!W->is_symm() && W->is_band()) {
        unsigned L, U;
        W->getBandwidth(L, U);
        return gb_solve(getNinja(W), getStiffness(W),
            W->getTrialLoad() - W->getTrialResistance(), L, U);
    }

    return -1;
}

int Newton::analyze(const double& T)
{
    auto& D = getDomain();
    auto& W = D->getWorkroom();
    auto& C = getConvergence();

    // FORM INITIAL TAGENT STIFFNESS AND RESISTANCE FOR ELEMENTS
    D->updateTrialStatus();

    auto time_left = T;
    auto step = ini_step_size;

    unsigned num_increment = 0;

    while(time_left > 0. && num_increment++ < max_increment) {
        unsigned counter = 0;
        W->updateIncreTime(step);
        do {
            // FORM GLOBAL STIFFNESS
            D->updateStiffness();
            // FORM GLOBAL RESISTANCE
            D->updateResistance();
            // PROCESS BC AND LOAD
            D->process();
            // CALL SOLVER
            auto INFO = updateStatus();
            if(INFO != 0) {
                printf("ERROR CODE FROM SOLVER: %u\n", INFO);
                return INFO;
            }
            // UPDATE TRIAL STATUS FOR WORKROOM
            W->updateTrialDisplacement(W->getTrialDisplacement() + W->getNinja());
            // UPDATE FOR ELEMENTS AND CONTINUE THE LOOP IF NOT CONVERGED
            D->updateTrialStatus();
        } while(!C->if_converged() && ++counter < max_iteration);
        if(C->getFlag()) {
            // EXIT LOOP WITH CONVERGED STATUS COMMIT STATUS
            D->commitStatus();
            // MOVE TOWARDS
            time_left -= step;
        } else {
            // FAIL TO CONVERGE FOR CURRENT INCREMENT RESET STATUS
            D->resetStatus();
            // HALVE THE STEP SIZE
            if(step <= min_step_size) {
                printf("Newton::analyze() reaches minimum step size.\n");
                return -1;
            }
            step /= 2.;
        }
    }

    if(num_increment > max_increment)
        printf("Newton::analyze() reaches maximum iteration numbers. The trial status "
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
