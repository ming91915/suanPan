#include "BFGS.h"
#include <Convergence/Convergence.h>
#include <Domain/Domain.h>
#include <Domain/Workroom.h>

BFGS::BFGS(const unsigned& T,
    const shared_ptr<Domain>& D,
    const shared_ptr<Convergence>& C)
    : Solver(T, CT_BFGS, D, C)
{
}

BFGS::BFGS(const shared_ptr<Domain>& D, const shared_ptr<Convergence>& C)
    : Solver(0, CT_BFGS, D, C)
{
}

int BFGS::updateStatus() { return 0; }

int BFGS::analyze(const double& T)
{
    auto& D = getDomain();
    auto& W = D->getWorkroom();
    auto& C = getConvergence();

    auto time_left = T;
    auto step = time_left / 1.;

    auto tmp_size = W->getNumberDOF();

    auto tmp_eye = eye(tmp_size, tmp_size);

    auto& tmp_ninja = getNinja(W);

    while(time_left > 0.) {
        W->updateIncreTime(step);

        D->updateStiffness();
        D->updateResistance();
        D->process();

        vec tmp_residual = W->getTrialLoad() - W->getTrialResistance();

        mat inv_stiffness;

        if(!inv(inv_stiffness, W->getStiffness())) return -1;

        if(!solve(tmp_ninja, W->getStiffness(), tmp_residual)) return -1;

        W->updateTrialDisplacement(W->getTrialDisplacement() + W->getNinja());
        D->updateTrialStatus();

        while(!C->if_converged()) {
            auto factor = as_scalar(tmp_residual.t() * tmp_ninja);
            mat tmp_a = tmp_eye - tmp_ninja * tmp_residual.t() / factor;
            inv_stiffness =
                tmp_a * inv_stiffness * tmp_a.t() + tmp_ninja * tmp_ninja.t() / factor;
            D->updateResistance();
            tmp_residual = W->getTrialLoad() - W->getTrialResistance();

            tmp_ninja = inv_stiffness * tmp_residual;

            W->updateTrialDisplacement(W->getTrialDisplacement() + W->getNinja());
            D->updateTrialStatus();
        }
        D->commitStatus();
        time_left -= step;
    }

    return 0;
}

void BFGS::print() {}
