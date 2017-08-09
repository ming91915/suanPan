#include "RelError.h"
#include <Domain/Domain.h>
#include <Domain/Workroom.h>

/**
* \brief The default constructor.
* \param T `unique_tag`
* \param D `domain`
* \param E `tolerance`
* \param P `print_flag`
*/
RelError::RelError(const unsigned& T,
    const shared_ptr<Domain>& D,
    const double& E,
    const bool& P)
    : Convergence(T, CT_RELERROR, D, E, P)
{
}

/**
* \brief No tag version.
* \param D `domain`
* \param E `tolerance`
* \param P `print_flag`
*/
RelError::RelError(const shared_ptr<Domain>& D, const double& E, const bool& P)
    : Convergence(0, CT_RELERROR, D, E, P)
{
}

/**
* \brief Method to return `conv_flag`.
* \return `conv_flag`
*/
const bool& RelError::if_converged()
{
    auto& W = getDomain()->getWorkroom();

    setError(W->getError() / norm(W->getTrialDisplacement()));
    setFlag(getTolerance() > getError() ? true : false);

    if(if_print()) printf("Relative Error: %.5E.\n", getError());

    return getFlag();
}
