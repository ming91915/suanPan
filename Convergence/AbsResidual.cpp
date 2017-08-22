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
    auto& D = getDomain();
    auto& W = D->getWorkroom();

    vec tmp_residual = W->getTrialLoad() - W->getTrialResistance();

    for(const auto& I : D->getRestrainedDOF()) tmp_residual(I) = 0.;

    setError(norm(tmp_residual));

    setFlag(getTolerance() > getError() ? true : false);

    if(if_print()) suanpan_info("Absolute Residual: %.5E.\n", getError());

    return getFlag();
}
