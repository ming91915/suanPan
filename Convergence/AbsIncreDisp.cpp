#include "AbsIncreDisp.h"
#include <Domain/Domain.h>
#include <Domain/Workroom.h>

/**
* \brief The default constructor.
* \param T `unique_tag`
* \param D `domain`
* \param E `tolerance`
* \param M `max_itertation`
* \param P `print_flag`
*/
AbsIncreDisp::AbsIncreDisp(const unsigned& T,
    const shared_ptr<Domain>& D,
    const double& E,
    const unsigned& M,
    const bool& P)
    : Convergence(T, CT_ABSINCREDISP, D, E, M, P)
{
}

/**
* \brief No tag version.
* \param D `domain`
* \param E `tolerance`
* \param M `max_itertation`
* \param P `print_flag`
*/
AbsIncreDisp::AbsIncreDisp(const shared_ptr<Domain>& D,
    const double& E,
    const unsigned& M,
    const bool& P)
    : Convergence(0, CT_ABSINCREDISP, D, E, M, P)
{
}

/**
* \brief Method to return `conv_flag`.
* \return `conv_flag`
*/
const bool& AbsIncreDisp::if_converged()
{
    set_error(norm(get_domain()->get_workroom()->get_ninja()));
    set_conv_flag(get_tolerance() > get_error());

    if(if_print())
        suanpan_info("Absolute Incremental Displacement Error: %.5E.\n", get_error());

    return get_conv_flag();
}
