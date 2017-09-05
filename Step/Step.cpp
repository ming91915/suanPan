#include "Step.h"
#include "Converger/Converger.h"
#include <Domain/Domain.h>
#include <Domain/Workshop.h>
#include <Solver/Integrator/Integrator.h>
#include <Solver/Solver.h>

Step::Step(const unsigned& T, const unsigned& CT, const double& P)
    : Tag(T, CT)
    , time_period(P) {
    suanpan_debug("Step %u ctor() called.\n", T);
}

Step::~Step() { suanpan_debug("Step %u dtor() called.\n", get_tag()); }

const bool& Step::is_updated() const { return updated; }

int Step::initialize() {
    if(updated) return 0;

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

    if(factory == nullptr) factory = make_shared<Workshop>();

    factory->set_symm(symm_mat);
    factory->set_band(band_mat);

    database->set_workshop(factory);
    database->initialize();

    switch(get_class_tag()) {
    case CT_STATIC:
        factory->set_analysis_type(AnalysisType::STATICS);
        if(modifier == nullptr) modifier = make_shared<Integrator>();
        modifier->set_domain(database);
        break;
    case CT_DYNAMIC:
        if(modifier == nullptr) {
            suanpan_error("initialize() needs a valid Integrator.\n");
            return -1;
        }
        modifier->set_domain(database);
        factory->set_analysis_type(AnalysisType::DYNAMICS);
        break;
    case CT_FREQUENCE:
        factory->set_analysis_type(AnalysisType::EIGEN);
        break;
    default:
        suanpan_error("initialize() needs a valid Step.\n");
        return -1;
    }

    factory->initialize();

    tester->set_domain(database);
    solver->set_converger(tester);
    solver->set_integrator(modifier);

    return 0;
}

int Step::analyze() { return -1; }

void Step::set_workshop(const shared_ptr<Workshop>& F) {
    factory = F;
    updated = false;
}

const shared_ptr<Workshop>& Step::get_workshop() const { return factory; }

void Step::set_domain(const shared_ptr<Domain>& D) {
    database = D;
    updated = false;
}

const shared_ptr<Domain>& Step::get_domain() const { return database; }

void Step::set_solver(const shared_ptr<Solver>& S) {
    solver = S;
    updated = false;
}

const shared_ptr<Solver>& Step::get_solver() const { return solver; }

void Step::set_converger(const shared_ptr<Converger>& C) {
    tester = C;
    updated = false;
}

const shared_ptr<Converger>& Step::get_converger() const { return tester; }

void Step::set_integrator(const shared_ptr<Integrator>& G) {
    modifier = G;
    updated = false;
}

const shared_ptr<Integrator>& Step::get_integrator() const { return modifier; }

void Step::set_time_perid(const double& T) { time_period = T; }

const double& Step::get_time_period() const { return time_period; }

void Step::set_ini_step_size(const double& T) { ini_step_size = T; }

void Step::set_min_step_size(const double& T) { min_step_size = T; }

void Step::set_max_step_size(const double& T) { max_step_size = T; }

void Step::set_max_iteration(const unsigned& M) { max_increment = M; }

const double& Step::get_ini_step_size() const { return ini_step_size; }

const double& Step::get_min_step_size() const { return min_step_size; }

const double& Step::get_max_step_size() const { return max_step_size; }

const unsigned& Step::get_max_iteration() const { return max_increment; }

const bool& Step::is_fixed_step_size() const { return fixed_step_size; }

void Step::set_fixed_step_size(const bool& B) {
    if(fixed_step_size != B) {
        fixed_step_size = B;
        updated = false;
    }
}

const bool& Step::is_symm() const { return symm_mat; }

const bool& Step::is_band() const { return band_mat; }

void Step::set_symm(const bool& B) {
    if(symm_mat != B) {
        symm_mat = B;
        updated = false;
    }
}

void Step::set_band(const bool& B) {
    if(band_mat != B) {
        band_mat = B;
        updated = false;
    }
}
