#include "CentralDifference.h"
#include "Domain/Node.h"
#include <Domain/Domain.h>
#include <Domain/Workshop.h>

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
    const auto code = Integrator::initialize();

    if(code == 0) {
        auto& W = get_domain()->get_workshop();

        if(W->is_band() || W->is_symm()) {
            suanpan_error(
                "initialize() currently does not suppoort band or symmetric matrix.\n");
            return -1;
        }

        const auto eig_val = eig_sym(W->get_mass().i() * W->get_stiffness());

        max_dt = datum::pi / sqrt(eig_val.max());
    }

    return code;
}

void CentralDifference::update_parameter()
{
    auto& W = get_domain()->get_workshop();

    if(DT != W->get_incre_time() || W->get_pre_displacement().is_empty()) {
        DT = W->get_incre_time();
        if(DT > max_dt)
            suanpan_error("update_status() requires a smaller time increment.\n");

        C0 = 1. / DT / DT;
        C1 = .5 / DT;
        C2 = 2. * C0;
        C3 = 1. / C2;

        W->update_pre_displacement(W->get_current_displacement() -
            DT * W->get_current_velocity() + C3 * W->get_current_acceleration());
    }
}

void CentralDifference::update_stiffness()
{
    update_parameter();

    auto& W = get_domain()->get_workshop();

    get_stiffness(W) = C0 * W->get_mass() + C1 * W->get_damping();
}

void CentralDifference::update_resistance()
{
    update_parameter();

    auto& D = get_domain();
    auto& W = D->get_workshop();

    D->update_resistance();

    get_trial_resistance(W) +=
        (W->get_stiffness() - C2 * W->get_mass()) * W->get_current_displacement() +
        (C0 * W->get_mass() - C1 * W->get_damping()) * W->get_pre_displacement();
}

void CentralDifference::commit_status() const
{
    auto& D = get_domain();
    auto& W = D->get_workshop();

    W->update_current_velocity(
        C1 * (W->get_trial_displacement() - W->get_pre_displacement()));

    W->update_current_acceleration(
        C0 * (W->get_pre_displacement() - 2 * W->get_current_displacement() +
                 W->get_trial_displacement()));

    D->commit_status();

    auto& tmp_velocity = W->get_current_velocity();
    auto& tmp_acceleration = W->get_current_acceleration();
    for(const auto& I : get_node_pool(D)) {
        I->set_current_velocity(tmp_velocity(I->get_reordered_dof()));
        I->set_current_acceleration(tmp_acceleration(I->get_reordered_dof()));
    }
}
