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
#include "Solver.h"
#include <Converger/Converger.h>
#include <Solver/Integrator/Integrator.h>

Solver::Solver(const unsigned& T, const unsigned& CT, const shared_ptr<Converger>& C, const shared_ptr<Integrator>& G)
    : Tag(T, CT)
    , converger(C)
    , modifier(G) {
    suanpan_debug("Solver %u ctor() called.\n", get_tag());
}

Solver::~Solver() { suanpan_debug("Solver %u dtor() called.\n", get_tag()); }

int Solver::initialize() {
    if(converger == nullptr) {
        suanpan_error("initialize() needs a valid converger.\n");
        return -1;
    }

    if(modifier == nullptr) {
        suanpan_error("initialize() needs a valid integrator.\n");
        return -1;
    }

    return 0;
}

void Solver::set_converger(const shared_ptr<Converger>& C) {
    if(converger != C) converger = C;
}

const shared_ptr<Converger>& Solver::get_converger() const { return converger; }

void Solver::set_integrator(const shared_ptr<Integrator>& G) {
    if(modifier != G) modifier = G;
}

const shared_ptr<Integrator>& Solver::get_integrator() const { return modifier; }
