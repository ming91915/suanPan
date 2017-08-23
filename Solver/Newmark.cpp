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
        throw invalid_argument("Newmark() parameters are not acceptable.\n");
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
        throw invalid_argument("Newmark() parameters are not acceptable.\n");
}

Newmark::~Newmark() {}

int Newmark::updateStatus()
{
    auto& W = getDomain()->get_workroom();

    if(W->is_band()) {
        printf("Newmark::update_status() currently does not suppoort band matrix.\n");
        return -1;
    }

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

    mat EK = W->get_stiffness() + C0 * W->get_mass() + C1 * W->get_damping();

    vec EP = W->get_trial_load() +
        W->get_mass() *
            (C0 * W->get_current_displacement() + C2 * W->get_current_velocity() +
                C3 * W->get_current_acceleration()) +
        W->get_damping() *
            (C1 * W->get_current_displacement() + C4 * W->get_current_velocity() +
                C5 * W->get_current_acceleration());

    vec tmp_disp;

    if(!solve(tmp_disp, EK, EP)) return -1;

    W->update_trial_displacement(tmp_disp);

    //! TODO: Send Trial Displacement to NR Solver to Get Converged Displacement.

    W->update_trial_acceleration(C0 * W->get_incre_displacement() -
        C2 * W->get_current_velocity() - C3 * W->get_current_acceleration());
    W->update_trial_velocity(W->get_current_velocity() +
        C6 * W->get_current_acceleration() + C7 * W->get_trial_acceleration());

    return 0;
}

void Newmark::print() { printf("A Newmark solver.\n"); }

int Newmark::analyze(const double&) { return 0; }
