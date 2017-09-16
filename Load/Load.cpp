#include "Load.h"
#include <Load/Amplitude/Amplitude.h>

const double Load::multiplier = 1E6;

Load::Load(const unsigned& T, const unsigned& CT, const unsigned& ST, const shared_ptr<Amplitude>& A)
    : Tag(T, CT)
    , step_tag(ST)
    , magnitude(A) {
    if(magnitude == nullptr) magnitude = make_shared<Amplitude>();
}

Load::~Load() { suanpan_debug("Load %u dtor() called.\n", get_tag()); }

int Load::process(const shared_ptr<DomainBase>&) { return -1; }

void Load::set_step_tag(const unsigned& T) { step_tag = T; }

const unsigned& Load::get_step_tag() const { return step_tag; }
