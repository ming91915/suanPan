#include "CentralDifference.h"
#include <Domain/Domain.h>
#include <Domain/Workroom.h>

CentralDifference::CentralDifference(const unsigned& T,
    const shared_ptr<Domain>& D,
    const shared_ptr<Convergence>& C)
    : Solver(T, CT_CENTRALDIFFERENCE, D, C)
{
}

CentralDifference::CentralDifference(const shared_ptr<Domain>& D,
    const shared_ptr<Convergence>& C)
    : Solver(0, CT_CENTRALDIFFERENCE, D, C)
{
}

int CentralDifference::updateStatus()
{
    auto& W = getDomain()->getWorkroom();

    if(DT != W->getIncreTime() || W->getPreviousDisplacement().is_empty()) {
        DT = W->getIncreTime();
        C0 = 1. / DT / DT;
        C1 = .5 / DT;
        C2 = 2. * C0;
        C3 = 1. / C2;

        W->updatePreviousDisplacement(W->getCurrentDisplacement() -
            DT * W->getCurrentVelocity() + C3 * W->getCurrentAcceleration());
    }

    vec EP = W->getTrialLoad() -
        (W->getStiffness() - C2 * W->getMass()) * W->getCurrentDisplacement() -
        (C0 * W->getMass() - C1 * W->getDamping()) * W->getPreviousDisplacement();

    if(!solve(getTrialDisplacement(W), C0 * W->getMass() + C1 * W->getDamping(), EP,
           solve_opts::fast + solve_opts::no_approx))
        return -1;

    //! TODO: Send Trial Displacement to NR Solver to Get Converged Displacement.

    W->updateCurrentVelocity(
        C1 * (W->getTrialDisplacement() - W->getPreviousDisplacement()));

    W->updateCurrentAcceleration(
        C0 * (W->getPreviousDisplacement() - 2 * W->getCurrentDisplacement() +
                 W->getTrialDisplacement()));

    return 0;
}

int CentralDifference::analyze(const double& T)
{
    auto& D = getDomain();
    auto& W = D->getWorkroom();

    auto tmp_mass = conv_to<mat>::from(W->getMass());
    auto tmp_stiffness = conv_to<mat>::from(W->getStiffness());
    auto eig_val = eig_sym(tmp_mass.i() * tmp_stiffness);

    auto max_dt = datum::pi / sqrt(eig_val.max());

    return 0;
}
