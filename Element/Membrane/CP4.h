/**
* @class CP4
* @brief The CP4 class handles CPS4, CPE4, CPS4R and CPE4R elements. It is a four node
* constant
* strain membrane element with optional reduced integration for both plane stress and
* plane strain problems.
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

class CP4 final : public Element
{
    struct IntegrationPoint {
        vec coor;
        double weight, jacob_det;
        mat pn_pxy;
        unique_ptr<Material> m_material;
    };

    static const unsigned m_node;
    static const unsigned m_dof;

    double thickness = 1.;

    bool reduced_scheme = false;

    vector<unique_ptr<IntegrationPoint>> int_pt;

public:
    CP4(const unsigned&,
        const uvec&,
        const unsigned&,
        const double& = 1.,
        const bool& = false,
        const bool& = false);

    void initialize(const shared_ptr<Domain>&) override;

    int updateStatus() override;
    int commitStatus() override;
    int clearStatus() override;
    int resetStatus() override;

    void print() override;
};

#endif

//! @}
