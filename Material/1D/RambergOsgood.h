#ifndef RAMBERGOSGOOD_H
#define RAMBERGOSGOOD_H

#include <Material/Material.h>

class RambergOsgood final : public Material {
    const double elastic_modulus;
    const double yield_stress;
    const double offset;
    const double n;
    const double nm;

    const double tolerance;

public:
    explicit RambergOsgood(const unsigned& = 0, const double& = 2E5, const double& = 400., const double& = 1., const double& = 10., const double& = 0.);

    void initialize() override;

    unique_ptr<Material> get_copy() override;

    int update_incre_status(const vec&) override;
    int update_trial_status(const vec&) override;

    int clear_status() override;
    int commit_status() override;
    int reset_status() override;
};

#endif
