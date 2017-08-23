/**
* @class NodeRecorder
* @brief A NodeRecorder class.
*
* @author T
* @date 24/08/2017
* @version 0.1.0
* @file NodeRecorder.h
* @addtogroup Recorder
* @{
*/

#ifndef NODERECORDER_H
#define NODERECORDER_H

#include <Recorder/Recorder.h>

class NodeRecorder : public Recorder
{
public:
    explicit NodeRecorder(const unsigned& = 0,
        const unsigned& = 0,
        const OutputList& = OutputList::NL,
        const bool& = true);

    void record(const shared_ptr<Domain>&) override;

    void print() override;
};

#endif

//! @}
