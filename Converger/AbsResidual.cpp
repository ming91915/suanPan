#include "AbsResidual.h"
#include <Domain/DomainBase.h>
#include <Domain/Factory.hpp>

AbsResidual::AbsResidual(const unsigned& T, const shared_ptr<DomainBase>& D, const double& E, const unsigned& M, const bool& P)
    : Converger(T, CT_ABSRESIDUAL, D, E, M, P) {}

AbsResidual::AbsResidual(const shared_ptr<DomainBase>& D, const double& E, const unsigned& M, const bool& P)
    : Converger(0, CT_ABSRESIDUAL, D, E, M, P) {}

AbsResidual::AbsResidual(const double& E, const unsigned& M, const bool& P)
    : Converger(0, CT_ABSRESIDUAL, nullptr, E, M, P) {}

const bool& AbsResidual::if_converged() {
    auto& D = get_domain();
    auto& W = D->get_factory();

    auto t_shinobi = W->get_shinobi();

    // not really interested in reactions
    for(const auto& I : D->get_restrained_dof()) t_shinobi(I) = 0.;

    set_error(norm(t_shinobi));

    set_conv_flag(get_tolerance() > get_error());

    if(if_print()) suanpan_info("absolute residual: %.5E.\n", get_error());

    return get_conv_flag();
}
