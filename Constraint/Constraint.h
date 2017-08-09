/**
 * @class Constraint
 * @brief A Constraint class.
 *
 * The Constraint class.
 *
 * @author T
 * @date 03/07/2017
 * @file Constraint.h
 */

#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include <Domain/Tag.h>

class Domain;

class Constraint : public Tag
{
    unsigned step_tag = 0; /**< step tag */
public:
    explicit Constraint(const unsigned& = 0,
        const unsigned& = CT_CONSTRAINT,
        const unsigned& = 0);
    virtual ~Constraint();

    void setStepTag(const unsigned&);
    const unsigned& getStepTag() const;

    virtual int process(const shared_ptr<Domain>&) = 0;
};

#endif
