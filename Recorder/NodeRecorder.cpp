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

#include "NodeRecorder.h"
#include <Domain/Domain.h>
#include <Domain/Factory.hpp>
#include <Domain/Node.h>
#include <hdf5.h>
#include <hdf5_hl.h>

NodeRecorder::NodeRecorder(const unsigned& T, const unsigned& B, const OutputList& L, const bool& R)
    : Recorder(T, CT_NODERECORDER, B, L, R) {}

void NodeRecorder::record(const shared_ptr<Domain>& D) {
    auto& t_obj = D->get_node(get_object_tag());

    insert(t_obj->record(get_variable_type()));

    if(if_record_time()) insert(D->get_factory().lock()->get_current_time());
}

void NodeRecorder::print() {
    auto& data = get_data_pool();
    auto& time = get_time_pool();

    mat data_to_write(data.cbegin()->cbegin()->n_elem + 1, time.size());

    for(size_t I = 0; I < time.size(); ++I) {
        data_to_write(0, I) = time[I];
        for(const auto& J : data[I])
            for(unsigned K = 0; K < J.n_elem; ++K) data_to_write(K + 1, I) = J[K];
    }

    hsize_t dimention[2] = { data_to_write.n_cols, data_to_write.n_rows };

    std::ostringstream file_name;

    file_name << to_char(get_variable_type()) << get_object_tag();

    const auto file_id = H5Fcreate(file_name.str().c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

    const auto group_d = H5Gcreate2(file_id, "/DISPLACEMENT", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    H5LTmake_dataset(group_d, file_name.str().c_str(), 2, dimention, H5T_NATIVE_DOUBLE, data_to_write.mem);

    H5Fclose(file_id);
}
