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

#include "RelResidual.h"
#include <Domain/DomainBase.h>
#include <Domain/Factory.hpp>

RelResidual::RelResidual(const unsigned& T, const shared_ptr<DomainBase>& D, const double& E, const unsigned& M, const bool& P)
    : Converger(T, CT_RELRESIDUAL, D, E, M, P) {}

RelResidual::RelResidual(const shared_ptr<DomainBase>& D, const double& E, const unsigned& M, const bool& P)
    : Converger(0, CT_RELRESIDUAL, D, E, M, P) {}

RelResidual::RelResidual(const double& E, const unsigned& M, const bool& P)
    : Converger(0, CT_RELRESIDUAL, nullptr, E, M, P) {}

const bool& RelResidual::if_converged() {
    auto& D = get_domain();
    auto& W = D->get_factory();

    const auto t_shinobi = norm(W->get_shinobi());
    const auto t_resistance = norm(W->get_trial_resistance());

    set_error(t_resistance == 0. ? 1E6 : t_shinobi / t_resistance);

    set_conv_flag(get_tolerance() > get_error());

    if(if_print()) suanpan_info("relative residual: %.5E.\n", get_error());

    return get_conv_flag();
}
