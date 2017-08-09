#include "RK45.h"
#include <Domain/Workroom.h>
#include <Solver/ODE.h>

RK45::RK45(const unsigned& T,
    const shared_ptr<ODE>& D,
    const shared_ptr<Convergence>& C,
    const shared_ptr<Workroom>& W)
    : ODE_Solver(T, CT_RK45, D, C, W)
{
}

RK45::RK45(const shared_ptr<ODE>& D,
    const shared_ptr<Convergence>& C,
    const shared_ptr<Workroom>& W)
    : ODE_Solver(0, CT_RK45, D, C, W)
{
}

RK45::~RK45() {}

int RK45::updateStatus()
{
    auto& D = getODE();
    auto& W = getWorkroom();

    S1 = D->getDY(W->getCurrentTime(), W->getCurrentDisplacement());
    S2 = D->getDY(W->getCurrentTime() + .25 * W->getIncreTime(),
        W->getCurrentDisplacement() + W->getIncreTime() * .25 * S1);
    S3 = D->getDY(W->getCurrentTime() + .375 * W->getIncreTime(),
        W->getCurrentDisplacement() + W->getIncreTime() * (.09375 * S1 + .28125 * S2));
    S4 = D->getDY(W->getCurrentTime() + 12. / 13. * W->getIncreTime(),
        W->getCurrentDisplacement() +
            W->getIncreTime() *
                (1932. / 2197. * S1 - 7200. / 2197. * S2 + 7296. / 2197. * S3));
    S5 = D->getDY(W->getTrialTime(),
        W->getCurrentDisplacement() +
            W->getIncreTime() *
                (439. / 216. * S1 - 8. * S2 + 3680. / 513. * S3 - 845. / 4104. * S4));
    S6 = D->getDY(W->getCurrentTime() + .5 * W->getIncreTime(),
        W->getCurrentDisplacement() +
            W->getIncreTime() * (-8. / 27. * S1 + 2. * S2 - 3544. / 2565. * S3 +
                                    1859. / 4104. * S4 - 11. / 40. * S5));

    W->updateIncreDisplacement(
        W->getIncreTime() * (16. / 135. * S1 + 6656. / 12825. * S3 +
                                28561. / 56430. * S4 - 9. / 50. * S5 + 2. / 55. * S6));

    W->setError(norm(
        W->getIncreTime() * (1. / 360. * S1 - 128. / 4275. * S3 - 2197. / 75240. * S4 +
                                1. / 50. * S5 + 2. / 55. * S6)));

    return 0;
}

void RK45::print() { printf("A Runge--Kutta--Fehlberg 4/5 Solver Object.\n"); }
