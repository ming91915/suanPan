/*******************************************************************************
 * Copyright (C) 2017 Theodore Chang
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/
/**
 * @class Bead
 * @brief A Bead class is a top level container.
 * @author T
 * @date 27/08/2017
 * @version 0.2.1
 * @file Bead.h
 */

#ifndef BEAD_H
#define BEAD_H

#include <Domain/Storage.hpp>
#include <map>
#include <suanPan.h>

using std::map;
using std::unordered_map;

class Domain;
class Solver;
class Converger;
class Step;

class Bead {
    unsigned current_domain = 1;
    unsigned current_solver = 0;
    unsigned current_converger = 0;
    unsigned current_step = 0;

    DomainStorage domain_pool;
    SolverStorage solver_pool;
    ConvergerStorage converger_pool;

    shared_ptr<map<unsigned, shared_ptr<Step>>> step_pool;

public:
    Bead();

    bool insert(const shared_ptr<Domain>&);
    bool insert(const shared_ptr<Solver>&);
    bool insert(const shared_ptr<Converger>&);
    bool insert(const shared_ptr<Step>&) const;

    const shared_ptr<Domain>& get_domain(const unsigned&) const;
    const shared_ptr<Solver>& get_solver(const unsigned&) const;
    const shared_ptr<Converger>& get_converger(const unsigned&) const;
    const shared_ptr<Step>& get_step(const unsigned&) const;

    friend shared_ptr<Domain>& get_domain(const shared_ptr<Bead>&, const unsigned&);
    friend shared_ptr<Solver>& get_solver(const shared_ptr<Bead>&, const unsigned&);
    friend shared_ptr<Converger>& get_converger(const shared_ptr<Bead>&, const unsigned&);
    friend shared_ptr<Step>& get_step(const shared_ptr<Bead>&, const unsigned&);

    const shared_ptr<Domain>& get_current_domain() const;
    const shared_ptr<Solver>& get_current_solver() const;
    const shared_ptr<Converger>& get_current_converger() const;
    const shared_ptr<Step>& get_current_step() const;

    friend shared_ptr<Domain>& get_current_domain(const shared_ptr<Bead>&);
    friend shared_ptr<Solver>& get_current_solver(const shared_ptr<Bead>&);
    friend shared_ptr<Converger>& get_current_converger(const shared_ptr<Bead>&);
    friend shared_ptr<Step>& get_current_step(const shared_ptr<Bead>&);

    void erase_domain(const unsigned&);
    void erase_solver(const unsigned&);
    void erase_converger(const unsigned&);
    void erase_step(const unsigned&);

    void disable_domain(const unsigned&);
    void disable_solver(const unsigned&);
    void disable_converger(const unsigned&);
    void disable_step(const unsigned&) const;

    void enable_domain(const unsigned&);
    void enable_solver(const unsigned&);
    void enable_converger(const unsigned&);
    void enable_step(const unsigned&) const;

    void set_current_domain(const unsigned&);
    void set_current_solver(const unsigned&);
    void set_current_converger(const unsigned&);
    void set_current_step(const unsigned&);

    int analyze();
};

#endif
