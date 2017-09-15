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
    auto& tmp_DomainBase = get_domain();

    auto& tmp_workroom = tmp_DomainBase->get_factory();

    vec tmp_residual = tmp_workroom->get_trial_load() - tmp_workroom->get_trial_resistance();

    for(const auto& I : tmp_DomainBase->get_restrained_dof()) tmp_residual(I) = 0.;

    set_error(norm(tmp_residual));

    set_conv_flag(get_tolerance() > get_error());

    if(if_print()) suanpan_info("absolute residual: %.5E.\n", get_error());

    return get_conv_flag();
}
