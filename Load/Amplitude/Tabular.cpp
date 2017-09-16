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
#include "Tabular.h"

Tabular::Tabular(const unsigned& T, const unsigned& CT)
    : Amplitude(T, CT) {}

Tabular::Tabular(const unsigned& T, const vec& TI, const vec& M)
    : Amplitude(T, CT_TABULAR)
    , time(TI)
    , magnitude(M) {
    if(time.n_elem != magnitude.n_elem) throw logic_error("Tabular requires two vectors of the same size.\n");
}

Tabular::Tabular(const unsigned& T, const char* P)
    : Amplitude(T, CT_TABULAR) {
    mat ext_data;
    ext_data.load(P);
    if(ext_data.n_cols == 2) {
        time = ext_data.col(0);
        magnitude = ext_data.col(1);
    } else if(ext_data.n_cols > 2)
        suanpan_warning("Tabular() reads more than two columns from the given file, check it.\n");
    else
        throw logic_error("Tabular requires two valid columns.\n");
}

Tabular::~Tabular() {}

double Tabular::get_amplitude(const double& T) {
    uword IDX = 0;
    for(uword i = 0; i < time.n_elem; ++i)
        if(time(i) < T)
            IDX = i;
        else
            break;
    auto A = magnitude(IDX);
    if(IDX == 0)
        A = 0.;
    else if(IDX != time.n_elem - 1)
        A += (T - time(IDX)) * (magnitude(IDX + 1) - magnitude(IDX)) / (time(IDX + 1) - time(IDX));
    return A;
}

void Tabular::print() { suanpan_info("Tabular.\n"); }
