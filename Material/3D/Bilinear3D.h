/**
* @class Bilinear3D
* @brief The Bilinear3D class defines a bilinear hardening material with
* mixed hardening
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

class Bilinear3D : public Material {
    static const vec norm_weight;
    static const double root_two_third;
    static const mat unit_dev_tensor;

    const double elastic_modulus; /**< elastic modulus */
    const double poissons_ratio;  /**< poisson's ratio */
    const double yield_stress;    /**< initial yield stress */
    const double hardening_ratio; /**< hardening ratio */
    const double beta;            /**< isotropic/kinematic hardening factor */

    const double tolerance;

    const double shear_modulus;       /**< shear modulus */
    const double double_shear;        /**< double shear modulus */
    const double square_double_shear; /**< double shear modulus */
    const double plastic_modulus;     /**< plastic modulus */

    const double factor;

    double current_plastic_strain;
    double trial_plastic_strain;

    vec current_back_stress;
    vec trial_back_stress;

public:
    explicit Bilinear3D(const unsigned& = 0, /**< tag */
        const double& = 2E5,                 /**< elastic modulus */
        const double& = .25,                 /**< poisson's ratio */
        const double& = 400.,                /**< initial yield stress */
        const double& = 0.,                  /**< hardening ratio */
        const double& = 0.,                  /**< isotropic/kinematic hardening factor */
        const double& = 0.);                 /**< density */

    void initialize() override;

    unique_ptr<Material> get_copy() override;

    int update_incre_status(const vec&) override;
    int update_trial_status(const vec&) override;

    int clear_status() override;
    int commit_status() override;
    int reset_status() override;
};

#endif

//! @}
