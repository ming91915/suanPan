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
 * @class GQ12
 * @brief The GQ12 class implements the displacement based four node quadrilateral drilling element proposed by Long and Xu (1994).
 *
 * https://doi.org/10.1016/0045-7949(94)90356-5
 *
 * The element assumes the displacement field is compatible/conforming on element boundaries in an averaged/weak sense. The element does not pass the patch test rigidly but exhibits a good performance.
 *
 * @author T
 * @date 05/09/2017
 * @version 0.1.1
 * @file GQ12.h
 * @addtogroup Membrane
 * @ingroup Element
 * @{
 */

#ifndef GQ12_H
#define GQ12_H

#include <Element/Element.h>

class GQ12 : public Element {
    struct IntegrationPoint {
        vec coor;
        double weight, jacob_det;
        mat strain_mat;
        unique_ptr<Material> m_material;
    };

    static const unsigned m_node;
    static const unsigned m_dof;

    const double thickness;

    vector<unique_ptr<IntegrationPoint>> int_pt;

public:
    GQ12(const unsigned&, const uvec&, const unsigned&, const double& = 1.);

    void initialize(const shared_ptr<DomainBase>&) override;

    int update_status() override;

    int commit_status() override;
    int clear_status() override;
    int reset_status() override;

    void print() override;
};

#endif

//! @}
