#include "Bead.h"
#include "Convergence/Convergence.h"
#include <Domain/Domain.h>
#include <Recorder/Recorder.h>
#include <Step/Step.h>

Bead::Bead()
{
    // BY DEFAULT USE DOMAIN ONE
    domains.insert({ 1, make_shared<Domain>(1) });
}

bool Bead::insert(const shared_ptr<Domain>& D)
{
    auto F = domains.insert({ D->getTag(), D });
    return F.second;
}

bool Bead::insert(const shared_ptr<Convergence>& C)
{
    auto F = convergers.insert({ C->getTag(), C });
    return F.second;
}

bool Bead::insert(const shared_ptr<Step>& S)
{
    auto F = steps.insert({ S->getTag(), S });
    return F.second;
}

bool Bead::insert(const shared_ptr<Recorder>& R)
{
    auto F = loggers.insert({ R->getTag(), R });
    return F.second;
}

const shared_ptr<Domain>& Bead::getDomain(const unsigned& T) const
{
    return domains.at(T);
}

shared_ptr<Domain>& Bead::getDomain(const unsigned& T) { return domains[T]; }

const shared_ptr<Convergence>& Bead::getConvergence(const unsigned& T) const
{
    return convergers.at(T);
}

shared_ptr<Convergence>& Bead::getConvergence(const unsigned& T) { return convergers[T]; }

const shared_ptr<Step>& Bead::getStep(const unsigned& T) const { return steps.at(T); }

shared_ptr<Step>& Bead::getStep(const unsigned& T) { return steps[T]; }

const shared_ptr<Recorder>& Bead::getRecorder(const unsigned& T) const
{
    return loggers.at(T);
}

shared_ptr<Recorder>& Bead::getRecorder(const unsigned& T) { return loggers[T]; }

const shared_ptr<Domain>& Bead::getCurrentDomain() const
{
    return domains.at(current_domain);
}

shared_ptr<Domain>& Bead::getCurrentDomain() { return domains[current_domain]; }

const shared_ptr<Step>& Bead::getCurrentStep() const { return steps.at(current_step); }

shared_ptr<Step>& Bead::getCurrentStep() { return steps[current_step]; }

int Bead::erase_domain(const unsigned& T)
{
    auto& tmp_domain = getDomain(T);
    if(tmp_domain == nullptr)
        suanpan_info("erase_domain() cannot find the Domain %u, nothing changed.\n", T);
    else {
        auto flag = 'Y';
        suanpan_info(
            "erase_domain() is about to delete Domain %u, are you sure? [Y/N]", T);
        std::cin.get(flag);
        if(flag == 'Y' || flag == '\n') domains.erase(T);
    }
    return 0;
}

void Bead::setCurrentDomain(const unsigned& T) const
{
    auto tmp_ptr = &current_domain;
    auto domain_ptr = const_cast<unsigned*>(tmp_ptr);
    *domain_ptr = T;
}

void Bead::setCurrentStep(const unsigned& T) const
{
    auto tmp_ptr = &current_step;
    auto step_ptr = const_cast<unsigned*>(tmp_ptr);
    *step_ptr = T;
}

void Bead::analyze()
{
    for(const auto& I : steps) I.second->analyze();
}
