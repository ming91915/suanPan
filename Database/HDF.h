#pragma once
#include "Database.h"
class HDF : public Database
{
public:
    HDF();

    int save(const char*) override final;
};
