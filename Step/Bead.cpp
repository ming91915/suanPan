#include "Bead.h"
#include "Convergence/Convergence.h"
#include <Domain/Domain.h>
#include <Recorder/Recorder.h>
#include <Step/Step.h>

Bead::Bead()
{
    // BY DEFAULT USE DOMAIN ONE
    insert(make_shared<Domain>(1));
}

bool Bead::insert(const shared_ptr<Domain>& D)
{
    auto F = domain_pool.insert({ D->getTag(), D });
    return F.second;
}

bool Bead::insert(const shared_ptr<Convergence>& C)
{
    auto F = converger_pool.insert({ C->getTag(), C });
    return F.second;
}

bool Bead::insert(const shared_ptr<Step>& S)
{
    auto F = step_pool.insert({ S->getTag(), S });
    return F.second;
}

bool Bead::insert(const shared_ptr<Recorder>& R)
{
    auto F = recorder_pool.insert({ R->getTag(), R });
    return F.second;
}

const shared_ptr<Domain>& Bead::getDomain(const unsigned& T) const
{
    return domain_pool.at(T);
}

const shared_ptr<Convergence>& Bead::getConvergence(const unsigned& T) const
{
    return converger_pool.at(T);
}

const shared_ptr<Step>& Bead::getStep(const unsigned& T) const { return step_pool.at(T); }

const shared_ptr<Recorder>& Bead::getRecorder(const unsigned& T) const
{
    return recorder_pool.at(T);
}

const shared_ptr<Domain>& Bead::getCurrentDomain() const
{
    return domain_pool.at(current_domain);
}

const shared_ptr<Step>& Bead::getCurrentStep() const
{
    return step_pool.at(current_step);
}

void Bead::erase_domain(const unsigned& T)
{
    if(domain_pool.find(T) == domain_pool.end())
        suanpan_info("erase_domain() cannot find Domain %u, nothing changed.\n", T);
    else {
        auto flag = 'Y';
        suanpan_info(
            "erase_domain() is about to delete Domain %u, are you sure? [Y/N]", T);
        std::cin.get(flag);
        if(flag == 'Y' || flag == '\n') {
            domain_pool.erase(T);
            if(domain_pool.size() != 0) {
                setCurrentDomain(domain_pool.cbegin()->first);
                suanpan_info("erase_domain() switches to Domain %u.\n", current_domain);
            } else {
                insert(make_shared<Domain>(1));
                setCurrentDomain(1);
                suanpan_info("erase_domain() removes the last domain and switches to "
                             "default Domain 1.\n");
            }
        }
    }
}

void Bead::erase_convergence(const unsigned& T) { converger_pool.erase(T); }

void Bead::erase_step(const unsigned& T) { step_pool.erase(T); }

void Bead::erase_recorder(const unsigned& T) { recorder_pool.erase(T); }

void Bead::disable_domain(const unsigned& T)
{
    if(domain_pool.find(T) != domain_pool.end()) {
        domain_pool.at(T)->disable();
        suanpan_debug("disable_domain() disables Domain %u.\n", T);
    } else
        suanpan_info("disable_domain() cannot find Domain %u.\n", T);
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

void Bead::disable_recorder(const unsigned& T)
{
    if(recorder_pool.find(T) != recorder_pool.end()) {
        recorder_pool.at(T)->disable();
        suanpan_debug("disable_recorder() disables Recorder %u.\n", T);
    } else
        suanpan_info("disable_recorder() cannot find Recorder %u.\n", T);
}

void Bead::setCurrentDomain(const unsigned& T) { current_domain = T; }

void Bead::setCurrentStep(const unsigned& T) { current_step = T; }

void Bead::analyze()
{
    for(const auto& I : step_pool)
        if(I.second->getStatus()) I.second->analyze();
}

shared_ptr<Domain>& getDomain(const shared_ptr<Bead>& B, const unsigned& T)
{
    return B->domain_pool[T];
}
shared_ptr<Convergence>& getConvergence(const shared_ptr<Bead>& B, const unsigned& T)
{
    return B->converger_pool[T];
}
shared_ptr<Step>& getStep(const shared_ptr<Bead>& B, const unsigned& T)
{
    return B->step_pool[T];
}
shared_ptr<Recorder>& getRecorder(const shared_ptr<Bead>& B, const unsigned& T)
{
    return B->recorder_pool[T];
}
shared_ptr<Domain>& getCurrentDomain(const shared_ptr<Bead>& B)
{
    return B->domain_pool[B->current_domain];
}
shared_ptr<Step>& getCurrentStep(const shared_ptr<Bead>& B)
{
    return B->step_pool[B->current_step];
}
