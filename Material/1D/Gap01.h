/**
* @class Gap01
* @brief The Gap01 class defines a gap material. The material does not
* response to the
* compression, but show bilinear elastic-perfectly plastic response with
* the elastic
* modulus of \f$E\f$ and the yield stress of \f$\sigma_y\f$. The
* material also shows a gap
* feature during reloading stage, i.e., from zero stress to tension
* stress.
* @author T
* @date 15/08/2017
* @version 0.1.0
* @file Gap01.h
* @addtogroup Material-1D
* @ingroup Material
* @{
*/

#ifndef GAP01_H
#define GAP01_H

#include <Material/Material.h>

class Gap01 : public Material {
    const double elastic_modulus;
    const double yield_stress;
    const double gap_strain;

    double current_load_flag = 0.;
    double trial_load_flag = 0.;
    double current_reverse_strain = 0.;
    double trial_reverse_strain = 0.;

public:
    explicit Gap01(const unsigned& = 0, // tag
        const double& = 2E5,            // elastic modulus
        const double& = 200.,           // yield stress
        const double& = 0.,             // gap strain
        const double& = 0.);            // density

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
