#include "Static.h"
#include "Solver/Newton.h"

Static::Static(const unsigned& T, const shared_ptr<Solver>& S, const double& P)
    : Step(T, CT_STATIC, S, P)
{
}

int Static::analyze()
{
    if(get_solver() == nullptr) {
        suanpan_fatal("analyze() needs a valid Solver.\n");
        return -1;
    }

    auto& tmp_solver = get_solver();
    auto& tmp_time = get_time_period();

    tmp_solver->enable_band();
    tmp_solver->enable_symm();

    tmp_solver->initialize();

    return tmp_solver->analyze(tmp_time);
}
