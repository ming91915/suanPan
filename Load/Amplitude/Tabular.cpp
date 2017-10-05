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

Tabular::Tabular(const unsigned& T, const vec& TI, const vec& M, const unsigned& ST)
    : Amplitude(T, CT_TABULAR, ST)
    , time(TI)
    , magnitude(M) {
    if(time.n_elem != magnitude.n_elem) throw logic_error("Tabular requires two vectors of the same size.\n");
}

Tabular::Tabular(const unsigned& T, const char* P, const unsigned& ST)
    : Amplitude(T, CT_TABULAR, ST) {
    mat ext_data;
    if(!ext_data.load(P, auto_detect)) {
        suanpan_error("cannot load file.\n");
        throw;
    }
    if(ext_data.n_cols == 2) {
        time = ext_data.col(0);
        magnitude = ext_data.col(1);
    } else if(ext_data.n_cols > 2)
        suanpan_warning("Tabular() reads more than two columns from the given file, check it.\n");
    else
        throw logic_error("Tabular requires two valid columns.\n");
}

double Tabular::get_amplitude(const double& T) {
    const auto step_time = T - start_time;

    uword IDX = 0;
    while(IDX < time.n_elem && time(IDX) < step_time) ++IDX;

    return IDX == 0 ? 0. : IDX == time.n_elem ? magnitude(magnitude.n_elem - 1) : magnitude(IDX - 1) + (step_time - time(IDX - 1)) * (magnitude(IDX) - magnitude(IDX - 1)) / (time(IDX) - time(IDX - 1));
}

void Tabular::print() { suanpan_info("Tabular.\n"); }
