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

    const shared_ptr<Domain>& get_domain(const unsigned&) const;
    const shared_ptr<Step>& get_step(const unsigned&) const;
    const shared_ptr<Convergence>& get_convergence(const unsigned&) const;

    friend shared_ptr<Domain>& get_domain(const shared_ptr<Bead>&, const unsigned&);
    friend shared_ptr<Step>& get_step(const shared_ptr<Bead>&, const unsigned&);
    friend shared_ptr<Convergence>& get_convergence(const shared_ptr<Bead>&,
        const unsigned&);

    const shared_ptr<Domain>& get_current_domain() const;
    const shared_ptr<Step>& get_current_step() const;

    friend shared_ptr<Domain>& get_current_domain(const shared_ptr<Bead>&);
    friend shared_ptr<Step>& get_current_step(const shared_ptr<Bead>&);

    void erase_domain(const unsigned&);
    void erase_step(const unsigned&);
    void erase_convergence(const unsigned&);

    void disable_domain(const unsigned&);
    void disable_step(const unsigned&);
    void disable_convergence(const unsigned&);

    void set_current_domain(const unsigned&);
    void set_current_step(const unsigned&);

    int analyze();
};

#endif
