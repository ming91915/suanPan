#pragma once
#include "Domain/Domain.h"

class Database : public Tag
{
    shared_ptr<Domain> domain;

public:
    explicit Database(const unsigned& = 0, const unsigned& = 0);
    virtual ~Database();

    void setDomain(const shared_ptr<Domain>& D);
    const shared_ptr<Domain>& getDomain() const;

    virtual int save(const char*) = 0;
};
