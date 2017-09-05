#include "ODE_Solver.h"
#include <Converger/Converger.h>
#include <Domain/Workshop.h>
#include <Solver/ODE.h>

ODE_Solver::ODE_Solver(const unsigned& T, const unsigned& CT, const shared_ptr<ODE>& E, const shared_ptr<Converger>& C, const shared_ptr<Workshop>& W)
    : Solver(T, CT, C, nullptr)
    , ode_system(E)
    , factory(W) {}

ODE_Solver::~ODE_Solver() {}

int ODE_Solver::initialize() {
    if(ode_system == nullptr) {
        suanpan_error("initialize() needs a valid ODE.\n");
        return -1;
    }

    auto& ode_size = ode_system->get_dimension();

    if(factory == nullptr)
        factory = make_shared<Workshop>(ode_size, AnalysisType::DISP);
    else if(ode_size != factory->get_dof_number())
        factory->set_analysis_type(AnalysisType::DISP);

    factory->initialize_displacement(ode_size);

    auto& tmp_converger = get_converger();
    if(tmp_converger == nullptr) {
        suanpan_error("initialize() needs a valid Converger.\n");
        return -1;
    }

    return 0;
}

int ODE_Solver::update_status() { return -1; }

int ODE_Solver::analyze(const unsigned& T) {
    auto& tmp_converger = get_converger();

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
        if(tmp_converger->if_converged()) {
            factory->commit_status();
            time_left -= step;
        }
        step *= .8 * pow(tmp_converger->get_tolerance() / tmp_converger->get_error(), factor);
        if(step > time_left) step = time_left;
    }

    return 0;
}

void ODE_Solver::set_ode(const shared_ptr<ODE>& E) { ode_system = E; }

const shared_ptr<ODE>& ODE_Solver::get_ode() const { return ode_system; }

void ODE_Solver::set_workroom(const shared_ptr<Workshop>& W) { factory = W; }

const shared_ptr<Workshop>& ODE_Solver::get_workroom() const { return factory; }
