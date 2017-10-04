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
 * @class Integrator
 * @brief The Integrator class is basically a warpper of the DomainBase class with regard to some status changing methods.
 *
 * By default, the Step object calls DomainBase(Workshop) object to update displacement/resistance/stiffness independently. When it comes to dynamic analysis (time integration is invoved), it is necessary to compute the equivalent load/stiffness by combining several quantities.
 *
 * The Integrator object is acting like an agent between Workshop and Step, that can modify corresponding quantities to account for dynamic effect.
 *
 * @author T
 * @date 27/08/2017
 * @version 0.1.2
 * @file Integrator.h
 * @addtogroup Integrator
 * @{
 */

#ifndef INTERGRATOR_H
#define INTERGRATOR_H

#include <Domain/Tag.h>

class DomainBase;

class Integrator : public Tag {
    weak_ptr<DomainBase> database;

public:
    explicit Integrator(const unsigned& = 0, const unsigned& = CT_INTERGRATOR);
    virtual ~Integrator();

    void set_domain(const weak_ptr<DomainBase>&);
    const weak_ptr<DomainBase>& get_domain() const;

    virtual int initialize();

    virtual int process_load() const;
    virtual int process_constraint() const;
    virtual int process_criterion() const;

    void record() const;

    virtual void assemble_resistance();
    virtual void assemble_matrix();

    void update_trial_time(const double&) const;
    void update_incre_time(const double&) const;
    virtual void update_trial_status();
    virtual void update_incre_status();

    virtual void erase_machine_error() const;

    virtual void commit_status() const;
    virtual void clear_status() const;
    virtual void reset_status() const;
};

#endif

//! @}
