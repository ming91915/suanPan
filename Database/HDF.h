#ifndef HDF_H
#define HDF_H

#include "Database.h"

class HDF : public Database {
public:
    HDF();

    int save(const char*) override final;
};

#endif
