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

#include "Arnoldi.h"
#include <Solver/Integrator/Integrator.h>

Arnoldi::Arnoldi(const unsigned& T)
    : Solver(T, CT_ARNOLDI) {}

int Arnoldi::analyze() {
    auto& G = get_integrator();

    // assemble resistance
    G->assemble_resistance();
    // assemble stiffness
    G->assemble_matrix();
    // process loads
    G->process_load();
    // process constraints
    G->process_constraint();

    return 0;
}

void Arnoldi::print() { suanpan_info("A solver using Arnoldi--Raphson iteration method.\n"); }
