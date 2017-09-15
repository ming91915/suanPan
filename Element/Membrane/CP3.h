/**
 * @class CP3
 * @brief The CP3 class defines CPS3 CPE3 elements.
 * @author T
 * @date 05/09/2017
 * @version 0.1.1
 * @file CP3.h
 * @addtogroup Membrane
 * @ingroup Element
 * @{
 */

#ifndef CP3_H
#define CP3_H

#include <Element/Element.h>

class CP3 final : public Element {
    static const unsigned m_node;
    static const unsigned m_dof;

    const double thickness; /**< thickness */

    double area = 0.; /**< area */

    mat pn_pxy;

    unique_ptr<Material> m_material; /**< store material model */
public:
    CP3(const unsigned&, const uvec&, const unsigned&, const double& = 1.);

    void initialize(const shared_ptr<DomainBase>&) override;

    int update_status() override;

    int commit_status() override;
    int clear_status() override;
    int reset_status() override;

    void print() override;
};

#endif

//! @}
