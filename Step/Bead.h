/**
* @class Bead
* @brief A Bead class is a top level container.
* @author T
* @date 23/08/2017
* @version 0.2.1
* @file Bead.h
*/

#ifndef BEAD_H
#define BEAD_H

#include <suanPan.h>
#include <unordered_map>

class Domain;
class Convergence;
class Step;

using std::unordered_map;

class Bead
{
    unsigned current_domain = 1;
    unsigned current_step = 0;

    unordered_map<unsigned, shared_ptr<Domain>> domain_pool;
    unordered_map<unsigned, shared_ptr<Step>> step_pool;
    unordered_map<unsigned, shared_ptr<Convergence>> converger_pool;

public:
    Bead();

    bool insert(const shared_ptr<Domain>&);
    bool insert(const shared_ptr<Step>&);
    bool insert(const shared_ptr<Convergence>&);

    const shared_ptr<Domain>& getDomain(const unsigned&) const;
    const shared_ptr<Step>& getStep(const unsigned&) const;
    const shared_ptr<Convergence>& getConvergence(const unsigned&) const;

    friend shared_ptr<Domain>& getDomain(const shared_ptr<Bead>&, const unsigned&);
    friend shared_ptr<Step>& getStep(const shared_ptr<Bead>&, const unsigned&);
    friend shared_ptr<Convergence>& getConvergence(const shared_ptr<Bead>&,
        const unsigned&);

    const shared_ptr<Domain>& getCurrentDomain() const;
    const shared_ptr<Step>& getCurrentStep() const;

    friend shared_ptr<Domain>& getCurrentDomain(const shared_ptr<Bead>&);
    friend shared_ptr<Step>& getCurrentStep(const shared_ptr<Bead>&);

    void erase_domain(const unsigned&);
    void erase_step(const unsigned&);
    void erase_convergence(const unsigned&);

    void disable_domain(const unsigned&);
    void disable_step(const unsigned&);
    void disable_convergence(const unsigned&);

    void setCurrentDomain(const unsigned&);
    void setCurrentStep(const unsigned&);

    void analyze();
};

shared_ptr<Domain>& getDomain(const shared_ptr<Bead>&, const unsigned&);
shared_ptr<Step>& getStep(const shared_ptr<Bead>&, const unsigned&);
shared_ptr<Convergence>& getConvergence(const shared_ptr<Bead>&, const unsigned&);

shared_ptr<Domain>& getCurrentDomain(const shared_ptr<Bead>&);
shared_ptr<Step>& getCurrentStep(const shared_ptr<Bead>&);

#endif
