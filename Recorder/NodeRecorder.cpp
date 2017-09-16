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

NodeRecorder::NodeRecorder(const unsigned& T, const unsigned& B, const OutputList& L, const bool& R)
    : Recorder(T, CT_NODERECORDER, B, L, R) {
    suanpan_info("NodeRecorder %u ctor() called.\n", T);
}

void NodeRecorder::record(const shared_ptr<Domain>& D) {
    auto& tmp_obj = D->get_node(get_object_tag());

    insert(tmp_obj->record(get_variable_type()));

    if(if_record_time()) insert(D->get_factory()->get_current_time());
}

void NodeRecorder::print() {
    for(const auto& I : get_data_pool()) {
        for(const auto& J : I) J.print();
        suanpan_info("\n");
    }
}
