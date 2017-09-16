/**
 * @class Load
 * @brief A Load class.
 *
 * The Load class is in charge of returning load level according to given time increment.
 *
 * @author T
 * @date 05/09/2017
 * @version 0.1.0
 * @file Load.h
 * @addtogroup Load
 * @{
 */

#ifndef LOAD_H
#define LOAD_H

#include <Domain/Tag.h>

class Amplitude;
class DomainBase;

class Load : public Tag {
protected:
    static const double multiplier;

    unsigned step_tag = 0;

    shared_ptr<Amplitude> magnitude;

public:
    explicit Load(const unsigned& = 0, const unsigned& = CT_LOAD, const unsigned& = 0, const shared_ptr<Amplitude>& = nullptr);
    virtual ~Load();

    virtual int process(const shared_ptr<DomainBase>&) = 0;

    void set_step_tag(const unsigned&);
    const unsigned& get_step_tag() const;
};

#endif

//! @}
