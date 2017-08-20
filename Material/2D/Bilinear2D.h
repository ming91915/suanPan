#pragma once
#include <Material/3D/Bilinear3D.h>

class Bilinear2D : public Material
{
    unsigned material_type;

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

    double getParameter(const unsigned& = 0) const override;

    unique_ptr<Material> getCopy() override;

    int updateIncreStatus(const vec&) override;
    int updateTrialStatus(const vec&) override;

    int clearStatus() override;
    int commitStatus() override;
    int resetStatus() override;
};
