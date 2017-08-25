#pragma once
#include "Integrator.h"
class PlainIntegrator : public Integrator
{
public:
    explicit PlainIntegrator(const unsigned& = 0, const shared_ptr<Domain>& = nullptr);

    int update_status() override;
    void commit_status() override;
};
