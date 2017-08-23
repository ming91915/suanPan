#include "AbsError.h"
#include <Domain/Domain.h>
#include <Domain/Workroom.h>

/**
 * \brief The default constructor.
 * \param T `unique_tag`
 * \param D `domain`
 * \param E `tolerance`
 * \param P `print_flag`
 */
AbsError::AbsError(const unsigned& T,
    const shared_ptr<Domain>& D,
    const double& E,
    const bool& P)
    : Convergence(T, CT_ABSERROR, D, E, P)
{
}

/**
 * \brief No tag version.
 * \param D `domain`
 * \param E `tolerance`
 * \param P `print_flag`
 */
AbsError::AbsError(const shared_ptr<Domain>& D, const double& E, const bool& P)
    : Convergence(0, CT_ABSERROR, D, E, P)
{
}

/**
 * \brief Method to return `conv_flag`.
 * \return `conv_flag`
 */
const bool& AbsError::if_converged()
{
    auto& tmp_domain = get_domain();
    if(tmp_domain == nullptr) {
        suanpan_error("if_converged() needs a valid domain.\n");
        set_conv_flag(false);
    } else {
        set_error(tmp_domain->get_workroom()->get_error());
        set_conv_flag(get_tolerance() > get_error());

        if(if_print()) suanpan_info("Absolute Error: %.5E.\n", get_error());
    }

    return get_conv_flag();
}
