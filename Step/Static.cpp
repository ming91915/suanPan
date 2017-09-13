#include "Static.h"
#include <Solver/Integrator/Integrator.h>
#include <Solver/Solver.h>

Static::Static(const unsigned& T, const double& P)
    : Step(T, CT_STATIC, P) {}

int Static::analyze() {
    auto& S = get_solver();
    auto& G = get_integrator();

    auto time_left = get_time_period();
    auto step = get_ini_step_size();

    unsigned num_increment = 0;

    while(time_left > 0. && ++num_increment <= get_max_iteration()) {
        G->update_incre_time(step);
        const auto code = S->analyze(get_tag());
        if(code == 0) { // SUCCEEDED STEP
            G->commit_status();
            G->record();
            time_left -= step;
        } else if(code == -1) { // FAILED UNCONVERGED
            G->reset_status();
            if(step <= get_min_step_size()) {
                suanpan_error("analyze() reaches minimum step size %.3E.\n", get_min_step_size());
                return -1;
            }
            if(is_fixed_step_size()) {
                suanpan_error("analyze() does not converge for given fixed step size %.3E.\n", step);
                return -1;
            }
            step /= 2.;
        } else
            return -1;
    }

    if(num_increment > get_max_iteration()) {
        suanpan_warning("analyze() reaches maximum iteration number %u.\n", get_max_iteration());
        return -1;
    }

    return 0;
}
