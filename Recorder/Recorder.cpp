#include "Recorder.h"

Recorder::Recorder(const unsigned& T,
    const unsigned& CT,
    const unsigned& B,
    const OutputList& L,
    const bool& R)
    : Tag(T, CT)
    , object_tag(B)
    , variable_type(L)
    , variable_name(to_char(L))
    , record_time(R)
{
}

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
