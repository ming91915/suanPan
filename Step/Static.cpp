#include "Static.h"
#include <Domain/Domain.h>
#include <Domain/Workroom.h>
#include <Solver/Integrator/Integrator.h>
#include <Solver/Solver.h>

Static::Static(const unsigned& T, const double& P)
    : Step(T, CT_STATIC, P)
{
}

int Static::analyze()
{
    auto& tmp_workroom = get_workroom();
    auto& tmp_domain = get_domain();
    auto& tmp_solver = get_solver();
    auto& tmp_integrator = get_integrator();

    // FORM INITIAL TAGENT STIFFNESS AND RESISTANCE FOR ELEMENTS
    tmp_domain->update_trial_status();

    auto time_left = get_time_period();
    auto step = ini_step_size;

    unsigned num_increment = 0;

    while(time_left > 0. && num_increment++ < max_increment) {
        tmp_workroom->update_incre_time(step);
        auto code = tmp_solver->analyze(get_tag());
        if(code == 0) {
            tmp_integrator->commit_status();
            tmp_domain->commit_status();
            tmp_domain->record();
            time_left -= step;
        } else if(code == -1) {
            tmp_domain->reset_status();
            if(step <= min_step_size) {
                suanpan_error("analyze() reaches minimum step size.\n");
                return -1;
            }
            step /= 2.;
        } else {
            suanpan_error(
                "analyze() recieves error code %u from lapack subroutine.\n", code);
            return -1;
        }
    }

    if(num_increment > max_increment)
        suanpan_warning("analyze() reaches maximum iteration number.\n");

    return 0;
}
