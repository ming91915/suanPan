#include "ArcLength.h"
#include "Domain/DomainBase.h"
#include <Domain/Factory.hpp>
#include <Domain/Node.h>
#include <Solver/Integrator/Integrator.h>
#include <Solver/Solver.h>

ArcLength::ArcLength(const unsigned& T, const unsigned& NT, const unsigned& DT, const double& MA)
    : Step(T, CT_ARCLENGTH, 0.)
    , node(NT)
    , dof(DT)
    , maginitude(MA) {}

int ArcLength::analyze() {
    auto& S = get_solver();
    auto& G = get_integrator();
    auto& D = G->get_domain();

    auto& t_dof = D->get_node(node)->get_reordered_dof().at(dof - 1);
    auto& t_disp = D->get_factory().lock()->get_trial_displacement();
    auto& dof_ptr = t_disp(t_dof);

    // make sure the stiffness and resistance are correct
    G->update_trial_status();

    auto time_left = get_time_period();
    auto step = get_ini_step_size();

    unsigned num_increment = 0;

    while(true) {
        // check if the target time point is hit
        if(time_left <= 1E-14) return 0;
        // check if the maximum substep number is hit
        if(++num_increment > get_max_substep()) {
            suanpan_warning("analyze() reaches maximum substep number %u.\n", get_max_substep());
            return -1;
        }
        // update incremental and trial time
        G->update_incre_time(step);
        // call solver
        const auto code = S->analyze();
        if(code == 0) { // success step
                        // commit converged iteration
            G->commit_status();
            // record response
            G->record();
            // eat current increment
            time_left -= step;
            // check if time overflows
            if(step > time_left) step = time_left;
        } else if(code == -1) { // failed step
                                // reset to the start of current substep
            G->reset_status();
            // check if minimum step size is hit
            if(step <= get_min_step_size()) {
                suanpan_error("analyze() reaches minimum step size %.3E.\n", get_min_step_size());
                return -1;
            }
            // check if fixed step size
            if(is_fixed_step_size()) {
                suanpan_error("analyze() does not converge for given fixed step size %.3E.\n", step);
                return -1;
            }
            // step size is allowed to decrease
            step /= 2.;
        } else // positive codes are from lapack subroutines
            return -1;
    }
}
