
#ifndef DISPLACEMENTCONTROL_H
#define DISPLACEMENTCONTROL_H

#include "Solver.h"

class DisplacementControl : public Solver {
public:
    DisplacementControl();

    int analyze() override;
};

#endif // DISPLACEMENTCONTROL_H
