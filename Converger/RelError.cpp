#include "RelError.h"
#include <Domain/Domain.h>
#include <Domain/Workshop.h>

/**
* \brief The default constructor.
* \param T `unique_tag`
* \param D `domain`
* \param E `tolerance`
* \param M `max_iteration`
* \param P `print_flag`
*/
RelError::RelError(const unsigned& T, const shared_ptr<Domain>& D, const double& E, const unsigned& M, const bool& P)
    : Converger(T, CT_RELERROR, D, E, M, P) {}

/**
* \brief No tag version.
* \param D `domain`
* \param E `tolerance`
* \param M `max_iteration`
* \param P `print_flag`
*/
RelError::RelError(const shared_ptr<Domain>& D, const double& E, const unsigned& M, const bool& P)
    : Converger(0, CT_RELERROR, D, E, M, P) {}

/**
* \brief Method to return `conv_flag`.
* \return `conv_flag`
*/
const bool& RelError::if_converged() {
    auto& tmp_workroom = get_domain()->get_workshop();

    set_error(tmp_workroom->get_error() / norm(tmp_workroom->get_trial_displacement()));
    set_conv_flag(get_tolerance() > get_error());

    if(if_print()) suanpan_info("relative error: %.5E.\n", get_error());

    return get_conv_flag();
}
