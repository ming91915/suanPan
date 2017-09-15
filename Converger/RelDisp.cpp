#include "RelDisp.h"
#include <Domain/DomainBase.h>
#include <Domain/Factory.hpp>

RelDisp::RelDisp(const unsigned& T, const shared_ptr<DomainBase>& D, const double& E, const unsigned& M, const bool& P)
    : Converger(T, CT_RELDISP, D, E, M, P) {}

RelDisp::RelDisp(const shared_ptr<DomainBase>& D, const double& E, const unsigned& M, const bool& P)
    : Converger(0, CT_RELDISP, D, E, M, P) {}

const bool& RelDisp::if_converged() {
    auto& tmp_workroom = get_domain()->get_factory();

    set_error(norm(tmp_workroom->get_incre_displacement() / tmp_workroom->get_trial_displacement()));
    set_conv_flag(get_tolerance() > get_error());

    if(if_print()) suanpan_info("relative displacement error: %.5E.\n", get_error());

    return get_conv_flag();
}
