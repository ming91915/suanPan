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

    S1 = D->getDY(W->getCurrentTime(), W->getCurrentDisplacement());
    S2 =
        D->getDY(W->getTrialTime(), W->getCurrentDisplacement() + W->getIncreTime() * S1);
    S3 = D->getDY(W->getCurrentTime() + .5 * W->getIncreTime(),
        W->getCurrentDisplacement() + W->getIncreTime() * .25 * (S1 + S2));

    W->updateIncreDisplacement(W->getIncreTime() * (S1 + S2 + 4. * S3) / 6.);

    W->setError(norm(W->getIncreTime() * (S1 + S2 - 2. * S3) / 3.));

    return 0;
}

void RK23::print() { printf("A Runge--Kutta 2/3 Solver Object.\n"); }
