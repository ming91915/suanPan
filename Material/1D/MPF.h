/**
* @class MPF
* @brief A 1-D Elastic class.
* @author T
* @date 11/08/2017
* @version 0.1.0
* @file MPF.h
* @addtogroup Material-1D
* @ingroup Material
* @{
*/

#ifndef MPF_H
#define MPF_H

#include <Material/Material.h>

class MPF final : public Material
{
    const double elastic_modulus;    // elastic modulus
    const double yield_stress;       // yield stress
    const double hardening_ratio;    // hardening ratio
    const double R0, A1, A2, A3, A4; // model parameters

    const bool isotropic_hardening; // isotropic hardening switch
    const bool constant_radius;     // constant radius switch

    const double yield_strain; // yield strain

public:
    explicit MPF(const unsigned& = 0, // tag
        const double& = 2E5,          // elastic modulus
        const double& = 400.,         // yield stress
        const double& = .05,          // hardening ratio
        const double& = 20.,          // R0
        const double& = 18.5,         // A1
        const double& = .15,          // A2
        const double& = .01,          // A3
        const double& = 7.,           // A4
        const bool& = false,          // isotropic hardening switch
        const bool& = false,          // constant radius switch
        const double& = 0.);          // density

    void initialize() override;

    unique_ptr<Material> getCopy() override;

    int updateIncreStatus(const vec&) override;
    int updateTrialStatus(const vec&) override;

    int clearStatus() override;
    int commitStatus() override;
    int resetStatus() override;
};

#endif

//! @}
