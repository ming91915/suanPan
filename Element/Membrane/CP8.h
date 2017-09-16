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
 * @class CP8
 * @brief The CP8 class handles CPS8, CPE8, CPS8R and CPE8R elements. It is a four node constant strain membrane element with optional reduced integration for both plane stress and plane strain problems.
 * @author T
 * @date 12/09/2017
 * @version 0.1.2
 * @file CP8.h
 * @addtogroup Membrane
 * @ingroup Element
 * @{
 */

#ifndef CP8_H
#define CP8_H

#include <Element/Element.h>

class CP8 final : public Element {
    struct IntegrationPoint {
        vec coor;
        double weight, jacob_det;
        mat pn_pxy;
        unique_ptr<Material> m_material;
    };

    static const unsigned m_node;
    static const unsigned m_dof;

    const double thickness;

    const bool reduced_scheme;

    vector<unique_ptr<IntegrationPoint>> int_pt;

public:
    CP8(const unsigned&,      // tag
        const uvec&,          // node tags
        const unsigned&,      // material tag
        const double& = 1.,   // thickness
        const bool& = false,  // reduced integration
        const bool& = false); // nonlinear geometry switch

    void initialize(const shared_ptr<DomainBase>&) override;

    int update_status() override;

    int commit_status() override;
    int clear_status() override;
    int reset_status() override;

    void print() override;
};

#endif

//! @}
