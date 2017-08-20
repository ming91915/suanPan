#include "Load.h"
#include "Amplitude/Amplitude.h"

Load::Load(const unsigned& T,
    const unsigned& CT,
    const unsigned& ST,
    const shared_ptr<Amplitude>& A)
    : Tag(T, CT)
    , step_tag(ST)
    , magnitude(A)
{
    if(magnitude == nullptr) magnitude = make_shared<Amplitude>();
}

Load::~Load() { suanpan_debug("Load %u dtor() Called.\n", getTag()); }

int Load::process(const shared_ptr<Domain>&) { return -1; }

const unsigned& Load::getStepTag() const { return step_tag; }
