#include "ODE_Solver.h"
#include <Convergence/Convergence.h>
#include <Domain/Domain.h>
#include <Domain/Workroom.h>
#include <Solver/ODE.h>

ODE_Solver::ODE_Solver(const unsigned& T,
    const unsigned& CT,
    const shared_ptr<ODE>& E,
    const shared_ptr<Convergence>& C,
    const shared_ptr<Workroom>& W)
    : Solver(T, CT, nullptr, C)
    , ode_system(E)
    , factory(W)
{
}

ODE_Solver::~ODE_Solver() {}

void ODE_Solver::initialize()
{
    auto& tmp_ode = getODE();

    if(tmp_ode == nullptr) throw invalid_argument("ODE_Solver needs a valid ODE.\n");

    auto& ode_size = tmp_ode->getDimension();

    if(factory == nullptr)
        factory = make_shared<Workroom>(ode_size, SUANPAN_DISP);
    else if(ode_size != factory->getNumberDOF())
        factory->setAnalysisType(SUANPAN_DISP);

    factory->initializeDisplacement(ode_size);

    auto tmp_domain = make_shared<Domain>();
    tmp_domain->setWorkroom(factory);

    getConvergence()->setDomain(tmp_domain);
}

int ODE_Solver::analyze(const double& T)
{
    auto& W = getWorkroom();
    auto& C = getConvergence();

    auto factor = .2;

    switch(getClassTag()) {
    case CT_BS23:
    case CT_RK23:
        factor = 1. / 3.;
        break;
    case CT_RK45:
    case CT_DP45:
    default:
        break;
    }

    auto time_left = T;
    auto step = time_left;

    while(time_left > 0.) {
        W->updateIncreTime(step);
        if(updateStatus() == -1) return -1;
        if(C->if_converged()) {
            W->commitStatus();
            time_left -= step;
        }
        step *= .8 * pow(C->getTolerance() / C->getError(), factor);
        if(step > time_left) step = time_left;
    }

    return 0;
}

void ODE_Solver::setODE(const shared_ptr<ODE>& E) { ode_system = E; }

const shared_ptr<ODE>& ODE_Solver::getODE() const { return ode_system; }

void ODE_Solver::setWorkroom(const shared_ptr<Workroom> W) { factory = W; }

const shared_ptr<Workroom>& ODE_Solver::getWorkroom() const { return factory; }
