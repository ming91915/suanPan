#include "CentralDifference.h"
#include <Domain/Domain.h>
#include <Domain/Workroom.h>

CentralDifference::CentralDifference(const unsigned& T, const shared_ptr<Domain>& D)
    : Integrator(T, CT_CENTRALDIFFERENCE, D)
{
}

CentralDifference::CentralDifference(const shared_ptr<Domain>& D)
    : Integrator(0, CT_CENTRALDIFFERENCE, D)
{
}

int CentralDifference::initialize()
{
    auto code = Integrator::initialize();

    if(code == 0) {
        auto& D = get_domain();
        auto& W = D->get_workroom();

        if(W->is_band() || W->is_symm()) {
            suanpan_error(
                "initialize() currently does not suppoort band or symmetric matrix.\n");
            return -1;
        }

        auto eig_val = eig_sym(W->get_mass().i() * W->get_stiffness());

        max_dt = datum::pi / sqrt(eig_val.max());
    }

    return code;
}

int CentralDifference::update_status()
{
    auto& D = get_domain();
    auto& W = get_domain()->get_workroom();

    if(DT != W->get_incre_time() || W->get_pre_displacement().is_empty()) {
        DT = W->get_incre_time();
        if(DT > max_dt) {
            suanpan_error("update_status() requires a smaller time increment.\n");
            return -1;
        }
        C0 = 1. / DT / DT;
        C1 = .5 / DT;
        C2 = 2. * C0;
        C3 = 1. / C2;

        W->update_pre_displacement(W->get_current_displacement() -
            DT * W->get_current_velocity() + C3 * W->get_current_acceleration());
    }

    get_stiffness(W) = C0 * W->get_mass() + C1 * W->get_damping();

    D->update_resistance();

    get_trial_resistance(W) +=
        (W->get_stiffness() - C2 * W->get_mass()) * W->get_current_displacement() +
        (C0 * W->get_mass() - C1 * W->get_damping()) * W->get_pre_displacement();

    return 0;
}

void CentralDifference::commit_status()
{
    auto& W = get_domain()->get_workroom();

    W->update_current_velocity(
        C1 * (W->get_trial_displacement() - W->get_pre_displacement()));

    W->update_current_acceleration(
        C0 * (W->get_pre_displacement() - 2 * W->get_current_displacement() +
                 W->get_trial_displacement()));
}
