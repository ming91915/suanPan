
#ifndef DISPLACEMENTLOAD_H
#define DISPLACEMENTLOAD_H

#include "Load.h"

class DisplacementLoad : public Load {
    double pattern = 0.;

    uvec nodes;
    uvec dofs;

public:
    explicit DisplacementLoad(const unsigned& = 0, const unsigned& = 0, const double& = 0., const uvec& = {}, const unsigned& = 0, const shared_ptr<Amplitude>& = nullptr);
    DisplacementLoad(const unsigned&, const unsigned&, const double&, const uvec&, const uvec&, const shared_ptr<Amplitude>& = nullptr);

    int process(const shared_ptr<DomainBase>&) override;
};

#endif // DISPLACEMENTLOAD_H
