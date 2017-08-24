#include "RK45.h"
#include <Domain/Workroom.h>
#include <Solver/ODE.h>

RK45::RK45(const unsigned& T,
    const shared_ptr<ODE>& D,
    const shared_ptr<Convergence>& C,
    const shared_ptr<Workroom>& W)
    : ODE_Solver(T, CT_RK45, D, C, W)
{
}

RK45::RK45(const shared_ptr<ODE>& D,
    const shared_ptr<Convergence>& C,
    const shared_ptr<Workroom>& W)
    : ODE_Solver(0, CT_RK45, D, C, W)
{
}

RK45::~RK45() {}

int RK45::update_status()
{
    auto& D = getODE();
    auto& W = getWorkroom();

    S1 = D->eval(W->get_current_time(), W->get_current_displacement());
    S2 = D->eval(W->get_current_time() + .25 * W->get_incre_time(),
        W->get_current_displacement() + W->get_incre_time() * .25 * S1);
    S3 = D->eval(W->get_current_time() + .375 * W->get_incre_time(),
        W->get_current_displacement() +
            W->get_incre_time() * (.09375 * S1 + .28125 * S2));
    S4 = D->eval(W->get_current_time() + 12. / 13. * W->get_incre_time(),
        W->get_current_displacement() +
            W->get_incre_time() *
                (1932. / 2197. * S1 - 7200. / 2197. * S2 + 7296. / 2197. * S3));
    S5 = D->eval(W->get_trial_time(),
        W->get_current_displacement() +
            W->get_incre_time() *
                (439. / 216. * S1 - 8. * S2 + 3680. / 513. * S3 - 845. / 4104. * S4));
    S6 = D->eval(W->get_current_time() + .5 * W->get_incre_time(),
        W->get_current_displacement() +
            W->get_incre_time() * (-8. / 27. * S1 + 2. * S2 - 3544. / 2565. * S3 +
                                      1859. / 4104. * S4 - 11. / 40. * S5));

    W->update_incre_displacement(
        W->get_incre_time() * (16. / 135. * S1 + 6656. / 12825. * S3 +
                                  28561. / 56430. * S4 - 9. / 50. * S5 + 2. / 55. * S6));

    W->set_error(norm(
        W->get_incre_time() * (1. / 360. * S1 - 128. / 4275. * S3 - 2197. / 75240. * S4 +
                                  1. / 50. * S5 + 2. / 55. * S6)));

    return 0;
}

void RK45::print() { printf("A Runge--Kutta--Fehlberg 4/5 Solver Object.\n"); }
