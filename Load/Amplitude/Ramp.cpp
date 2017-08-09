#include "Ramp.h"

Ramp::Ramp(const unsigned& T, const unsigned& CT)
    : Amplitude(T, CT)
{
}

Ramp::~Ramp() {}

double Ramp::getAmplitude(const double& T)
{
    if(T < 0.) return 0.;
    if(T > 1.) return 1.;
    return T;
}

void Ramp::print() { printf("Linear/Ramp.\n"); }
