/**
 * @class Elastic3D
 * @brief The Elastic3D class defines a isotropic elastic material for
 * 3-D problems.
 *
 * The Young's modulus is stored in `elastic_modulus`. The Poisson's
 * ratio is stored in
 * `poissons_ratio`.
 *
 * @author T
 * @date 30/07/2017
 * @version 0.1.1
 * @file Elastic3D.h
 * @addtogroup Material-3D
 * @ingroup Material
 * @{
 */

#ifndef Elastic3D_H
#define Elastic3D_H

#include <Material/Material.h>

class Elastic3D : public Material {
    double elastic_modulus = 2E5; /**< elastic modulus */
    double poissons_ratio = .2;

public:
    explicit Elastic3D(const unsigned& = 0, const double& = 2E5, const double& = .2, const double& = 0);
    explicit Elastic3D(const double&, const double& = .2, const double& = 0);
    ~Elastic3D();

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
