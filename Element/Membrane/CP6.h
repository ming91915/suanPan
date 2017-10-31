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
 * @class CP6
 * @brief The CP6 class defines CPS6 CPE6 elements.
 * @author T
 * @date 26/10/2017
 * @version 0.1.1
 * @file CP6.h
 * @addtogroup Membrane
 * @ingroup Element
 * @{
 */

#ifndef CP6_H
#define CP6_H

#include <Element/MaterialElement.h>

class CP6 final : public MaterialElement {
    static const unsigned m_node, m_dof;

    const double thickness; /**< thickness */

    double area = 0.; /**< area */

    struct IntegrationPoint {
        vec coor;
        double weight;
        unique_ptr<Material> m_material;
        mat pn_pxy;
        IntegrationPoint(const vec&, const double, unique_ptr<Material>&&, const mat&);
    };

    vector<IntegrationPoint> int_pt;

public:
    CP6(const unsigned, const uvec&, const unsigned, const double = 1.);

    void initialize(const shared_ptr<DomainBase>&) override;

    int update_status() override;

    int commit_status() override;
    int clear_status() override;
    int reset_status() override;

    void print() override;
};

#endif

//! @}
