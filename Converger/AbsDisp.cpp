#include "AbsDisp.h"
#include <Domain/Domain.h>
#include <Domain/Workroom.h>

/**
* \brief The complete constructor.
* \param T `unique_tag`
* \param D `domain`
* \param E `tolerance`
* \param M `max_itertation`
* \param P `print_flag`
*/
AbsDisp::AbsDisp(const unsigned& T,
    const shared_ptr<Domain>& D,
    const double& E,
    const unsigned& M,
    const bool& P)
    : Converger(T, CT_ABSDISP, D, E, M, P)
{
}

/**
* \brief No tag version.
* \param D `domain`
* \param E `tolerance`
* \param M `max_itertation`
* \param P `print_flag`
*/
AbsDisp::AbsDisp(const shared_ptr<Domain>& D,
    const double& E,
    const unsigned& M,
    const bool& P)
    : Converger(0, CT_ABSDISP, D, E, M, P)
{
}

/**
 * \brief
 * \return
 */
const bool& AbsDisp::if_converged()
{
    set_error(norm(get_domain()->get_workroom()->get_incre_displacement()));
    set_conv_flag(get_tolerance() > get_error());

    if(if_print()) suanpan_info("Absolute Displacement Error: %.5E.\n", get_error());

    return get_conv_flag();
}
