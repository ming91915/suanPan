#include "Static.h"
#include <Solver/Integrator/Integrator.h>
#include <Solver/Solver.h>

Static::Static(const unsigned& T, const double& P)
    : Step(T, CT_STATIC, P)
{
}

int Static::analyze()
{
    auto& S = get_solver();
    auto& G = get_integrator();

    // FORM INITIAL TAGENT STIFFNESS AND RESISTANCE FOR ELEMENTS
    G->update_trial_status();

    auto time_left = get_time_period();
    auto step = ini_step_size;

    unsigned num_increment = 0;

    while(time_left > 0. && ++num_increment <= max_increment) {
        G->update_incre_time(step);
        const auto code = S->analyze(get_tag());
        if(code == 0) { // SUCCEEDED STEP
            G->commit_status();
            G->record();
            time_left -= step;
        } else if(code == -1) { // FAILED UNCONVERGED
            G->reset_status();
            if(step <= min_step_size) {
                suanpan_error(
                    "analyze() reaches minimum step size %.3E.\n", min_step_size);
                return -1;
            }
            if(!fixed_size) step /= 2.;
        } else { // FAILED SOLVER
            suanpan_error(
                "analyze() recieves error code %u from lapack subroutine.\n", code);
            return -1;
        }
    }

    if(num_increment > max_increment) {
        suanpan_warning(
            "analyze() reaches maximum iteration number %u.\n", max_increment);
        return -1;
    }

    return 0;
}
