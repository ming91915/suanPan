/**
* @class MaterialExample
* @brief A MaterialExample material class.
* @author T
* @date 23/08/2017
* @version 0.1.0
* @file MaterialExample.h
* @addtogroup Material-1D
* @ingroup Material
* @{
*/

#ifndef MaterialExample_H
#define MaterialExample_H

#include <Material/Material.h>

class MaterialExample final : public Material {
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
    explicit MaterialExample(const unsigned& = 0, /**< tag */
        const double& = 2E5,                      /**< elastic modulus */
        const double& = 400.,                     /**< initial yield stress */
        const double& = .05,                      /**< hardening ratio */
        const double& = 0.,                       /**< isotropic/kinematic hardening factor */
        const double& = 0.);                      /**< density */

    void initialize() override;

    unique_ptr<Material> get_copy() override;

    int update_trial_status(const vec&) override;

    int clear_status() override;
    int commit_status() override;
    int reset_status() override;
};

#endif

//! @}
