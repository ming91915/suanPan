#include "RelIncreDisp.h"
#include <Domain/Domain.h>
#include <Domain/Workroom.h>

RelIncreDisp::RelIncreDisp(const unsigned& T,
    const shared_ptr<Domain>& D,
    const double& E,
    const bool& P)
    : Convergence(T, CT_RELINCREDISP, D, E, P)
{
}

RelIncreDisp::RelIncreDisp(const shared_ptr<Domain>& D, const double& E, const bool& P)
    : Convergence(0, CT_RELINCREDISP, D, E, P)
{
}

const bool& RelIncreDisp::if_converged()
{
    auto& tmp_domain = get_domain();
    if(tmp_domain == nullptr) {
        suanpan_error("if_converged() needs a valid domain.\n");
        set_conv_flag(false);
    } else {
        auto& tmp_workroom = tmp_domain->get_workroom();

        set_error(norm(tmp_workroom->get_ninja()) /
            norm(tmp_workroom->get_trial_displacement()));
        set_conv_flag(get_tolerance() > get_error());

        if(if_print())
            suanpan_info("Relative Incremental Displacement Error: %.5E.\n", get_error());
    }

    return get_conv_flag();
}
