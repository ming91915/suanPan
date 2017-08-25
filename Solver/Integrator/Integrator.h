/**
* @class Integrator
* @brief An Integrator class.
*
* @author T
* @date 25/08/2017
* @version 0.1.1
* @file Integrator.h
*/

#ifndef INTERGRATOR_H
#define INTERGRATOR_H

#include <Domain/Tag.h>

class Domain;

class Integrator : public Tag
{
    bool initialized = false;

    shared_ptr<Domain> database;

public:
    explicit Integrator(const unsigned& = 0,
        const unsigned& = CT_INTERGRATOR,
        const shared_ptr<Domain>& = nullptr);
    virtual ~Integrator();

    const bool& is_initialized() const;

    virtual int initialize();

    void set_domain(const shared_ptr<Domain>&);
    const shared_ptr<Domain>& get_domain() const;

    virtual int update_status() = 0;

    virtual void commit_status() = 0;
};

#endif
