/**
* @class PS
* @brief The PS class defines a Pian--Sumihara membrane element.
* @author T
* @date 30/07/2017
* @version 0.1.0
* @file PS.h
* @addtogroup Membrane
* @ingroup Element
* @{
*/

#ifndef PS_H
#define PS_H

#include <Element/Element.h>
#include <array>

#ifndef PLANE_STRESS
#define PLANE_STRESS 0
#endif
#ifndef PLANE_STRAIN
#define PLANE_STRAIN 1
#endif

using std::array;

class PS final : public Element
{
    struct IntegrationPoint {
        vec coor;
        double weight;
        unique_ptr<Material> m_material;
        mat strain_mat, n_stress;
    };

    static const unsigned m_node;
    static const unsigned m_dof;

    double thickness = 1.;

    unsigned element_type = PLANE_STRESS;

    array<unique_ptr<IntegrationPoint>, 4> int_pt;

    mat ele_coor, inv_stiffness, tmp_a, tmp_c;

public:
    PS(const unsigned&,
        const uvec&,
        const unsigned&,
        const double& = 1.,
        const unsigned& = PLANE_STRESS,
        const bool& = false);

    void initialize(const shared_ptr<Domain>&) override;

    int update_status() override;

    int commit_status() override;
    int clear_status() override;
    int reset_status() override;
};

#endif

//! @}
