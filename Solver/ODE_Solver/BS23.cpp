#include "BS23.h"
#include <Domain/Workshop.h>
#include <Solver/ODE.h>

BS23::BS23(const unsigned& T,
    const shared_ptr<ODE>& D,
    const shared_ptr<Converger>& C,
    const shared_ptr<Workshop>& W)
    : ODE_Solver(T, CT_BS23, D, C, W)
{
}

BS23::BS23(const shared_ptr<ODE>& D,
    const shared_ptr<Converger>& C,
    const shared_ptr<Workshop>& W)
    : ODE_Solver(0, CT_BS23, D, C, W)
{
}

BS23::~BS23() {}

int BS23::update_status()
{
    auto& D = get_ode();
    auto& W = get_workroom();

    auto& c_time = W->get_current_time();
    auto& t_time = W->get_trial_time();
    auto& i_time = W->get_incre_time();
    auto& c_disp = W->get_current_displacement();

    S1 = D->eval(c_time, c_disp);
    S2 = D->eval(c_time + .5 * i_time, c_disp + i_time * .5 * S1);
    S3 = D->eval(c_time + .75 * i_time, c_disp + i_time * .75 * S2);

    W->update_incre_displacement(i_time * (2. * S1 + 3. * S2 + 4. * S3) / 9.);

    S4 = D->eval(t_time, W->get_trial_displacement());

    W->set_error(norm(i_time * (-5. * S1 + 6. * S2 + 8. * S3 - 9. * S4) / 72.));

    return 0;
}

void BS23::print() { suanpan_info("A Bogacki--Shampine 2/3 Pair Solver object.\n"); }
