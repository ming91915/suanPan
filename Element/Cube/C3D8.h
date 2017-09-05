/**
* @class C3D8
* @brief The C3D8 class defines C3D8 C3D8R elements.
* @author T
* @date 13/08/2017
* @version 0.1.0
* @file C3D8.h
* @addtogroup Cube
* @ingroup Element
* @{
*/

#ifndef C3D8_H
#define C3D8_H

#include <Element/Element.h>

class C3D8 final : public Element {
    struct IntegrationPoint {
        vec coor;
        double weight, jacob_det;
        mat pn_pxy;
        unique_ptr<Material> c_material;
    };

    static const unsigned c_node;
    static const unsigned c_dof;

    const bool reduced_scheme;

    vector<unique_ptr<IntegrationPoint>> int_pt;

public:
    C3D8(const unsigned&,     // tag
        const uvec&,          // node tags
        const unsigned&,      // material tag
        const bool& = false,  // reduced integration
        const bool& = false); // nonlinear geometry switch

    void initialize(const shared_ptr<Domain>&) override;

    int update_status() override;

    int commit_status() override;
    int clear_status() override;
    int reset_status() override;
};

#endif

//! @}
