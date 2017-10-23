/*******************************************************************************
 * Copyright (C) 2017 Theodore Chang
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

#ifndef ODE_INSTANCE_H
#define ODE_INSTANCE_H

#include "ODE.h"

class ODE_INSTANCE : public ODE {
public:
    ODE_INSTANCE()
        : ODE(0, 1) {}

    unique_ptr<ODE> get_copy() override { return make_unique<ODE_INSTANCE>(*this); }

    //! Analytical solution:
    //! y=@(x)(-exp(-x*x/2)*x*x-2*exp(-x*x/2)+3)/(exp(-x*x/2));
    vec eval(const double T, const vec& Y) final { return T * Y + T * T * T; }
};

#endif
