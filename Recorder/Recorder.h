/**
 * @class Recorder
 * @brief A Recorder class.
 * @author T
 * @date 27/07/2017
 * @version 0.1.0
 * @file Recorder.h
 * @{
 */

#ifndef RECORDER_H
#define RECORDER_H

#include "OutputList.h"
#include <Domain/Tag.h>

class Domain;

using std::vector;

class Recorder : public Tag {
    unsigned object_tag;
    OutputList variable_type;
    const char* variable_name;
    vector<double> time_pool;      /**< recorded data */
    vector<vector<vec>> data_pool; /**< recorded data */

    bool record_time;

public:
    explicit Recorder(const unsigned& = 0, const unsigned& = CT_RECORDER, const unsigned& = 0, const OutputList& = OutputList::NL, const bool& = true);
    Recorder(const Recorder&) = delete;
    Recorder& operator=(const Recorder&) = delete;
    virtual ~Recorder();

    void set_object_tag(const unsigned&);
    const unsigned& get_object_tag() const;

    void set_variable_type(const OutputList&);
    const OutputList& get_variable_type() const;

    const bool& if_record_time() const;

    void insert(const double&);
    void insert(const vector<vec>&);

    const vector<vector<vec>>& get_data_pool() const;

    virtual void record(const shared_ptr<Domain>&) = 0;

    void print() override;
};

#endif

//! @}
