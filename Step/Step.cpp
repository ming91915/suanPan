#include "Step.h"

Step::Step(const unsigned& T,
    const unsigned& CT,
    const shared_ptr<Solver>& S,
    const double& P)
    : Tag(T, CT)
    , time_period(P)
    , solver(S)
{
}

int Step::analyze() { return -1; }

void Step::setSolver(const shared_ptr<Solver>& S) { solver = S; }

const shared_ptr<Solver>& Step::getSolver() const { return solver; }

void Step::setTimePerid(const double& T) { time_period = T; }

const double& Step::getTimePeriod() const { return time_period; }
