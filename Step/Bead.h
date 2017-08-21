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
    unordered_map<unsigned, shared_ptr<Domain>> domains;
    unordered_map<unsigned, shared_ptr<Convergence>> convergers;
    unordered_map<unsigned, shared_ptr<Step>> steps;
    unordered_map<unsigned, shared_ptr<Recorder>> loggers;

public:
    Bead();

    const unsigned current_domain = 1;
    const unsigned current_step = 0;

    bool insert(const shared_ptr<Domain>&);
    bool insert(const shared_ptr<Convergence>&);
    bool insert(const shared_ptr<Step>&);
    bool insert(const shared_ptr<Recorder>&);

    const shared_ptr<Domain>& getDomain(const unsigned&) const;
    shared_ptr<Domain>& getDomain(const unsigned&);

    const shared_ptr<Convergence>& getConvergence(const unsigned&) const;
    shared_ptr<Convergence>& getConvergence(const unsigned&);

    const shared_ptr<Step>& getStep(const unsigned&) const;
    shared_ptr<Step>& getStep(const unsigned&);

    const shared_ptr<Recorder>& getRecorder(const unsigned&) const;
    shared_ptr<Recorder>& getRecorder(const unsigned&);

    const shared_ptr<Domain>& getCurrentDomain() const;
    shared_ptr<Domain>& getCurrentDomain();

    const shared_ptr<Step>& getCurrentStep() const;
    shared_ptr<Step>& getCurrentStep();

    int erase_domain(const unsigned&);

    void setCurrentDomain(const unsigned&) const;
    void setCurrentStep(const unsigned&) const;

    void analyze();
};

#endif
