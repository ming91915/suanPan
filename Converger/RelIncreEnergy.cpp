////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2017 Theodore Chang
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////
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
