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
 * @class PS
 * @brief The PS class defines a Pian--Sumihara membrane element.
 * @author T
 * @date 30/07/2017
 * @version 0.1.0
 * @file PS.h
 * @addtogroup Membrane
 * @ingroup Element
 * @{
 */

#ifndef PS_H
#define PS_H

#include <Element/Element.h>
#include <array>

#ifndef PLANE_STRESS
#define PLANE_STRESS 0
#endif
#ifndef PLANE_STRAIN
#define PLANE_STRAIN 1
#endif

using std::array;

class PS final : public Element {
    struct IntegrationPoint {
        vec coor;
        double weight;
        unique_ptr<Material> m_material;
        mat strain_mat, n_stress;
    };

    static const unsigned m_node;
    static const unsigned m_dof;

    double thickness = 1.;

    unsigned element_type = PLANE_STRESS;

    array<unique_ptr<IntegrationPoint>, 4> int_pt;

    mat ele_coor, inv_stiffness, tmp_a, tmp_c;

public:
    PS(const unsigned&, const uvec&, const unsigned&, const double& = 1., const unsigned& = PLANE_STRESS, const bool& = false);

    void initialize(const shared_ptr<DomainBase>&) override;

    int update_status() override;

    int commit_status() override;
    int clear_status() override;
    int reset_status() override;
};

#endif

//! @}
