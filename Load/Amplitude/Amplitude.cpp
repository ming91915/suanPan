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

#include "Amplitude.h"

Amplitude::Amplitude(const unsigned& T, const unsigned& CT)
    : Tag(T, CT) {}

Amplitude::~Amplitude() {}

double Amplitude::get_amplitude(const double& T) {
    if(T < 0.) return 0.;
    if(T > 1.) return 1.;
    return T;
}

void Amplitude::print() { suanpan_debug("This is an Amplitude object.\n"); }
