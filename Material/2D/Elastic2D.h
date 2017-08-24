/**
* @class Elastic2D
* @brief The Elastic2D class defines a isotropic elastic material for plane stress and
* plane strain problems.
*
* The Young's modulus is stored in `elastic_modulus`. The Poisson's ratio is stored in
* `poissons_ratio`. The `material_type` labels if it is plane stress or plane strain. The
* default value `0` represents plane stress. Initializing the object with a nonzero value
* gives a plane strain type response.
*
* @author T
* @date 29/07/2017
* @version 0.1.1
* @file Elastic2D.h
* @addtogroup Material-2D
* @ingroup Material
* @{
*/

#ifndef ELASTIC2D_H
#define ELASTIC2D_H

#include <Material/Material.h>

class Elastic2D : public Material
{
    const double elastic_modulus; // elastic modulus
    const double poissons_ratio;  // poissons ratio

    unsigned material_type = 0;

public:
    explicit Elastic2D(const unsigned& = 0,
        const double& = 2E5,
        const double& = .2,
        const double& = 0,
        const unsigned& = 0);
    explicit Elastic2D(const double&,
        const double& = .2,
        const double& = 0,
        const unsigned& = 0);
    ~Elastic2D();

    void initialize() override;

    unique_ptr<Material> get_copy() override;

    int update_incre_status(const vec&) override;
    int update_trial_status(const vec&) override;

    int clear_status() override;
    int commit_status() override;
    int reset_status() override;

    void print() override;

    double get_parameter(const unsigned& = 0) const override;
};

#endif

//! @}
