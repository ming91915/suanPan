#include "DP45.h"
#include <Domain/Workroom.h>
#include <Solver/ODE.h>

DP45::DP45(const unsigned& T,
    const shared_ptr<ODE>& D,
    const shared_ptr<Convergence>& C,
    const shared_ptr<Workroom>& W)
    : ODE_Solver(T, CT_DP45, D, C, W)
{
}

DP45::DP45(const shared_ptr<ODE>& D,
    const shared_ptr<Convergence>& C,
    const shared_ptr<Workroom>& W)
    : ODE_Solver(0, CT_DP45, D, C, W)
{
}

DP45::~DP45() {}

int DP45::updateStatus()
{
    auto& D = getODE();
    auto& W = getWorkroom();

    S1 = D->getDY(W->getCurrentTime(), W->getCurrentDisplacement());
    S2 = D->getDY(W->getCurrentTime() + .2 * W->getIncreTime(),
        W->getCurrentDisplacement() + W->getIncreTime() * .2 * S1);
    S3 = D->getDY(W->getCurrentTime() + .3 * W->getIncreTime(),
        W->getCurrentDisplacement() + W->getIncreTime() * (.075 * S1 + .225 * S2));
    S4 = D->getDY(W->getCurrentTime() + .8 * W->getIncreTime(),
        W->getCurrentDisplacement() +
            W->getIncreTime() * (44. / 45. * S1 - 56. / 15. * S2 + 32. / 9. * S3));
    S5 = D->getDY(W->getCurrentTime() + 8. / 9. * W->getIncreTime(),
        W->getCurrentDisplacement() +
            W->getIncreTime() * (19372. / 6561. * S1 - 25360. / 2187. * S2 +
                                    64448. / 6561. * S3 - 212. / 729. * S4));
    S6 = D->getDY(W->getTrialTime(),
        W->getCurrentDisplacement() +
            W->getIncreTime() *
                (9017. / 3168. * S1 - 355. / 33. * S2 + 46732. / 5247. * S3 +
                    49. / 176. * S4 - 5103. / 18656. * S5));

    W->updateIncreDisplacement(
        W->getIncreTime() * (35. / 384. * S1 + 500. / 1113. * S3 + 125. / 192. * S4 -
                                2187. / 6784. * S5 + 11. / 84. * S6));

    S7 = D->getDY(W->getTrialTime(), W->getTrialDisplacement());

    W->setError(norm(W->getIncreTime() *
        (71. / 57600. * S1 - 71. / 16695. * S3 + 71. / 1920. * S4 -
            17253. / 339200. * S5 + 22. / 525. * S6 - 1. / 40. * S7)));

    return 0;
}

void DP45::print() { printf("A Dormand--Prince 4/5 Pair Solver object.\n"); }
