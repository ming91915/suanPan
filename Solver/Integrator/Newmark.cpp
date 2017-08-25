#include "Newmark.h"
#include <Domain/Domain.h>
#include <Domain/Workroom.h>

Newmark::Newmark(const unsigned& T,
    const shared_ptr<Domain>& D,
    const double& A,
    const double& B)
    : Integrator(T, CT_NEWMARK, D)
    , alpha(A)
    , beta(B)
{
    if(beta < .5 || alpha < .25 * (.5 + beta) * (.5 + beta))
        suanpan_error("Newmark() parameters are not acceptable.\n");
}

Newmark::Newmark(const shared_ptr<Domain>& D, const double& A, const double& B)
    : Integrator(0, CT_NEWMARK, D)
    , alpha(A)
    , beta(B)
{
    if(beta < .5 || alpha < .25 * (.5 + beta) * (.5 + beta))
        suanpan_error("Newmark() parameters are not acceptable.\n");
}

Newmark::~Newmark() {}

int Newmark::initialize()
{
    auto code = Integrator::initialize();

    if(code == 0) {
        auto& W = get_domain()->get_workroom();

        if(W->is_band() || W->is_symm()) {
            suanpan_error(
                "initialize() currently does not suppoort band or symmetric matrix.\n");
            return -1;
        }
    }

    return code;
}

int Newmark::update_status()
{
    auto& D = get_domain();
    auto& W = D->get_workroom();

    if(DT != W->get_incre_time()) {
        DT = W->get_incre_time();
        C2 = 1. / alpha / DT;
        C0 = C2 / DT;
        C1 = C2 * beta;
        C3 = .5 / alpha - 1.;
        C4 = beta / alpha - 1.;
        C5 = (C4 - 1.) * DT / 2.;
        C7 = DT * beta;
        C6 = DT - C7;
    }

    D->update_stiffness();

    get_stiffness(W) += C0 * W->get_mass() + C1 * W->get_damping();

    D->update_resistance();

    get_trial_resistance(W) -= W->get_mass() *
            (C0 * W->get_current_displacement() + C2 * W->get_current_velocity() +
                C3 * W->get_current_acceleration()) -
        W->get_damping() *
            (C1 * W->get_current_displacement() + C4 * W->get_current_velocity() +
                C5 * W->get_current_acceleration());

    return 0;
}

void Newmark::commit_status()
{
    auto& W = get_domain()->get_workroom();

    W->update_trial_acceleration(C0 * W->get_incre_displacement() -
        C2 * W->get_current_velocity() - C3 * W->get_current_acceleration());
    W->update_trial_velocity(W->get_current_velocity() +
        C6 * W->get_current_acceleration() + C7 * W->get_trial_acceleration());
}

void Newmark::print() { suanpan_info("A Newmark solver.\n"); }
