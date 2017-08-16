#include "Bead.h"
#include <Domain/Domain.h>
#include <Recorder/Recorder.h>
#include <Step/Step.h>

void Bead::insert(const shared_ptr<Domain>& D) { domains.insert({ D->getTag(), D }); }

void Bead::insert(const shared_ptr<Step>& S) { steps.insert({ S->getTag(), S }); }

void Bead::insert(const shared_ptr<Recorder>& R) { loggers.insert({ R->getTag(), R }); }
