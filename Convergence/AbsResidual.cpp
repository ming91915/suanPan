#include "AbsResidual.h"
#include <Domain/Domain.h>
#include <Domain/Workroom.h>

AbsResidual::AbsResidual(const unsigned& T,
    const shared_ptr<Domain>& D,
    const double& E,
    const bool& P)
    : Convergence(T, CT_ABSRESIDUAL, D, E, P)
{
}

AbsResidual::AbsResidual(const shared_ptr<Domain>& D, const double& E, const bool& P)
    : Convergence(0, CT_ABSRESIDUAL, D, E, P)
{
}

AbsResidual::AbsResidual(const double& E, const bool& P)
    : Convergence(0, CT_ABSRESIDUAL, nullptr, E, P)
{
}

const bool& AbsResidual::if_converged()
{
    auto& tmp_domain = get_domain();
    if(tmp_domain == nullptr) {
        suanpan_error("if_converged() needs a valid domain.\n");
        set_conv_flag(false);
    } else {
        auto& tmp_workroom = tmp_domain->get_workroom();

        vec tmp_residual =
            tmp_workroom->get_trial_load() - tmp_workroom->get_trial_resistance();

        for(const auto& I : tmp_domain->get_restrained_dof()) tmp_residual(I) = 0.;

        set_error(norm(tmp_residual));

        set_conv_flag(get_tolerance() > get_error());

        if(if_print()) suanpan_info("Absolute Residual: %.5E.\n", get_error());
    }

    return get_conv_flag();
}
