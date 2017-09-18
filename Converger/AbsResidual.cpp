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

#include "AbsResidual.h"
#include <Domain/DomainBase.h>
#include <Domain/Factory.hpp>

AbsResidual::AbsResidual(const unsigned& T, const shared_ptr<DomainBase>& D, const double& E, const unsigned& M, const bool& P)
    : Converger(T, CT_ABSRESIDUAL, D, E, M, P) {}

AbsResidual::AbsResidual(const shared_ptr<DomainBase>& D, const double& E, const unsigned& M, const bool& P)
    : Converger(0, CT_ABSRESIDUAL, D, E, M, P) {}

AbsResidual::AbsResidual(const double& E, const unsigned& M, const bool& P)
    : Converger(0, CT_ABSRESIDUAL, nullptr, E, M, P) {}

const bool& AbsResidual::if_converged() {
    auto& D = get_domain();
    auto& W = D->get_factory();

    set_error(1E6);

    set_conv_flag(get_tolerance() > get_error());

    if(if_print()) suanpan_info("absolute residual: %.5E.\n", get_error());

    return get_conv_flag();
}
