/**
 * @class Recorder
 * @brief A Recorder class.
 * @author T
 * @date 27/07/2017
 * @version 0.1.0
 * @file Recorder.h
 */

#ifndef RECORDER_H
#define RECORDER_H

#include <Domain/Tag.h>

using std::vector;

class Recorder : public Tag
{
protected:
    vector<vec> pool; /**< recorded data */
public:
    explicit Recorder(const unsigned& T = 0, const unsigned& CT = CT_RECORDER)
        : Tag(T, CT)
    {
    }
    virtual ~Recorder() {}

    virtual void record() = 0;
};

#endif
