#ifndef CONCRETR01_H
#define CONCRETE01_H

#include <Material/Material.h>

class Concrete01 : public Material
{
public:
    Concrete01(const unsigned& T, const double& E)
        : Material(T, MT_CONCRETE01)
    {
        initial_stiffness = { E };
        Concrete01::initialize();
    }

    void initialize() override;
    unique_ptr<Material> getCopy() override;
    int updateTrialStatus(const vec&) override;
    int clearStatus() override;
    int commitStatus() override;
    int resetStatus() override;
};

#endif
