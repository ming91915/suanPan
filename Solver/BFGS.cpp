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

int BFGS::update_status() { return 0; }

int BFGS::analyze(const double& T)
{
    auto& D = get_domain();
    auto& W = D->get_workroom();
    auto& C = get_convergence();

    if(W->is_band())
        suanpan_fatal("BFGS::analyze() currently does not suppoort band matrix.\n");

    auto time_left = T;
    auto step = time_left / 1.;

    auto tmp_size = W->get_dof_number();

    auto tmp_eye = eye(tmp_size, tmp_size);

    auto& tmp_ninja = get_ninja(W);

    while(time_left > 0.) {
        W->update_incre_time(step);

        D->update_stiffness();
        D->update_resistance();
        D->process();

        vec tmp_residual = W->get_trial_load() - W->get_trial_resistance();

        mat inv_stiffness;

        if(!inv(inv_stiffness, W->get_stiffness())) return -1;

        if(!solve(tmp_ninja, W->get_stiffness(), tmp_residual)) return -1;

        W->update_trial_displacement(W->get_trial_displacement() + W->get_ninja());
        D->update_trial_status();

        while(!C->if_converged()) {
            auto factor = dot(tmp_residual, tmp_ninja);
            mat tmp_a = tmp_eye - tmp_ninja * tmp_residual.t() / factor;
            inv_stiffness =
                tmp_a * inv_stiffness * tmp_a.t() + tmp_ninja * tmp_ninja.t() / factor;
            D->update_resistance();
            tmp_residual = W->get_trial_load() - W->get_trial_resistance();

            tmp_ninja = inv_stiffness * tmp_residual;

            W->update_trial_displacement(W->get_trial_displacement() + W->get_ninja());
            D->update_trial_status();
        }
        D->commit_status();
        time_left -= step;
    }

    return 0;
}

void BFGS::print() {}
