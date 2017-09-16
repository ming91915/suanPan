#include "RelIncreEnergy.h"
#include <Domain/DomainBase.h>
#include <Domain/Factory.hpp>

/**
 * \brief The complete constructor.
 * \param T `unique_tag`
 * \param D `DomainBase`
 * \param E `tolerance`
 * \param M `max_itertation`
 * \param P `print_flag`
 */
RelIncreEnergy::RelIncreEnergy(const unsigned& T, const shared_ptr<DomainBase>& D, const double& E, const unsigned& M, const bool& P)
    : Converger(T, CT_RELINCREENERGY, D, E, M, P) {}

/**
 * \brief No tag version.
 * \param D `DomainBase`
 * \param E `tolerance`
 * \param M `max_itertation`
 * \param P `print_flag`
 */
RelIncreEnergy::RelIncreEnergy(const shared_ptr<DomainBase>& D, const double& E, const unsigned& M, const bool& P)
    : Converger(0, CT_RELINCREENERGY, D, E, M, P) {}

/**
 * \brief
 * \return
 */
const bool& RelIncreEnergy::if_converged() {
    auto& D = get_domain();
    auto& W = D->get_factory();

    auto& t_disp = W->get_trial_displacement();
    auto& t_ninja = W->get_ninja();
    auto& t_shinobi = W->get_shinobi();

    auto t_error = 0.;
    for(const auto& I : D->get_loaded_dof()) t_error += t_ninja(I) * t_shinobi(I) / t_disp(I);
    for(const auto& I : D->get_constrained_dof()) t_error -= t_ninja(I) * t_shinobi(I) / t_disp(I);

    set_error(abs(t_error));
    set_conv_flag(get_tolerance() > get_error());

    if(if_print()) suanpan_info("relative energy increment error: %.5E.\n", get_error());

    return get_conv_flag();
}
