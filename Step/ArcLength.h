
#ifndef ARCLENGTH_H
#define ARCLENGTH_H

#include "Step.h"

class ArcLength : public Step {
    unsigned node, dof;
    double maginitude;

public:
    ArcLength();

    int initialize() override;

    int analyze() override;
};

#endif // ARCLENGTH_H
