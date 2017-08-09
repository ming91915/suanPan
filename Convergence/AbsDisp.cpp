#include "AbsDisp.h"
#include <Domain/Domain.h>
#include <Domain/Workroom.h>

/**
 * \brief The complete constructor.
 * \param T `unique_tag`
 * \param D `domain`
 * \param E `tolerance`
 * \param P `print_flag`
 */
AbsDisp::AbsDisp(const unsigned& T,
    const shared_ptr<Domain>& D,
    const double& E,
    const bool& P)
    : Convergence(T, CT_ABSDISP, D, E, P)
{
}

/**
 * \brief No tag version.
 * \param D `domain`
 * \param E `tolerance`
 * \param P `print_flag`
 */
AbsDisp::AbsDisp(const shared_ptr<Domain>& D, const double& E, const bool& P)
    : Convergence(0, CT_ABSDISP, D, E, P)
{
}

/**
 * \brief
 * \return
 */
const bool& AbsDisp::if_converged()
{
    setError(norm(getDomain()->getWorkroom()->getIncreDisplacement()));
    setFlag(getTolerance() > getError() ? true : false);

    if(if_print()) printf("Absolute Displacement Error: %.5E.\n", getError());

    return getFlag();
}
