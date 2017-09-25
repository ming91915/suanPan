////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2017 Theodore Chang
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////

#include "Step.h"
#include "Solver/Ramm.h"
#include <Converger/RelIncreDisp.h>
#include <Domain/Domain.h>
#include <Domain/Factory.hpp>
#include <Solver/Integrator/Newmark.h>
#include <Solver/Newton.h>

Step::Step(const unsigned& T, const unsigned& CT, const double& P)
    : Tag(T, CT)
    , time_period(P) {
    suanpan_debug("Step %u ctor() called.\n", T);
}

Step::~Step() { suanpan_debug("Step %u dtor() called.\n", get_tag()); }

const bool& Step::is_updated() const { return updated; }

int Step::initialize() {
    if(database == nullptr) {
        suanpan_error("initialize() needs a valid domain.\n");
        return -1;
    }

    switch(get_class_tag()) {
    case CT_ARCLENGTH:
        if(solver == nullptr) solver = make_shared<Ramm>();
        break;
    case CT_STATIC:
    case CT_DYNAMIC:
    case CT_FREQUENCE:
        if(solver == nullptr) solver = make_shared<Newton>();
        break;
    default:
        suanpan_error("initialize() needs a valid step.\n");
        return -1;
    }

    if(factory == nullptr) factory = database->get_factory().lock();
    if(factory == nullptr) factory = make_shared<Factory<double>>();
    if(tester == nullptr) tester = make_shared<RelIncreDisp>(1E-6, 10, false);

    if(symm_mat && band_mat)
        factory->set_storage_scheme(StorageScheme::BANDSYMM);
    else if(!symm_mat && band_mat)
        factory->set_storage_scheme(StorageScheme::BAND);
    else if(symm_mat && !band_mat)
        factory->set_storage_scheme(StorageScheme::SYMMPACK);
    else if(!symm_mat && !band_mat)
        factory->set_storage_scheme(StorageScheme::FULL);

    database->set_factory(factory);

    database->initialize();

    switch(get_class_tag()) {
    case CT_STATIC:
    case CT_ARCLENGTH:
        factory->set_analysis_type(AnalysisType::STATICS);
        if(modifier == nullptr) modifier = make_shared<Integrator>();
        modifier->set_domain(database);
        break;
    case CT_DYNAMIC:
        if(modifier == nullptr) {
            suanpan_error("initialize() needs a valid integrator.\n");
            return -1;
        }
        modifier->set_domain(database);
        factory->set_analysis_type(AnalysisType::DYNAMICS);
        if(modifier == nullptr) modifier = make_shared<Newmark>();
        modifier->set_domain(database);
        break;
    case CT_FREQUENCE:
        factory->set_analysis_type(AnalysisType::EIGEN);
        break;
    default:
        suanpan_error("initialize() needs a valid step.\n");
        return -1;
    }

    factory->initialize();

    tester->set_domain(database);
    solver->set_converger(tester);
    solver->set_integrator(modifier);

    return 0;
}

int Step::analyze() { return -1; }

void Step::set_factory(const shared_ptr<Factory<double>>& F) {
    if(factory != F) {
        factory = F;
        updated = false;
    }
}

const shared_ptr<Factory<double>>& Step::get_factory() const { return factory; }

void Step::set_domain(const shared_ptr<Domain>& D) {
    if(database != D) {
        database = D;
        updated = false;
    }
}

const shared_ptr<Domain>& Step::get_domain() const { return database; }

void Step::set_solver(const shared_ptr<Solver>& S) {
    if(solver != S) {
        solver = S;
        updated = false;
    }
}

const shared_ptr<Solver>& Step::get_solver() const { return solver; }

void Step::set_converger(const shared_ptr<Converger>& C) {
    if(tester != C) {
        tester = C;
        updated = false;
    }
}

const shared_ptr<Converger>& Step::get_converger() const { return tester; }

void Step::set_integrator(const shared_ptr<Integrator>& G) {
    if(modifier != G) {
        modifier = G;
        updated = false;
    }
}

const shared_ptr<Integrator>& Step::get_integrator() const { return modifier; }

void Step::set_time_perid(const double& T) {
    if(time_period != T) {
        time_period = T;
        updated = false;
        const auto tmp_iteration = static_cast<unsigned>(floor(time_period / ini_step_size)) + 1;
        if(tmp_iteration > max_substep && max_substep != 0)
            if(tmp_iteration > std::numeric_limits<unsigned>::max()) {
                suanpan_warning("set_ini_step_size() exceeds limits.\n");
                set_max_substep(std::numeric_limits<unsigned>::max());
            } else
                set_max_substep(tmp_iteration);
    }
}

const double& Step::get_time_period() const { return time_period; }

void Step::set_ini_step_size(const double& T) {
    if(ini_step_size != T) {
        ini_step_size = T;
        updated = false;
        const auto tmp_iteration = static_cast<unsigned>(floor(time_period / ini_step_size)) + 1;
        if(tmp_iteration > max_substep && max_substep != 0)
            if(tmp_iteration > std::numeric_limits<unsigned>::max()) {
                suanpan_warning("set_ini_step_size() exceeds limits.\n");
                set_max_substep(std::numeric_limits<unsigned>::max());
            } else
                set_max_substep(tmp_iteration);
    }
}

void Step::set_min_step_size(const double& T) {
    if(min_step_size != T) {
        min_step_size = T;
        updated = false;
    }
}

void Step::set_max_step_size(const double& T) {
    if(max_step_size != T) {
        max_step_size = T;
        updated = false;
    }
}

void Step::set_max_substep(const unsigned& M) {
    if(max_substep != M) {
        max_substep = M;
        updated = false;
    }
}

const double& Step::get_ini_step_size() const { return ini_step_size; }

const double& Step::get_min_step_size() const { return min_step_size; }

const double& Step::get_max_step_size() const { return max_step_size; }

const unsigned& Step::get_max_substep() const { return max_substep; }

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
