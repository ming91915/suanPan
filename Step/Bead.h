/**
* @class Bead
* @brief A Bead class is a container.
* @author T
* @date 17/08/2017
* @version 0.2.0
* @file Bead.h
*/

#ifndef BEAD_H
#define BEAD_H

#include <suanPan.h>
#include <unordered_map>

class Domain;
class Convergence;
class Step;
class Recorder;

using std::unordered_map;

class Bead
{
    unsigned current_domain = 1;
    unsigned current_step = 0;

    unordered_map<unsigned, shared_ptr<Domain>> domain_pool;
    unordered_map<unsigned, shared_ptr<Convergence>> converger_pool;
    unordered_map<unsigned, shared_ptr<Step>> step_pool;
    unordered_map<unsigned, shared_ptr<Recorder>> recorder_pool;

public:
    Bead();

    bool insert(const shared_ptr<Domain>&);
    bool insert(const shared_ptr<Convergence>&);
    bool insert(const shared_ptr<Step>&);
    bool insert(const shared_ptr<Recorder>&);

    const shared_ptr<Domain>& getDomain(const unsigned&) const;
    const shared_ptr<Convergence>& getConvergence(const unsigned&) const;
    const shared_ptr<Step>& getStep(const unsigned&) const;
    const shared_ptr<Recorder>& getRecorder(const unsigned&) const;

    const shared_ptr<Domain>& getCurrentDomain() const;
    const shared_ptr<Step>& getCurrentStep() const;

    friend shared_ptr<Domain>& getDomain(const shared_ptr<Bead>&, const unsigned&);
    friend shared_ptr<Convergence>& getConvergence(const shared_ptr<Bead>&,
        const unsigned&);
    friend shared_ptr<Step>& getStep(const shared_ptr<Bead>&, const unsigned&);
    friend shared_ptr<Recorder>& getRecorder(const shared_ptr<Bead>&, const unsigned&);
    friend shared_ptr<Domain>& getCurrentDomain(const shared_ptr<Bead>&);
    friend shared_ptr<Step>& getCurrentStep(const shared_ptr<Bead>&);

    int erase_domain(const unsigned&);

    void setCurrentDomain(const unsigned&);
    void setCurrentStep(const unsigned&);

    void analyze();
};

shared_ptr<Domain>& getDomain(const shared_ptr<Bead>&, const unsigned&);
shared_ptr<Convergence>& getConvergence(const shared_ptr<Bead>&, const unsigned&);
shared_ptr<Step>& getStep(const shared_ptr<Bead>&, const unsigned&);
shared_ptr<Recorder>& getRecorder(const shared_ptr<Bead>&, const unsigned&);
shared_ptr<Domain>& getCurrentDomain(const shared_ptr<Bead>&);
shared_ptr<Step>& getCurrentStep(const shared_ptr<Bead>&);

#endif
