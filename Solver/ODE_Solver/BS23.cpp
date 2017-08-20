#include "BS23.h"
#include <Domain/Workroom.h>
#include <Solver/ODE.h>

BS23::BS23(const unsigned& T,
    const shared_ptr<ODE>& D,
    const shared_ptr<Convergence>& C,
    const shared_ptr<Workroom>& W)
    : ODE_Solver(T, CT_BS23, D, C, W)
{
}

BS23::BS23(const shared_ptr<ODE>& D,
    const shared_ptr<Convergence>& C,
    const shared_ptr<Workroom>& W)
    : ODE_Solver(0, CT_BS23, D, C, W)
{
}

BS23::~BS23() {}

int BS23::updateStatus()
{
    auto& D = getODE();
    auto& W = getWorkroom();

    if(D == nullptr || W == nullptr) return -1;

    auto& c_time = W->getCurrentTime();
    auto& t_time = W->getTrialTime();
    auto& i_time = W->getIncreTime();
    auto& c_disp = W->getCurrentDisplacement();

    S1 = D->eval(c_time, c_disp);
    S2 = D->eval(c_time + .5 * i_time, c_disp + i_time * .5 * S1);
    S3 = D->eval(c_time + .75 * i_time, c_disp + i_time * .75 * S2);

    W->updateIncreDisplacement(i_time * (2. * S1 + 3. * S2 + 4. * S3) / 9.);

    S4 = D->eval(t_time, W->getTrialDisplacement());

    W->setError(norm(i_time * (-5. * S1 + 6. * S2 + 8. * S3 - 9. * S4) / 72.));

    return 0;
}

void BS23::print() { printf("A Bogacki--Shampine 2/3 Pair Solver object.\n"); }
