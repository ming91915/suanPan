/**
 * @class C3D20
 * @brief The C3D20 class defines C3D20 C3D20R elements.
 * @author T
 * @date 14/09/2017
 * @version 0.1.0
 * @file C3D20.h
 * @addtogroup Cube
 * @ingroup Element
 * @{
 */

#ifndef C3D20_H
#define C3D20_H

#include <Element/Element.h>

class C3D20 final : public Element {
    struct IntegrationPoint {
        vec coor;
        double weight, jacob_det;
        mat strain_mat;
        unique_ptr<Material> c_material;
    };

    static const unsigned c_node, c_dof;

    const bool reduced_scheme;

    vector<unique_ptr<IntegrationPoint>> int_pt;

public:
    C3D20(const unsigned&,    // tag
        const uvec&,          // node tags
        const unsigned&,      // material tag
        const bool& = false,  // reduced integration
        const bool& = false); // nonlinear geometry switch

    void initialize(const shared_ptr<Domain>&) override;

    int update_status() override;

    int commit_status() override;
    int clear_status() override;
    int reset_status() override;

    void print() override;
};

#endif

//! @}
