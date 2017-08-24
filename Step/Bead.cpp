#include "Bead.h"
#include <Convergence/Convergence.h>
#include <Domain/Domain.h>
#include <Solver/Solver.h>
#include <Step/Step.h>

Bead::Bead()
{
    // BY DEFAULT USE DOMAIN ONE
    insert(make_shared<Domain>(1));
}

bool Bead::insert(const shared_ptr<Domain>& D)
{
    auto F = domain_pool.insert({ D->get_tag(), D });
    return F.second;
}

bool Bead::insert(const shared_ptr<Solver>& S)
{
    auto F = solver_pool.insert({ S->get_tag(), S });
    return F.second;
}

bool Bead::insert(const shared_ptr<Convergence>& C)
{
    auto F = converger_pool.insert({ C->get_tag(), C });
    return F.second;
}

bool Bead::insert(const shared_ptr<Step>& S)
{
    auto F = step_pool.insert({ S->get_tag(), S });
    return F.second;
}

const shared_ptr<Domain>& Bead::get_domain(const unsigned& T) const
{
    return domain_pool.at(T);
}

const shared_ptr<Solver>& Bead::get_solver(const unsigned& T) const
{
    return solver_pool.at(T);
}

const shared_ptr<Convergence>& Bead::get_convergence(const unsigned& T) const
{
    return converger_pool.at(T);
}

const shared_ptr<Step>& Bead::get_step(const unsigned& T) const
{
    return step_pool.at(T);
}

const shared_ptr<Domain>& Bead::get_current_domain() const
{
    return domain_pool.at(current_domain);
}

const shared_ptr<Step>& Bead::get_current_step() const
{
    return step_pool.at(current_step);
}

void Bead::erase_domain(const unsigned& T)
{
    if(domain_pool.find(T) == domain_pool.end())
        suanpan_info("erase_domain() cannot find Domain %u, nothing changed.\n", T);
    else {
        char flag;
        suanpan_info(
            "erase_domain() is about to delete Domain %u, are you sure? [Y/N]", T);
        std::cin.get(flag);
        if(flag == 'y' || flag == 'Y' || flag == '\n') {
            domain_pool.erase(T);
            if(domain_pool.size() != 0) {
                set_current_domain(domain_pool.cbegin()->first);
                suanpan_info("erase_domain() switches to Domain %u.\n", current_domain);
            } else {
                insert(make_shared<Domain>(1));
                set_current_domain(1);
                suanpan_info("erase_domain() removes the last domain and switches to "
                             "default Domain 1.\n");
            }
        }
    }
}

void Bead::erase_solver(const unsigned& T) { solver_pool.erase(T); }

void Bead::erase_convergence(const unsigned& T) { converger_pool.erase(T); }

void Bead::erase_step(const unsigned& T)
{
    if(step_pool.find(T) == step_pool.end())
        suanpan_info("erase_step() cannot find Step %u, nothing changed.\n", T);
    else {
        char flag;
        suanpan_info("erase_step() is about to delete Step %u, are you sure? [Y/N]", T);
        std::cin.get(flag);
        if(flag == 'y' || flag == 'Y' || flag == '\n') {
            step_pool.erase(T);
            if(step_pool.size() != 0) {
                set_current_step(step_pool.rbegin()->first);
                suanpan_info("erase_step() switches to Step %u.\n", current_step);
            } else {
                set_current_step(0);
                suanpan_info("erase_step() removes the last step and switches to "
                             "default Step 0.\n");
            }
        }
    }
}

void Bead::disable_domain(const unsigned& T)
{
    if(domain_pool.find(T) != domain_pool.end()) {
        domain_pool.at(T)->disable();
        suanpan_debug("disable_domain() disables Domain %u.\n", T);
    } else
        suanpan_info("disable_domain() cannot find Domain %u.\n", T);
}

void Bead::disable_solver(const unsigned& T)
{
    if(solver_pool.find(T) != solver_pool.end()) {
        solver_pool.at(T)->disable();
        suanpan_debug("disable_step() disables Solver %u.\n", T);
    } else
        suanpan_info("disable_step() cannot find Solver %u.\n", T);
}

void Bead::disable_convergence(const unsigned& T)
{
    if(converger_pool.find(T) != converger_pool.end()) {
        converger_pool.at(T)->disable();
        suanpan_debug("disable_convergence() disables Convergence %u.\n", T);
    } else
        suanpan_info("disable_convergence() cannot find Convergence %u.\n", T);
}

void Bead::disable_step(const unsigned& T)
{
    if(step_pool.find(T) != step_pool.end()) {
        step_pool.at(T)->disable();
        suanpan_debug("disable_step() disables Step %u.\n", T);
    } else
        suanpan_info("disable_step() cannot find Step %u.\n", T);
}

void Bead::set_current_domain(const unsigned& T) { current_domain = T; }

void Bead::set_current_step(const unsigned& T) { current_step = T; }

int Bead::analyze()
{
    auto code = 0;
    for(const auto& I : step_pool)
        if(I.second->is_active()) {
            I.second->initialize();
            code += I.second->analyze();
        }
    return code;
}

shared_ptr<Domain>& get_domain(const shared_ptr<Bead>& B, const unsigned& T)
{
    return B->domain_pool[T];
}

shared_ptr<Solver>& get_solver(const shared_ptr<Bead>& B, const unsigned& T)
{
    return B->solver_pool[T];
}

shared_ptr<Convergence>& get_convergence(const shared_ptr<Bead>& B, const unsigned& T)
{
    return B->converger_pool[T];
}

shared_ptr<Step>& get_step(const shared_ptr<Bead>& B, const unsigned& T)
{
    return B->step_pool[T];
}

shared_ptr<Domain>& get_current_domain(const shared_ptr<Bead>& B)
{
    return B->domain_pool[B->current_domain];
}

shared_ptr<Step>& get_current_step(const shared_ptr<Bead>& B)
{
    return B->step_pool[B->current_step];
}