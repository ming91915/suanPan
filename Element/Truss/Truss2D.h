/**
* @class Truss2D
* @brief The Truss2D class handles both linear and nonlinear problems by using a
* corotational
* transformation.
* @author T
* @date 03/08/2017
* @version 0.1.0
* @file Truss2D.h
* @addtogroup Truss
* @ingroup Element
* @{
*/

#ifndef TRUSS2D_H
#define TRUSS2D_H

#include <Element/Element.h>

class Truss2D final : public Element
{
    double length = 0.;   /**< length of the element */
    double area = 0.;     /**< area of the cross section */
    vec direction_cosine; /**< direction cosine */

    unique_ptr<Material> t_material; /**< material model */

    bool update_area = true; /**<  */
    bool log_strain = true;  /**<  */
public:
    explicit Truss2D(const unsigned& T = 0);
    Truss2D(const unsigned&,
        const uvec&,
        const unsigned&,
        const double&,
        const bool& = false,
        const bool& = true,
        const bool& = true);

    void initialize(const shared_ptr<Domain>&) override;

    int updateStatus() override;
    int commitStatus() override;
    int clearStatus() override;
    int resetStatus() override;

    void print() override;
};

#endif

//! @}
