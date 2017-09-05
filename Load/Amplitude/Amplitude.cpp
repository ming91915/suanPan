#include "Amplitude.h"

Amplitude::Amplitude(const unsigned& T, const unsigned& CT)
    : Tag(T, CT) {}

Amplitude::~Amplitude() {}

double Amplitude::get_amplitude(const double& T) {
    if(T < 0.) return 0.;
    if(T > 1.) return 1.;
    return T;
}

void Amplitude::print() { suanpan_debug("This is an Amplitude object.\n"); }
