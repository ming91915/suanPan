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

#include <map>
#include <suanPan.h>
#include <unordered_map>

class Domain;
class Solver;
class Convergence;
class Step;

using std::unordered_map;
using std::map;

class Bead
{
    unsigned current_domain = 1;
    unsigned current_solver = 0;
    unsigned current_converger = 0;
    unsigned current_step = 0;

    unordered_map<unsigned, shared_ptr<Domain>> domain_pool;
    unordered_map<unsigned, shared_ptr<Solver>> solver_pool;
    unordered_map<unsigned, shared_ptr<Convergence>> converger_pool;
    map<unsigned, shared_ptr<Step>> step_pool;

public:
    Bead();

    bool insert(const shared_ptr<Domain>&);
    bool insert(const shared_ptr<Solver>&);
    bool insert(const shared_ptr<Convergence>&);
    bool insert(const shared_ptr<Step>&);

    const shared_ptr<Domain>& get_domain(const unsigned&) const;
    const shared_ptr<Solver>& get_solver(const unsigned&) const;
    const shared_ptr<Convergence>& get_convergence(const unsigned&) const;
    const shared_ptr<Step>& get_step(const unsigned&) const;

    friend shared_ptr<Domain>& get_domain(const shared_ptr<Bead>&, const unsigned&);
    friend shared_ptr<Solver>& get_solver(const shared_ptr<Bead>&, const unsigned&);
    friend shared_ptr<Convergence>& get_convergence(const shared_ptr<Bead>&,
        const unsigned&);
    friend shared_ptr<Step>& get_step(const shared_ptr<Bead>&, const unsigned&);

    const shared_ptr<Domain>& get_current_domain() const;
    const shared_ptr<Solver>& get_current_solver() const;
    const shared_ptr<Convergence>& get_current_convergence() const;
    const shared_ptr<Step>& get_current_step() const;

    friend shared_ptr<Domain>& get_current_domain(const shared_ptr<Bead>&);
    friend shared_ptr<Solver>& get_current_solver(const shared_ptr<Bead>&);
    friend shared_ptr<Convergence>& get_current_convergence(const shared_ptr<Bead>&);
    friend shared_ptr<Step>& get_current_step(const shared_ptr<Bead>&);

    void erase_domain(const unsigned&);
    void erase_solver(const unsigned&);
    void erase_convergence(const unsigned&);
    void erase_step(const unsigned&);

    void disable_domain(const unsigned&);
    void disable_solver(const unsigned&);
    void disable_convergence(const unsigned&);
    void disable_step(const unsigned&);

    void enable_domain(const unsigned&);
    void enable_solver(const unsigned&);
    void enable_convergence(const unsigned&);
    void enable_step(const unsigned&);

    void set_current_domain(const unsigned&);
    void set_current_solver(const unsigned&);
    void set_current_convergence(const unsigned&);
    void set_current_step(const unsigned&);

    int analyze();
};

#endif
