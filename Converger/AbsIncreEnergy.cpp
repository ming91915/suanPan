#include "AbsIncreEnergy.h"
#include <Domain/Domain.h>
#include <Domain/Factory.hpp>

/**
 * \brief The complete constructor.
 * \param T `unique_tag`
 * \param D `domain`
 * \param E `tolerance`
 * \param M `max_itertation`
 * \param P `print_flag`
 */
AbsIncreEnergy::AbsIncreEnergy(const unsigned& T, const shared_ptr<Domain>& D, const double& E, const unsigned& M, const bool& P)
    : Converger(T, CT_ABSINCREENERGY, D, E, M, P) {}

/**
 * \brief No tag version.
 * \param D `domain`
 * \param E `tolerance`
 * \param M `max_itertation`
 * \param P `print_flag`
 */
AbsIncreEnergy::AbsIncreEnergy(const shared_ptr<Domain>& D, const double& E, const unsigned& M, const bool& P)
    : Converger(0, CT_ABSINCREENERGY, D, E, M, P) {}

/**
 * \brief
 * \return
 */
const bool& AbsIncreEnergy::if_converged() {
    auto& D = get_domain();
    auto& W = D->get_factory();

    auto& t_ninja = W->get_ninja();
    auto& t_resistance = W->get_incre_resistance();
    vec t_residual = W->get_trial_load() - W->get_trial_resistance();

    auto t_error = 0.;
    for(const auto& I : D->get_loaded_dof()) t_error += t_ninja(I) * t_residual(I);
    for(const auto& I : D->get_constrained_dof()) t_error -= t_ninja(I) * t_resistance(I);

    set_error(abs(t_error));
    set_conv_flag(get_tolerance() > get_error());

    if(if_print()) suanpan_info("absolute energy increment error: %.5E.\n", get_error());

    return get_conv_flag();
}
