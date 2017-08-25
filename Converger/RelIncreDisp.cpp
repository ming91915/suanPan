#include "RelIncreDisp.h"
#include <Domain/Domain.h>
#include <Domain/Workroom.h>

RelIncreDisp::RelIncreDisp(const unsigned& T,
    const shared_ptr<Domain>& D,
    const double& E,
    const unsigned& M,
    const bool& P)
    : Converger(T, CT_RELINCREDISP, D, E, M, P)
{
}

RelIncreDisp::RelIncreDisp(const shared_ptr<Domain>& D,
    const double& E,
    const unsigned& M,
    const bool& P)
    : Converger(0, CT_RELINCREDISP, D, E, M, P)
{
}

const bool& RelIncreDisp::if_converged()
{
    auto& tmp_workroom = get_domain()->get_workroom();

    set_error(
        norm(tmp_workroom->get_ninja()) / norm(tmp_workroom->get_trial_displacement()));
    set_conv_flag(get_tolerance() > get_error());

    if(if_print())
        suanpan_info("Relative Incremental Displacement Error: %.5E.\n", get_error());

    return get_conv_flag();
}
