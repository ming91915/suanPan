/**
* @class CP3
* @brief The CP3 class defines CPS3 CPE3 elements.
* @author T
* @date 10/08/2017
* @version 0.1.0
* @file CP3.h
* @addtogroup Membrane
* @ingroup Element
* @{
*/

#ifndef CP3_H
#define CP3_H

#include <Element/Element.h>

class CP3 final : public Element
{
    static const unsigned m_node;
    static const unsigned m_dof;

    double thickness = 1.; /**< thickness */

    double area = 0.; /**< area */

    mat strain_mat;

    unique_ptr<Material> m_material; /**< store material model */
public:
    CP3(const unsigned&, const uvec&, const unsigned&, const double& = 1.);

    void initialize(const shared_ptr<Domain>&) override;

    int update_status() override;

    int commit_status() override;
    int clear_status() override;
    int reset_status() override;
};

#endif

//! @}
