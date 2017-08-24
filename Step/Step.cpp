#include "Step.h"
#include <Convergence/Convergence.h>
#include <Domain/Domain.h>
#include <Domain/Workroom.h>
#include <Solver/Solver.h>

Step::Step(const unsigned& T,
    const unsigned& CT,
    const shared_ptr<Domain>& D,
    const shared_ptr<Solver>& S,
    const shared_ptr<Convergence>& C,
    const double& P)
    : Tag(T, CT)
    , time_period(P)
    , database(D)
    , solver(S)
    , converger(C)
{
    enable_band();
    enable_symm();
}

int Step::initialize()
{
    if(factory == nullptr) factory = make_shared<Workroom>();

    if(database == nullptr) {
        suanpan_error("initialize() needs a valid Domain.\n");
        return -1;
    }

    if(solver == nullptr) {
        suanpan_error("initialize() needs a valid Solver.\n");
        return -1;
    }

    if(converger == nullptr) {
        suanpan_error("initialize() needs a valid Converger.\n");
        return -1;
    }

    database->set_workroom(factory);
    database->initialize();

    factory->set_analysis_type(SUANPAN_STATICS);

    if(band_mat)
        factory->enable_band();
    else
        factory->disable_band();

    if(symm_mat)
        factory->enable_symm();
    else
        factory->disable_symm();

    factory->initialize();

    solver->set_domain(database);
    solver->initialize();

    converger->set_domain(database);
    converger->initialize();

    return 0;
}

int Step::analyze() { return -1; }

void Step::set_workroom(const shared_ptr<Workroom>& F) { factory = F; }

const shared_ptr<Workroom>& Step::get_workroom() const { return factory; }

void Step::set_domain(const shared_ptr<Domain>& D) { database = D; }

const shared_ptr<Domain>& Step::get_domain() const { return database; }

void Step::set_solver(const shared_ptr<Solver>& S) { solver = S; }

const shared_ptr<Solver>& Step::get_solver() const { return solver; }

void Step::set_convergence(const shared_ptr<Convergence>& C) { converger = C; }

const shared_ptr<Convergence>& Step::get_convergence() const { return converger; }

void Step::set_time_perid(const double& T) { time_period = T; }

const double& Step::get_time_period() const { return time_period; }

void Step::enable_band() { band_mat = true; }

void Step::disable_band() { band_mat = false; }

void Step::enable_symm() { symm_mat = true; }

void Step::disable_symm() { symm_mat = false; }
