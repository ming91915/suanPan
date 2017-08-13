/**
* @class Bilinear1D
* @brief A Bilinear1D material class.
* @author T
* @date 08/08/2017
* @version 0.1.0
* @file Bilinear1D.h
* @addtogroup Material-1D
* @ingroup Material
* @{
*/

#ifndef BILINEAR1D_H
#define BILINEAR1D_H

#include <Material/Material.h>

class Bilinear1D final : public Material
{
    const double elastic_modulus; /**< elastic modulus */
    const double yield_stress;    /**< initial yield stress */
    const double hardening_ratio; /**< hardening ratio */
    const double beta;            /**< isotropic/kinematic hardening factor */
    const double plastic_modulus; /**< plastic modulus */

    const double tolerance;

    double current_back_stress = 0.;
    double current_plastic_strain = 0.;
    double trial_back_stress = 0.;
    double trial_plastic_strain = 0.;

public:
    explicit Bilinear1D(const unsigned& = 0, /**< tag */
        const double& = 2E5,                 /**< elastic modulus */
        const double& = 0.,                  /**< initial yield stress */
        const double& = 0.,                  /**< hardening ratio */
        const double& = 0.,                  /**< isotropic/kinematic hardening factor */
        const double& = 0.);                 /**< density */
    explicit Bilinear1D(const double&,       /**< elastic modulus */
        const double& = 0.,                  /**< initial yield stress */
        const double& = 0.,                  /**< hardening ratio */
        const double& = 0.,                  /**< isotropic/kinematic hardening factor */
        const double& = 0.);                 /**< density */

    void initialize() override;

    unique_ptr<Material> getCopy() override;

    int updateTrialStatus(const vec&) override;

    int clearStatus() override;
    int commitStatus() override;
    int resetStatus() override;
};

#endif

//! @}
