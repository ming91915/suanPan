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

#include <Element/MaterialElement.h>

class CP3 final : public MaterialElement {
    static const unsigned m_node, m_dof;

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
