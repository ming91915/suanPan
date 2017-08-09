#include "Newmark.h"
#include <Domain/Domain.h>
#include <Domain/Workroom.h>

Newmark::Newmark(const unsigned& T,
    const shared_ptr<Domain>& D,
    const shared_ptr<Convergence>& C,
    const double& A,
    const double& B)
    : Solver(T, CT_NEWMARK, D, C)
    , alpha(A)
    , beta(B)
{
    if(beta < .5 || alpha < .25 * (.5 + beta) * (.5 + beta))
        throw invalid_argument("Newmark parameters are not acceptable.\n");
}

Newmark::Newmark(const shared_ptr<Domain>& D,
    const shared_ptr<Convergence>& C,
    const double& A,
    const double& B)
    : Solver(0, CT_NEWMARK, D, C)
    , alpha(A)
    , beta(B)
{
    if(beta < .5 || alpha < .25 * (.5 + beta) * (.5 + beta))
        throw invalid_argument("Newmark parameters are not acceptable.\n");
}

Newmark::~Newmark() {}

int Newmark::updateStatus()
{
    auto& W = getDomain()->getWorkroom();

    if(DT != W->getIncreTime()) {
        DT = W->getIncreTime();
        C2 = 1. / alpha / DT;
        C0 = C2 / DT;
        C1 = C2 * beta;
        C3 = .5 / alpha - 1.;
        C4 = beta / alpha - 1.;
        C5 = (C4 - 1.) * DT / 2.;
        C7 = DT * beta;
        C6 = DT - C7;
    }

    mat EK = W->getStiffness() + C0 * W->getMass() + C1 * W->getDamping();

    vec EP = W->getTrialLoad() +
        W->getMass() * (C0 * W->getCurrentDisplacement() + C2 * W->getCurrentVelocity() +
                           C3 * W->getCurrentAcceleration()) +
        W->getDamping() *
            (C1 * W->getCurrentDisplacement() + C4 * W->getCurrentVelocity() +
                C5 * W->getCurrentAcceleration());

    vec tmp_disp;

    if(!solve(tmp_disp, EK, EP)) return -1;

    W->updateTrialDisplacement(tmp_disp);

    //! TODO: Send Trial Displacement to NR Solver to Get Converged Displacement.

    W->updateTrialAcceleration(C0 * W->getIncreDisplacement() -
        C2 * W->getCurrentVelocity() - C3 * W->getCurrentAcceleration());
    W->updateTrialVelocity(W->getCurrentVelocity() + C6 * W->getCurrentAcceleration() +
        C7 * W->getTrialAcceleration());

    return 0;
}

void Newmark::print() { printf("A Newmark solver.\n"); }

int Newmark::analyze(const double&) { return 0; }
