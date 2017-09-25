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

#include "Integrator.h"
#include <Domain/DomainBase.h>
#include <Domain/Factory.hpp>

Integrator::Integrator(const unsigned& T, const unsigned& CT, const shared_ptr<DomainBase>& D)
    : Tag(T, CT)
    , database(D) {
    suanpan_debug("Integrator %u ctor() called.\n", T);
}

Integrator::~Integrator() { suanpan_debug("Integrator %u dtor() called.\n", get_tag()); }

void Integrator::set_domain(const shared_ptr<DomainBase>& D) {
    if(database != D) database = D;
}

const shared_ptr<DomainBase>& Integrator::get_domain() const { return database; }

int Integrator::initialize() {
    if(database == nullptr) {
        suanpan_error("initialize() needs a valid domain.\n");
        return -1;
    }

    return 0;
}

int Integrator::process_load() const { return database->process_load(); }

int Integrator::process_constraint() const { return database->process_constraint(); }

int Integrator::process_criterion() const { return database->process_criterion(); }

void Integrator::record() const { database->record(); }

void Integrator::assemble_resistance() { database->assemble_resistance(); }

void Integrator::assemble_stiffness() { database->assemble_stiffness(); }

void Integrator::update_trial_time(const double& T) const { database->get_factory().lock()->update_trial_time(T); }

void Integrator::update_incre_time(const double& T) const { database->get_factory().lock()->update_incre_time(T); }

void Integrator::update_trial_status() { database->update_trial_status(); }

void Integrator::update_incre_status() { database->update_incre_status(); }

void Integrator::erase_machine_error() const { database->erase_machine_error(); }

void Integrator::commit_status() const { database->commit_status(); }

void Integrator::clear_status() const { database->clear_status(); }

void Integrator::reset_status() const { database->reset_status(); }
