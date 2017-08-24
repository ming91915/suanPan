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

    if(tmp_ode == nullptr) suanpan_error("initialize() needs a valid ODE.\n");

    auto& ode_size = tmp_ode->getDimension();

    if(factory == nullptr)
        factory = make_shared<Workroom>(ode_size, SUANPAN_DISP);
    else if(ode_size != factory->get_dof_number())
        factory->set_analysis_type(SUANPAN_DISP);

    factory->initialize_displacement(ode_size);

    auto tmp_domain = make_shared<Domain>();
    tmp_domain->set_workroom(factory);

    get_convergence()->set_domain(tmp_domain);
}

int ODE_Solver::analyze(const double& T)
{
    auto& W = getWorkroom();
    auto& C = get_convergence();

    auto factor = .2;

    switch(get_class_tag()) {
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
        W->update_incre_time(step);
        if(update_status() == -1) return -1;
        if(C->if_converged()) {
            W->commit_status();
            time_left -= step;
        }
        step *= .8 * pow(C->get_tolerance() / C->get_error(), factor);
        if(step > time_left) step = time_left;
    }

    return 0;
}

void ODE_Solver::setODE(const shared_ptr<ODE>& E) { ode_system = E; }

const shared_ptr<ODE>& ODE_Solver::getODE() const { return ode_system; }

void ODE_Solver::setWorkroom(const shared_ptr<Workroom> W) { factory = W; }

const shared_ptr<Workroom>& ODE_Solver::getWorkroom() const { return factory; }
