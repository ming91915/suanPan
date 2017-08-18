#include "Database.h"

Database::Database(const unsigned& T, const unsigned& CT)
    : Tag(T, CT)
{
}

Database::~Database() {}

void Database::setDomain(const shared_ptr<Domain>& D) { domain = D; }

const shared_ptr<Domain>& Database::getDomain() const { return domain; }
