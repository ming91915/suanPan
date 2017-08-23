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

    unique_ptr<Material> get_copy() override;

    int update_trial_status(const vec&) override;

    int clear_status() override;
    int commit_status() override;
    int reset_status() override;
};

#endif
