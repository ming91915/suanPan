/*******************************************************************************
 * Copyright (C) 2017 Theodore Chang
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/
/**
 * @class T2D2
 * @brief The T2D2 class handles both linear and nonlinear problems by
 * using a
 * corotational
 * transformation.
 * @author T
 * @date 14/08/2017
 * @version 0.1.1
 * @file T2D2.h
 * @addtogroup Truss
 * @ingroup Element
 * @{
 */

#ifndef T2D2_H
#define T2D2_H

#include <Element/MaterialElement.h>

class T2D2 final : public MaterialElement {
    static const unsigned t_node, t_dof;

    double length = 0.;   /**< length of the element */
    double area = 0.;     /**< area of the cross section */
    vec direction_cosine; /**< direction cosine */

    unique_ptr<Material> t_material; /**< material model */

    const bool update_area; /**<  */
    const bool log_strain;  /**<  */
public:
    T2D2(const unsigned&,    // tag
        const uvec&,         // node tag
        const unsigned&,     // material tag
        const double&,       // area
        const bool& = false, // nonlinear geometry switch
        const bool& = true,  // update area swicth
        const bool& = true   // log strain swicth
    );

    void initialize(const shared_ptr<DomainBase>&) override;

    int update_status() override;

    int commit_status() override;
    int clear_status() override;
    int reset_status() override;

    void print() override;
};

#endif

//! @}
