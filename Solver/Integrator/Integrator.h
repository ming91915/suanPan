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
    shared_ptr<DomainBase> database;

public:
    explicit Integrator(const unsigned& = 0, const unsigned& = CT_INTERGRATOR, const shared_ptr<DomainBase>& = nullptr);
    virtual ~Integrator();

    virtual int initialize();

    virtual void process(const unsigned& = 0) const;

    void record() const;

    virtual void update_resistance();
    virtual void update_stiffness();

    void update_trial_time(const double&) const;
    void update_incre_time(const double&) const;
    virtual void update_trial_status();
    virtual void update_incre_status();

    virtual void commit_status() const;
    virtual void clear_status() const;
    virtual void reset_status() const;

    void set_domain(const shared_ptr<DomainBase>&);
    const shared_ptr<DomainBase>& get_domain() const;
};

#endif

//! @}
