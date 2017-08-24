#include "CentralDifference.h"
#include <Domain/Domain.h>
#include <Domain/Workroom.h>

CentralDifference::CentralDifference(const unsigned& T, const shared_ptr<Domain>& D)
    : Solver(T, CT_CENTRALDIFFERENCE, D)
{
}

CentralDifference::CentralDifference(const shared_ptr<Domain>& D)
    : Solver(0, CT_CENTRALDIFFERENCE, D)
{
}

int CentralDifference::update_status()
{
    auto& W = get_domain()->get_workroom();

    if(DT != W->get_incre_time() || W->get_pre_displacement().is_empty()) {
        DT = W->get_incre_time();
        C0 = 1. / DT / DT;
        C1 = .5 / DT;
        C2 = 2. * C0;
        C3 = 1. / C2;

        W->update_pre_displacement(W->get_current_displacement() -
            DT * W->get_current_velocity() + C3 * W->get_current_acceleration());
    }

    vec EP = W->get_trial_load() -
        (W->get_stiffness() - C2 * W->get_mass()) * W->get_current_displacement() -
        (C0 * W->get_mass() - C1 * W->get_damping()) * W->get_pre_displacement();

    if(!solve(get_trial_displacement(W), C0 * W->get_mass() + C1 * W->get_damping(), EP,
           solve_opts::fast + solve_opts::no_approx))
        return -1;

    //! TODO: SEND TRIAL DISPLACEMENT TO NR SOLVER TO GET CONVERGED DISPLACEMENT.

    W->update_current_velocity(
        C1 * (W->get_trial_displacement() - W->get_pre_displacement()));

    W->update_current_acceleration(
        C0 * (W->get_pre_displacement() - 2 * W->get_current_displacement() +
                 W->get_trial_displacement()));

    return 0;
}

int CentralDifference::analyze(const unsigned& T)
{
    auto& D = get_domain();
    auto& W = D->get_workroom();

    if(W->is_band()) {
        suanpan_error("analyze() currently does not suppoort band matrix.\n");
        return -1;
    }

    auto eig_val = eig_sym(W->get_mass().i() * W->get_stiffness());

    max_dt = datum::pi / sqrt(eig_val.max());

    return 0;
}
