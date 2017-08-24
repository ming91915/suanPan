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
    : Solver(T, CT, nullptr)
    , ode_system(E)
    , factory(W)
{
}

ODE_Solver::~ODE_Solver() {}

int ODE_Solver::initialize()
{
    if(ode_system == nullptr) {
        suanpan_error("initialize() needs a valid ODE.\n");
        return -1;
    }

    auto& ode_size = ode_system->getDimension();

    if(factory == nullptr)
        factory = make_shared<Workroom>(ode_size, SUANPAN_DISP);
    else if(ode_size != factory->get_dof_number())
        factory->set_analysis_type(SUANPAN_DISP);

    factory->initialize_displacement(ode_size);

    auto tmp_domain = make_shared<Domain>();
    tmp_domain->set_workroom(factory);

    converger->set_domain(tmp_domain);

    return 0;
}

int ODE_Solver::update_status() { return -1; }

int ODE_Solver::analyze(const unsigned& T)
{
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

    auto time_left = 1.;
    auto step = time_left;

    while(time_left > 0.) {
        factory->update_incre_time(step);
        if(update_status() != 0) return -1;
        if(converger->if_converged()) {
            factory->commit_status();
            time_left -= step;
        }
        step *= .8 * pow(converger->get_tolerance() / converger->get_error(), factor);
        if(step > time_left) step = time_left;
    }

    return 0;
}

void ODE_Solver::set_ode(const shared_ptr<ODE>& E) { ode_system = E; }

const shared_ptr<ODE>& ODE_Solver::get_ode() const { return ode_system; }

void ODE_Solver::set_convergence(const shared_ptr<Convergence>& C) { converger = C; }

const shared_ptr<Convergence>& ODE_Solver::get_convergence() const { return converger; }

void ODE_Solver::set_workroom(const shared_ptr<Workroom>& W) { factory = W; }

const shared_ptr<Workroom>& ODE_Solver::get_workroom() const { return factory; }
