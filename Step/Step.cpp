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

void Step::set_solver(const shared_ptr<Solver>& S) { solver = S; }

const shared_ptr<Solver>& Step::get_solver() const { return solver; }

void Step::set_time_perid(const double& T) { time_period = T; }

const double& Step::get_time_period() const { return time_period; }
