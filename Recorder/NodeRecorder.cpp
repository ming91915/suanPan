#include "NodeRecorder.h"
#include <Domain/Domain.h>
#include <Domain/Node.h>
#include <Domain/Workroom.h>

NodeRecorder::NodeRecorder(const unsigned& T,
    const unsigned& B,
    const OutputList& L,
    const bool& R)
    : Recorder(T, CT_NODERECORDER, B, L, R)
{
    suanpan_info("NodeRecorder %u ctor() called.\n", T);
}

void NodeRecorder::record(const shared_ptr<Domain>& D)
{
    auto& tmp_obj = D->get_node(get_object_tag());

    insert(tmp_obj->record(get_variable_type()));

    if(if_record_time()) insert(D->get_workroom()->get_current_time());
}

void NodeRecorder::print()
{
    for(const auto& I : get_data_pool()) {
        for(const auto& J : I) J.print();
        suanpan_info("\n");
    }
}
