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

#include "ElementRecorder.h"
#include <Domain/DomainBase.h>
#include <Domain/Factory.hpp>
#include <Element/Element.h>

ElementRecorder::ElementRecorder(const unsigned& T, const unsigned& B, const OutputType& L, const bool& R)
    : Recorder(T, CT_ELEMENTRECORDER, B, L, R) {}

void ElementRecorder::record(const shared_ptr<DomainBase>& D) {
    auto& t_obj = D->get_element(get_object_tag());

    if(t_obj == nullptr) {
        D->disable_recorder(get_tag());
        return;
    }

    insert(t_obj->record(get_variable_type()));

    if(if_record_time()) insert(D->get_factory()->get_current_time());
}

void ElementRecorder::print() { suanpan_info("An Element Recorder.\n"); }
