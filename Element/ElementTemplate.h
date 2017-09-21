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
 * @class ElementTemplate
 * @brief The ElementTemplate class illustrates the basic formulation a typical Element class used in FEM analysis.
 *
 * @author T
 * @date 05/09/2017
 * @version 0.1.2
 * @file ElementTemplate.h
 * @addtogroup Element
 * @{
 */

#ifndef ELEMENTTEMPLATE_H
#define ELEMENTTEMPLATE_H

#include "Element.h"

class ElementTemplate : public Element {
    /**
     * @brief As a universal practice, we define two static constants to represent the number of nodes and the number of DoFs. This is not necessary but only for clearness.
     */
    static const unsigned m_node;
    static const unsigned m_dof;

    double thickness = 0.; /**< thickness */

    double area = 0.; /**< area */

    mat strain_mat;

    unique_ptr<Material> m_material; /**< store material model */
public:
    ElementTemplate(const unsigned&, const uvec&, const unsigned&, const double& = 1.);

    void initialize(const shared_ptr<DomainBase>&) override;

    int update_status() override;

    int commit_status() override;
    int clear_status() override;
    int reset_status() override;
};

#endif

//! @}
