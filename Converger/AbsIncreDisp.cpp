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
#include "AbsIncreDisp.h"
#include <Domain/DomainBase.h>
#include <Domain/Factory.hpp>

/**
 * \brief The default constructor.
 * \param T `unique_tag`
 * \param D `DomainBase`
 * \param E `tolerance`
 * \param M `max_itertation`
 * \param P `print_flag`
 */
AbsIncreDisp::AbsIncreDisp(const unsigned& T, const shared_ptr<DomainBase>& D, const double& E, const unsigned& M, const bool& P)
    : Converger(T, CT_ABSINCREDISP, D, E, M, P) {}

/**
 * \brief No tag version.
 * \param D `DomainBase`
 * \param E `tolerance`
 * \param M `max_itertation`
 * \param P `print_flag`
 */
AbsIncreDisp::AbsIncreDisp(const shared_ptr<DomainBase>& D, const double& E, const unsigned& M, const bool& P)
    : Converger(0, CT_ABSINCREDISP, D, E, M, P) {}

/**
 * \brief Method to return `conv_flag`.
 * \return `conv_flag`
 */
const bool& AbsIncreDisp::if_converged() {
    set_error(norm(get_domain()->get_factory()->get_ninja()));
    set_conv_flag(get_tolerance() > get_error());

    if(if_print()) suanpan_info("absolute incremental displacement error: %.5E.\n", get_error());

    return get_conv_flag();
}
