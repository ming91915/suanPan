
#ifndef ARCLENGTH_H
#define ARCLENGTH_H

#include "Step.h"

class ArcLength : public Step {
    unsigned node, dof;
    double maginitude;

public:
    explicit ArcLength(const unsigned& = 0, // tag
        const unsigned& = 0,                // node tag
        const unsigned& = 0,                // dof tag
        const double& = 0.);                // magnitude

    int initialize() override;

    int analyze() override;
};

#endif // ARCLENGTH_H
