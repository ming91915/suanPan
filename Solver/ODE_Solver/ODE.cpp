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
#include "ODE.h"

ODE::ODE(const unsigned& T, const unsigned& CT, const unsigned& D)
    : Tag(T, CT)
    , n_size(D) {}

void ODE::set_dimension(const unsigned& D) { n_size = D; }

const unsigned& ODE::get_dimension() const { return n_size; }

void ODE::print() { suanpan_info("ODE::print() is unimplemented.\n"); }

void ODE::operator()(const unsigned& D) { n_size = D; }

unsigned ODE::operator()() { return n_size; }
