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

#include "Modulated.h"

Modulated::Modulated(const unsigned T, const double AA, const double WW1, const double WW2, const unsigned ST)
    : Amplitude(T, CT_MODULATED, ST)
    , A(AA)
    , W1(WW1)
    , W2(WW2) {}

double Modulated::get_amplitude(const double& T) {
    const auto step_time = T - start_time;

    if(step_time <= 0.) return 0.;

    return A * sin(W1 * step_time) * sin(W2 * step_time);
}

void Modulated::print() { suanpan_info("Modulated Amplitude.\n"); }
