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

#include "RelError.h"
#include <Domain/DomainBase.h>
#include <Domain/Factory.hpp>

/**
 * \brief The default constructor.
 * \param T `unique_tag`
 * \param D `DomainBase`
 * \param E `tolerance`
 * \param M `max_iteration`
 * \param P `print_flag`
 */
RelError::RelError(const unsigned& T, const shared_ptr<DomainBase>& D, const double& E, const unsigned& M, const bool& P)
    : Converger(T, CT_RELERROR, D, E, M, P) {}

/**
 * \brief No tag version.
 * \param D `DomainBase`
 * \param E `tolerance`
 * \param M `max_iteration`
 * \param P `print_flag`
 */
RelError::RelError(const shared_ptr<DomainBase>& D, const double& E, const unsigned& M, const bool& P)
    : Converger(0, CT_RELERROR, D, E, M, P) {}

/**
 * \brief Method to return `conv_flag`.
 * \return `conv_flag`
 */
const bool& RelError::if_converged() {
    auto& tmp_workroom = get_domain()->get_factory();

    set_error(tmp_workroom->get_error() / norm(tmp_workroom->get_trial_displacement()));
    set_conv_flag(get_tolerance() > get_error());

    if(if_print()) suanpan_info("relative error: %.5E.\n", get_error());

    return get_conv_flag();
}
