/**
* @class Database
* @brief A Database class is a top level container.
* @author T
* @date 27/08/2017
* @version 0.2.1
* @file Database.h
*/

#ifndef DATABASE_H
#define DATABASE_H

#include "Domain/Domain.h"

class Database : public Tag {
    shared_ptr<Domain> domain;

public:
    explicit Database(const unsigned& = 0, const unsigned& = 0);
    virtual ~Database();

    void setDomain(const shared_ptr<Domain>& D);
    const shared_ptr<Domain>& getDomain() const;

    virtual int save(const char*) = 0;
};

#endif
