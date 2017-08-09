/**
* @class GQ12
* @brief The GQ12 class implements the displacement based four node quadrilateral drilling
* element proposed by Long and Xu (1994).
*
* https://doi.org/10.1016/0045-7949(94)90356-5
*
* The element assumes the displacement field is compatible/conforming on element
* boundaries in an averaged/weak sense. The element does not pass the patch test rigidly
* but exhibits a good performance.
*
* @author T
* @date 07/08/2017
* @version 0.1.0
* @file GQ12.h
* @addtogroup Membrane
* @ingroup Element
* @{
*/

#ifndef GQ12_H
#define GQ12_H

#include <Element/Element.h>
class GQ12 : public Element
{
    struct IntegrationPoint {
        vec coor;
        double weight, jacob_det;
        mat strain_mat;
        unique_ptr<Material> m_material;
    };

    static const unsigned m_node;
    static const unsigned m_dof;

    double thickness;

    vector<unique_ptr<IntegrationPoint>> int_pt;

public:
    GQ12(const unsigned& T, const uvec& N, const unsigned& M, const double& TH = 1.);

    void initialize(const shared_ptr<Domain>&) override;

    int updateStatus() override;
    int commitStatus() override;
    int clearStatus() override;
    int resetStatus() override;
};

#endif

//! @}
