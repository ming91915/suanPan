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

#include "Decay.h"

Decay::Decay(const unsigned T, const double AA, const double TTD, const unsigned ST)
    : Amplitude(T, CT_COSINE, ST)
    , A(AA)
    , TD(TTD) {}

double Decay::get_amplitude(const double& T) {
    const auto step_time = T - start_time;

    if(step_time <= 0.) return 0.;

    return A * exp(-start_time / TD);
}

void Decay::print() { suanpan_info("Decay Amplitude.\n"); }
