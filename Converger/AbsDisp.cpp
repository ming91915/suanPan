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

#include "AbsDisp.h"
#include <Domain/DomainBase.h>
#include <Domain/Factory.hpp>

/**
 * \brief The complete constructor.
 * \param T `unique_tag`
 * \param E `tolerance`
 * \param M `max_itertation`
 * \param P `print_flag`
 */
AbsDisp::AbsDisp(const unsigned& T, const double& E, const unsigned& M, const bool& P)
    : Converger(T, CT_ABSDISP, E, M, P) {}

/**
 * \brief
 * \return
 */
const bool& AbsDisp::if_converged() {
    set_error(norm(get_domain()->get_factory().lock()->get_incre_displacement()));
    set_conv_flag(get_tolerance() > get_error());

    if(if_print()) suanpan_info("absolute displacement error: %.5E.\n", get_error());

    return get_conv_flag();
}
