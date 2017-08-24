#include "Static.h"
#include "Convergence/Convergence.h"
#include "Domain/Domain.h"
#include "Domain/Workroom.h"
#include "Solver/Newton.h"

Static::Static(const unsigned& T,
    const shared_ptr<Domain>& D,
    const shared_ptr<Solver>& S,
    const shared_ptr<Convergence>& C,
    const double& P)
    : Step(T, CT_STATIC, D, S, C, P)
{
}

int Static::analyze()
{
    auto& tmp_workroom = get_workroom();
    auto& tmp_domain = get_domain();
    auto& tmp_solver = get_solver();
    auto& tmp_converger = get_convergence();

    // FORM INITIAL TAGENT STIFFNESS AND RESISTANCE FOR ELEMENTS
    tmp_domain->update_trial_status();

    auto time_left = get_time_period();
    auto step = ini_step_size;

    unsigned num_increment = 0;

    while(time_left > 0. && num_increment++ < max_increment) {
        unsigned counter = 0;
        tmp_workroom->update_incre_time(step);
        do
            tmp_solver->analyze(get_tag());
        while(!tmp_converger->if_converged() && ++counter < max_iteration);
        if(tmp_converger->get_conv_flag()) {
            // EXIT LOOP WITH CONVERGED STATUS COMMIT STATUS
            tmp_domain->commit_status();
            // RECORD STATUS
            tmp_domain->record();
            // MOVE TOWARDS
            time_left -= step;
        } else {
            // FAIL TO CONVERGE FOR CURRENT INCREMENT RESET STATUS
            tmp_domain->reset_status();
            // HALVE THE STEP SIZE
            if(step <= min_step_size) {
                suanpan_error("analyze() reaches minimum step size.\n");
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
