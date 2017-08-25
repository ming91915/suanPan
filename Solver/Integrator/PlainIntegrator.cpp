#include "PlainIntegrator.h"
#include "Domain/Domain.h"

PlainIntegrator::PlainIntegrator(const unsigned& T, const shared_ptr<Domain>& D)
    : Integrator(T, CT_PLAININTERGRATOR, D)
{
}

int PlainIntegrator::update_status()
{
    auto& D = get_domain();
    D->update_stiffness();
    D->update_resistance();

    return 0;
}

void PlainIntegrator::commit_status() {}