#pragma once
#include <Material/3D/Bilinear3D.h>

class Bilinear2D : public Material {
    const unsigned material_type;

    vec trial_full_strain;

    Bilinear3D base;

public:
    explicit Bilinear2D(const unsigned& = 0, /**< tag */
        const double& = 2E5,                 /**< elastic modulus */
        const double& = .25,                 /**< poisson's ratio */
        const double& = 400.,                /**< initial yield stress */
        const double& = .05,                 /**< hardening ratio */
        const double& = 0.,                  /**< isotropic/kinematic hardening factor */
        const unsigned& = 0,                 /**< plane stress or plane strain */
        const double& = 0.);                 /**< density */

    void initialize() override;

    double get_parameter(const unsigned& = 0) const override;

    unique_ptr<Material> get_copy() override;

    int update_incre_status(const vec&) override;
    int update_trial_status(const vec&) override;

    int clear_status() override;
    int commit_status() override;
    int reset_status() override;
};
