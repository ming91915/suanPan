/**
* @class Truss2D
* @brief The Truss2D class handles both linear and nonlinear problems by
* using a
* corotational
* transformation.
* @author T
* @date 14/08/2017
* @version 0.1.1
* @file Truss2D.h
* @addtogroup Truss
* @ingroup Element
* @{
*/

#ifndef TRUSS2D_H
#define TRUSS2D_H

#include <Element/Element.h>

class Truss2D final : public Element {
    static const unsigned t_node;
    static const unsigned t_dof;

    double length = 0.;   /**< length of the element */
    double area = 0.;     /**< area of the cross section */
    vec direction_cosine; /**< direction cosine */

    unique_ptr<Material> t_material; /**< material model */

    const bool update_area; /**<  */
    const bool log_strain;  /**<  */
public:
    explicit Truss2D(const unsigned& T = 0);
    Truss2D(const unsigned&, // tag
        const uvec&,         // node tag
        const unsigned&,     // material tag
        const double&,       // area
        const bool& = false, // nonlinear geometry switch
        const bool& = true,  // update area swicth
        const bool& = true); // log strain swicth

    void initialize(const shared_ptr<Domain>&) override;

    int update_status() override;

    int commit_status() override;
    int clear_status() override;
    int reset_status() override;

    void print() override;
};

#endif

//! @}
