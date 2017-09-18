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

#include "RelIncreDisp.h"
#include <Domain/DomainBase.h>
#include <Domain/Factory.hpp>

RelIncreDisp::RelIncreDisp(const unsigned& T, const shared_ptr<DomainBase>& D, const double& E, const unsigned& M, const bool& P)
    : Converger(T, CT_RELINCREDISP, D, E, M, P) {}

RelIncreDisp::RelIncreDisp(const shared_ptr<DomainBase>& D, const double& E, const unsigned& M, const bool& P)
    : Converger(0, CT_RELINCREDISP, D, E, M, P) {}

RelIncreDisp::RelIncreDisp(const double& E, const unsigned& M, const bool& P)
    : Converger(0, CT_RELINCREDISP, nullptr, E, M, P) {}

const bool& RelIncreDisp::if_converged() {
    const auto& t_factory = get_domain()->get_factory().lock();

    set_error(norm(t_factory->get_ninja()) / norm(t_factory->get_trial_displacement()));
    set_conv_flag(get_tolerance() > get_error());

    if(if_print()) suanpan_info("relative incremental displacement error: %.5E.\n", get_error());

    return get_conv_flag();
}
