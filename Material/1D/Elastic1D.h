/**
* @class Elastic1D
* @brief A 1-D Elastic class.
* @author T
* @date 27/07/2017
* @file Elastic1D.h
* @addtogroup Material-1D
* @ingroup Material
* @{
*/

#ifndef ELASTIC1D_H
#define ELASTIC1D_H

#include <Material/Material.h>

class Elastic1D : public Material {
    const double elastic_modulus; /**< elastic modulus */
public:
    explicit Elastic1D(const unsigned& = 0, const double& = 2E5, const double& = 0.);
    explicit Elastic1D(const double&, const double& = 0.);

    void initialize() override;

    unique_ptr<Material> get_copy() override;

    int update_trial_status(const vec&) override;

    int clear_status() override;
    int commit_status() override;
    int reset_status() override;

    void print() override;
};

#endif

//! @}
