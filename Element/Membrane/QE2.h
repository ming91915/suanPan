/**
* @class QE2
* @brief A QE2 class.
*
*
* @author T
* @date 30/07/2017
* @version 0.1.0
* @file QE2.h
* @addtogroup Membrane
* @ingroup Element
* @{
*/

#ifndef QE2_H
#define QE2_H

#include <Element/Element.h>

class QE2 : public Element
{
    struct IntegrationPoint {
        vec coor;
        double weight, jacob_det;
        mat jacob, strain_mat, pn_pxy;
        unique_ptr<Material> m_material;
    };

    static const unsigned m_node;
    static const unsigned m_dof;

    double thickness = 1.;

    vector<unique_ptr<IntegrationPoint>> int_pt;

    mat ele_coor, inv_stiffness;

public:
    QE2(const unsigned& T, const uvec& N, const unsigned& M, const double& TH = 1.);

    void initialize(const shared_ptr<Domain>&) override;

    int updateStatus() override;
    int commitStatus() override;
    int clearStatus() override;
    int resetStatus() override;
};

#endif

//! @}
