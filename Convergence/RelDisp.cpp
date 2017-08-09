#include "RelDisp.h"
#include <Domain/Domain.h>
#include <Domain/Workroom.h>

RelDisp::RelDisp(const unsigned& T,
    const shared_ptr<Domain>& D,
    const double& TOL,
    const bool& P)
    : Convergence(T, CT_RELDISP, D, TOL, P)
{
}

RelDisp::RelDisp(const shared_ptr<Domain>& D, const double& TOL, const bool& P)
    : Convergence(0, CT_RELDISP, D, TOL, P)
{
}

const bool& RelDisp::if_converged()
{
    auto& W = getDomain()->getWorkroom();

    setError(norm(W->getIncreDisplacement() / W->getTrialDisplacement()));
    setFlag(getError() > getTolerance() ? false : true);

    if(if_print()) printf("Relative Displacement Error: %.5E.\n", getError());

    return getFlag();
}
