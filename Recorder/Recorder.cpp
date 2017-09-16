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

Recorder::Recorder(const unsigned& T, const unsigned& CT, const unsigned& B, const OutputList& L, const bool& R)
    : Tag(T, CT)
    , object_tag(B)
    , variable_type(L)
    , variable_name(to_char(L))
    , record_time(R) {}

Recorder::~Recorder() {}

void Recorder::set_object_tag(const unsigned& T) { object_tag = T; }

const unsigned& Recorder::get_object_tag() const { return object_tag; }

void Recorder::set_variable_type(const OutputList& T) { variable_type = T; }

const OutputList& Recorder::get_variable_type() const { return variable_type; }

const bool& Recorder::if_record_time() const { return record_time; }

void Recorder::insert(const double& T) { time_pool.push_back(T); }

void Recorder::insert(const vector<vec>& D) { data_pool.push_back(D); }

const vector<vector<vec>>& Recorder::get_data_pool() const { return data_pool; }

void Recorder::record(const shared_ptr<Domain>&) {}

void Recorder::print() { suanpan_info("print() needs to be overwritten.\n"); }
