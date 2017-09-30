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

#include "Recorder.h"
#ifndef SUANPAN_NO_HDF5
#include <hdf5.h>
#include <hdf5_hl.h>
#endif

Recorder::Recorder(const unsigned& T, const unsigned& CT, const unsigned& B, const OutputList& L, const bool& R)
    : Tag(T, CT)
    , object_tag(B)
    , variable_type(L)
    , record_time(R) {
    suanpan_debug("NodeRecorder %u ctor() called.\n", T);
}

Recorder::~Recorder() { suanpan_debug("NodeRecorder %u dtor() called.\n", get_tag()); }

void Recorder::set_object_tag(const unsigned& T) { object_tag = T; }

const unsigned& Recorder::get_object_tag() const { return object_tag; }

void Recorder::set_variable_type(const OutputList& T) { variable_type = T; }

const OutputList& Recorder::get_variable_type() const { return variable_type; }

const bool& Recorder::if_record_time() const { return record_time; }

void Recorder::insert(const double& T) { time_pool.push_back(T); }

void Recorder::insert(const vector<vec>& D) { data_pool.push_back(D); }

const vector<vector<vec>>& Recorder::get_data_pool() const { return data_pool; }

const vector<double>& Recorder::get_time_pool() const { return time_pool; }

void Recorder::save() {
#ifndef SUANPAN_NO_HDF5
    ostringstream file_name;

    file_name << to_char(variable_type) << object_tag;

    mat data_to_write(data_pool.cbegin()->cbegin()->n_elem + 1, time_pool.size());

    for(size_t I = 0; I < time_pool.size(); ++I) {
        data_to_write(0, I) = time_pool[I];
        for(const auto& J : data_pool[I])
            for(unsigned K = 0; K < J.n_elem; ++K) data_to_write(K + 1, I) = J[K];
    }

    hsize_t dimention[2] = { data_to_write.n_cols, data_to_write.n_rows };

    const auto file_id = H5Fcreate(file_name.str().c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    const auto group_id = H5Gcreate2(file_id, "/DISPLACEMENT", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    H5LTmake_dataset(group_id, file_name.str().c_str(), 2, dimention, H5T_NATIVE_DOUBLE, data_to_write.mem);

    H5Gclose(group_id);
    H5Fclose(file_id);
#endif
}

void Recorder::print() { suanpan_info("print() needs to be overwritten.\n"); }
