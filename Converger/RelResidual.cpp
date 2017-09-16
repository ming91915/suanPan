#include "RelResidual.h"
#include <Domain/DomainBase.h>
#include <Domain/Factory.hpp>

RelResidual::RelResidual(const unsigned& T, const shared_ptr<DomainBase>& D, const double& E, const unsigned& M, const bool& P)
    : Converger(T, CT_RELRESIDUAL, D, E, M, P) {}

RelResidual::RelResidual(const shared_ptr<DomainBase>& D, const double& E, const unsigned& M, const bool& P)
    : Converger(0, CT_RELRESIDUAL, D, E, M, P) {}

RelResidual::RelResidual(const double& E, const unsigned& M, const bool& P)
    : Converger(0, CT_RELRESIDUAL, nullptr, E, M, P) {}

const bool& RelResidual::if_converged() {
    auto& D = get_domain();

    auto& W = D->get_factory();

    auto t_load = W->get_trial_load();

    vec t_residual = t_load - W->get_trial_resistance();

    for(const auto& I : D->get_restrained_dof()) {
        t_residual(I) = 0.;
        t_load(I) = 0.;
    }
    for(const auto& I : D->get_constrained_dof()) {
        t_residual(I) = 0.;
        t_load(I) = 0.;
    }

    set_error(norm(t_residual) / norm(t_load));

    set_conv_flag(get_tolerance() > get_error());

    if(if_print()) suanpan_info("relative residual: %.5E.\n", get_error());

    return get_conv_flag();
}
