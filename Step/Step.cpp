#include "Step.h"
#include <Domain/Domain.h>
#include <Domain/Workroom.h>
#include <Solver/Integrator/PlainIntegrator.h>
#include <Solver/Solver.h>

Step::Step(const unsigned& T, const unsigned& CT, const double& P)
    : Tag(T, CT)
    , time_period(P)
{
    enable_band();
    enable_symm();
}

Step::~Step() {}

const bool& Step::is_initialized() const { return initialized; }

int Step::initialize()
{
    if(!initialized) initialized = true;

    if(factory == nullptr) factory = make_shared<Workroom>();

    if(database == nullptr) {
        suanpan_error("initialize() needs a valid Domain.\n");
        return -1;
    }

    if(solver == nullptr) {
        suanpan_error("initialize() needs a valid Solver.\n");
        return -1;
    }

    if(tester == nullptr) {
        suanpan_error("initialize() needs a valid Converger.\n");
        return -1;
    }

    auto code = 0;

    database->set_workroom(factory);
    if(!database->is_initialized()) code += database->initialize();

    switch(get_class_tag()) {
    case CT_STATIC:
        factory->set_analysis_type(AnalysisType::STATICS);
        if(modifier == nullptr) modifier = make_shared<PlainIntegrator>();
        break;
    case CT_DYNAMIC:
        factory->set_analysis_type(AnalysisType::DYNAMICS);
        if(modifier == nullptr) {
            suanpan_error("initialize() needs a valid Integrator.\n");
            return -1;
        }
        break;
    case CT_FREQUENCE:
        factory->set_analysis_type(AnalysisType::EIGEN);
        break;
    default:
        break;
    }

    if(is_symm())
        factory->enable_symm();
    else
        factory->disable_symm();

    if(is_band())
        factory->enable_band();
    else
        factory->disable_band();

    if(!factory->is_initialized()) code += factory->initialize();

    solver->set_domain(database);
    solver->set_converger(tester);
    solver->set_integrator(modifier);

    if(!solver->is_initialized()) code += solver->initialize();

    return code;
}

int Step::analyze() { return -1; }

void Step::set_workroom(const shared_ptr<Workroom>& F) { factory = F; }

const shared_ptr<Workroom>& Step::get_workroom() const { return factory; }

void Step::set_domain(const shared_ptr<Domain>& D) { database = D; }

const shared_ptr<Domain>& Step::get_domain() const { return database; }

void Step::set_solver(const shared_ptr<Solver>& S) { solver = S; }

const shared_ptr<Solver>& Step::get_solver() const { return solver; }

void Step::set_converger(const shared_ptr<Converger>& C) { tester = C; }

const shared_ptr<Converger>& Step::get_converger() const { return tester; }

void Step::set_integrator(const shared_ptr<Integrator>& G) { modifier = G; }

const shared_ptr<Integrator>& Step::get_integrator() const { return modifier; }

void Step::set_time_perid(const double& T) { time_period = T; }

const double& Step::get_time_period() const { return time_period; }

const bool& Step::is_symm() const { return symm_mat; }

const bool& Step::is_band() const { return band_mat; }

void Step::enable_band() { band_mat = true; }

void Step::disable_band() { band_mat = false; }

void Step::enable_symm() { symm_mat = true; }

void Step::disable_symm() { symm_mat = false; }
