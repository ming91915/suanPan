#include "AbsIncreDisp.h"
#include <Domain/Domain.h>
#include <Domain/Workroom.h>

/**
* \brief The default constructor.
* \param T `unique_tag`
* \param D `domain`
* \param E `tolerance`
* \param P `print_flag`
*/
AbsIncreDisp::AbsIncreDisp(const unsigned& T,
    const shared_ptr<Domain>& D,
    const double& E,
    const bool& P)
    : Convergence(T, CT_ABSINCREDISP, D, E, P)
{
}

/**
* \brief No tag version.
* \param D `domain`
* \param E `tolerance`
* \param P `print_flag`
*/
AbsIncreDisp::AbsIncreDisp(const shared_ptr<Domain>& D, const double& E, const bool& P)
    : Convergence(0, CT_ABSINCREDISP, D, E, P)
{
}

/**
* \brief Method to return `conv_flag`.
* \return `conv_flag`
*/
const bool& AbsIncreDisp::if_converged()
{
    setError(norm(getDomain()->getWorkroom()->getNinja()));
    setFlag(getTolerance() > getError() ? true : false);

    if(if_print()) printf("Absolute Incremental Displacement Error: %.5E.\n", getError());

    return getFlag();
}
