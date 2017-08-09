/**
* @class Bilinear3D
* @brief The Bilinear3D class defines a bilinear hardening material with mixed hardening
* (isotropic and kinematic) based on J2 plasticity rule.
* @author T
* @date 08/08/2017
* @version 0.1.0
* @file Bilinear3D.h
* @addtogroup Material-3D
* @ingroup Material
* @{
*/

#ifndef BILINEAR3D_H
#define BILINEAR3D_H

#include <Material/Material.h>

class Bilinear3D : public Material
{
    static const vec norm_weight;
    static const double root_two_third;

    const double elastic_modulus; /**< elastic modulus */
    const double poissons_ratio;  /**< poisson's ratio */
    const double yield_stress;    /**< initial yield stress */
    const double hardening_ratio; /**< hardening ratio */
    const double beta;            /**< isotropic/kinematic hardening factor */

    double tolerance;

    double shear_modulus;       /**< shear modulus */
    double double_shear;        /**< double shear modulus */
    double square_double_shear; /**< double shear modulus */
    double plastic_modulus;     /**< plastic modulus */

    double factor;

    double current_plastic_strain;
    double trial_plastic_strain;

    vec current_back_stress;
    vec trial_back_stress;

public:
    explicit Bilinear3D(const unsigned& = 0, /**< tag */
        const double& = 2E5,                 /**< elastic modulus */
        const double& = 0.,                  /**< poisson's ratio */
        const double& = 0.,                  /**< initial yield stress */
        const double& = 0.,                  /**< hardening ratio */
        const double& = 0.,                  /**< isotropic/kinematic hardening factor */
        const double& = 0.);                 /**< density */
    explicit Bilinear3D(const double&,       /**< elastic modulus */
        const double& = 0.,                  /**< poisson's ratio */
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
