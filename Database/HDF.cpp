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
#include "HDF.h"
#include <hdf5.h>

HDF::HDF() {}

int HDF::save(const char* N) {
    auto& D = getDomain();

    const auto file_id = H5Fcreate(N, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

    return H5Fclose(file_id);
}
