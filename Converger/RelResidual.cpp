#include "RelResidual.h"
#include <Domain/Domain.h>
#include <Domain/Workshop.h>

RelResidual::RelResidual(const unsigned& T, const shared_ptr<Domain>& D, const double& E, const unsigned& M, const bool& P)
    : Converger(T, CT_RELRESIDUAL, D, E, M, P) {}

RelResidual::RelResidual(const shared_ptr<Domain>& D, const double& E, const unsigned& M, const bool& P)
    : Converger(0, CT_RELRESIDUAL, D, E, M, P) {}

RelResidual::RelResidual(const double& E, const unsigned& M, const bool& P)
    : Converger(0, CT_RELRESIDUAL, nullptr, E, M, P) {}

const bool& RelResidual::if_converged() {
    auto& tmp_domain = get_domain();

    auto& tmp_workroom = tmp_domain->get_workshop();

    auto tmp_load = tmp_workroom->get_trial_load();

    vec tmp_residual = tmp_load - tmp_workroom->get_trial_resistance();

    for(const auto& I : tmp_domain->get_restrained_dof()) {
        tmp_residual(I) = 0.;
        tmp_load(I) = 0.;
    }

    set_error(norm(tmp_residual) / norm(tmp_load));

    set_conv_flag(get_tolerance() > get_error());

    if(if_print()) suanpan_info("relative residual: %.5E.\n", get_error());

    return get_conv_flag();
}
