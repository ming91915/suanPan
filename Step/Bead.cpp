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

#include "Bead.h"
#include <Converger/Converger.h>
#include <Domain/Domain.h>
#include <Solver/Solver.h>
#include <Step/Step.h>

Bead::Bead()
    : step_pool(make_shared<map<unsigned, shared_ptr<Step>>>()) {
    insert(make_shared<Domain>(1));
}

bool Bead::insert(const shared_ptr<Domain>& D) { return domain_pool.insert(D); }

bool Bead::insert(const shared_ptr<Solver>& S) { return solver_pool.insert(S); }

bool Bead::insert(const shared_ptr<Converger>& C) { return converger_pool.insert(C); }

bool Bead::insert(const shared_ptr<Step>& S) const {
    auto F = step_pool->insert({ S->get_tag(), S });
    return F.second;
}

const shared_ptr<Domain>& Bead::get_domain(const unsigned& T) const { return domain_pool.at(T); }

const shared_ptr<Solver>& Bead::get_solver(const unsigned& T) const { return solver_pool.at(T); }

const shared_ptr<Converger>& Bead::get_converger(const unsigned& T) const { return converger_pool.at(T); }

const shared_ptr<Step>& Bead::get_step(const unsigned& T) const { return step_pool->at(T); }

const shared_ptr<Domain>& Bead::get_current_domain() const { return domain_pool.at(current_domain_tag); }

const shared_ptr<Solver>& Bead::get_current_solver() const { return solver_pool.at(current_solver_tag); }

const shared_ptr<Converger>& Bead::get_current_converger() const { return converger_pool.at(current_converger_tag); }

const shared_ptr<Step>& Bead::get_current_step() const { return step_pool->at(current_step_tag); }

const unsigned& Bead::get_current_domain_tag() const { return current_domain_tag; }

const unsigned& Bead::get_current_solver_tag() const { return current_solver_tag; }

const unsigned& Bead::get_current_converger_tag() const { return current_converger_tag; }

const unsigned& Bead::get_current_step_tag() const { return current_step_tag; }

void Bead::erase_domain(const unsigned& T) {
    if(!domain_pool.find(T))
        suanpan_info("erase_domain() cannot find Domain %u, nothing changed.\n", T);
    else {
        char flag;
        suanpan_info("erase_domain() is about to delete Domain %u, are you sure? [Y/n]", T);
        std::cin.get(flag);
        if(flag == 'y' || flag == 'Y' || flag == '\n') {
            domain_pool.erase(T);
            if(domain_pool.size() != 0) {
                set_current_domain(domain_pool.cbegin()->first);
                suanpan_info("erase_domain() switches to Domain %u.\n", current_domain_tag);
            } else {
                insert(make_shared<Domain>(1));
                set_current_domain(1);
                suanpan_info("erase_domain() removes the last domain and switches to default Domain 1.\n");
            }
        }
    }
}

void Bead::erase_solver(const unsigned& T) { solver_pool.erase(T); }

void Bead::erase_converger(const unsigned& T) { converger_pool.erase(T); }

void Bead::erase_step(const unsigned& T) {
    if(step_pool->find(T) == step_pool->end())
        suanpan_info("erase_step() cannot find Step %u, nothing changed.\n", T);
    else {
        char flag;
        suanpan_info("erase_step() is about to delete Step %u, are you sure? [Y/N]", T);
        std::cin.get(flag);
        if(flag == 'y' || flag == 'Y' || flag == '\n') {
            step_pool->erase(T);
            if(step_pool->size() != 0) {
                set_current_step(step_pool->rbegin()->first);
                suanpan_info("erase_step() switches to Step %u.\n", current_step_tag);
            } else {
                set_current_step(0);
                suanpan_info("erase_step() removes the last step and switches to default Step 0.\n");
            }
        }
    }
}

void Bead::disable_domain(const unsigned& T) { domain_pool.disable(T); }

void Bead::disable_solver(const unsigned& T) { solver_pool.disable(T); }

void Bead::disable_converger(const unsigned& T) { converger_pool.disable(T); }

void Bead::disable_step(const unsigned& T) const {
    if(step_pool->find(T) != step_pool->end()) {
        step_pool->at(T)->disable();
        suanpan_debug("disable_step() disables Step %u.\n", T);
    } else
        suanpan_info("disable_step() cannot find Step %u.\n", T);
}

void Bead::enable_domain(const unsigned& T) { domain_pool.enable(T); }

void Bead::enable_solver(const unsigned& T) { solver_pool.enable(T); }

void Bead::enable_converger(const unsigned& T) { converger_pool.enable(T); }

void Bead::enable_step(const unsigned& T) const {
    if(step_pool->find(T) != step_pool->end()) {
        step_pool->at(T)->enable();
        suanpan_debug("enable_step() enables Step %u.\n", T);
    } else
        suanpan_info("enable_step() cannot find Step %u.\n", T);
}

void Bead::set_current_domain(const unsigned& T) { current_domain_tag = T; }

void Bead::set_current_solver(const unsigned& T) { current_solver_tag = T; }

void Bead::set_current_converger(const unsigned& T) { current_converger_tag = T; }

void Bead::set_current_step(const unsigned& T) { current_step_tag = T; }

int Bead::analyze() {
    for(const auto& J : domain_pool) J.second->set_step_anchor(step_pool);

    auto code = 0;

    for(const auto& I : *step_pool)
        if(I.second->is_active() && I.second->initialize() == 0) code += I.second->analyze();

    return code;
}

shared_ptr<Domain>& get_domain(const shared_ptr<Bead>& B, const unsigned& T) { return B->domain_pool[T]; }

shared_ptr<Solver>& get_solver(const shared_ptr<Bead>& B, const unsigned& T) { return B->solver_pool[T]; }

shared_ptr<Converger>& get_converger(const shared_ptr<Bead>& B, const unsigned& T) { return B->converger_pool[T]; }

shared_ptr<Step>& get_step(const shared_ptr<Bead>& B, const unsigned& T) { return (*B->step_pool)[T]; }

shared_ptr<Domain>& get_current_domain(const shared_ptr<Bead>& B) { return B->domain_pool[B->current_domain_tag]; }

shared_ptr<Solver>& get_current_solver(const shared_ptr<Bead>& B) { return B->solver_pool[B->current_solver_tag]; }

shared_ptr<Converger>& get_current_converger(const shared_ptr<Bead>& B) { return B->converger_pool[B->current_converger_tag]; }

shared_ptr<Step>& get_current_step(const shared_ptr<Bead>& B) { return (*B->step_pool)[B->current_step_tag]; }
