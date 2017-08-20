#include "RK23.h"
#include <Domain/Workroom.h>
#include <Solver/ODE.h>

RK23::RK23(const unsigned& T,
    const shared_ptr<ODE>& D,
    const shared_ptr<Convergence>& C,
    const shared_ptr<Workroom>& W)
    : ODE_Solver(T, CT_RK23, D, C, W)
{
}

RK23::RK23(const shared_ptr<ODE>& D,
    const shared_ptr<Convergence>& C,
    const shared_ptr<Workroom>& W)
    : ODE_Solver(0, CT_RK23, D, C, W)
{
}

RK23::~RK23() {}

int RK23::updateStatus()
{
    auto& D = getODE();
    auto& W = getWorkroom();

    auto& c_time = W->getCurrentTime();
    auto& t_time = W->getTrialTime();
    auto& i_time = W->getIncreTime();
    auto& c_disp = W->getCurrentDisplacement();

    S1 = D->eval(c_time, c_disp);
    S2 = D->eval(t_time, c_disp + i_time * S1);
    S3 = D->eval(c_time + .5 * i_time, c_disp + i_time * .25 * (S1 + S2));

    W->updateIncreDisplacement(i_time * (S1 + S2 + 4. * S3) / 6.);

    W->setError(norm(i_time * (S1 + S2 - 2. * S3) / 3.));

    return 0;
}

void RK23::print() { printf("A Runge--Kutta 2/3 Solver Object.\n"); }
