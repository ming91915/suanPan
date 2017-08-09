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
    auto& W = getDomain()->getWorkroom();

    setError(norm(W->getNinja()) / norm(W->getTrialDisplacement()));
    setFlag(getTolerance() > getError() ? true : false);

    if(if_print()) printf("Relative Incremental Displacement Error: %.5E.\n", getError());

    return getFlag();
}
