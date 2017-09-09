/**
 * @class CP4
 * @brief The CP4 class handles CPS4, CPE4, CPS4R and CPE4R elements. It
 * is a four node
 * constant strain membrane element with optional reduced integration for
 * both plane stress
 * and plane strain problems.
 * @author T
 * @date 07/08/2017
 * @version 0.1.2
 * @file CP4.h
 * @addtogroup Membrane
 * @ingroup Element
 * @{
 */

#ifndef CP4_H
#define CP4_H

#include <Element/Element.h>

class CP4 final : public Element {
    struct IntegrationPoint {
        vec coor;
        double weight, jacob_det;
        mat pn_pxy;
        unique_ptr<Material> m_material;
    };

    static const unsigned m_node;
    static const unsigned m_dof;

    const double thickness;

    const bool reduced_scheme;

    vector<unique_ptr<IntegrationPoint>> int_pt;

public:
    CP4(const unsigned&,      // tag
        const uvec&,          // node tags
        const unsigned&,      // material tag
        const double& = 1.,   // thickness
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
