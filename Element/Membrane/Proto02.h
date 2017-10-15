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
 * @class Proto02
 * @brief A Proto02 class.
 *
 * @author T
 * @date 13/09/2017
 * @version 0.2.0
 * @file Proto02.h
 * @addtogroup Membrane
 * @ingroup Element
 * @{
 */

#ifndef PROTO02_H
#define PROTO02_H

#include <Element/Element.h>

class Proto02 : public Element {
    struct IntegrationPoint {
        vec coor;
        double weight, jacob_det;
        unique_ptr<Material> m_material;
        mat P, A, B, BI;
    };

    static const unsigned m_node, m_dof;

    static mat mapping;

    const double thickness;

    vector<unique_ptr<IntegrationPoint>> int_pt;

    mat HI, HIL, HILI; // constant matrices

    mat trial_ht, current_ht;

    vec trial_disp, current_disp;     // displacement
    vec trial_lambda, current_lambda; // enhanced strain
    vec trial_alpha, current_alpha;   // strain
    vec trial_beta, current_beta;     // stress

    mat initial_qtitt;
    mat trial_qtitt, current_qtitt; // eq. 65
    mat trial_qtifi, current_qtifi; // eq. 65

public:
    Proto02(const unsigned&, const uvec&, const unsigned&, const double& = 1.);

    void initialize(const shared_ptr<DomainBase>&) override;

    int update_status() override;

    int commit_status() override;
    int clear_status() override;
    int reset_status() override;

    vector<vec> record(const OutputType&) override;

    void print() override;
};

#endif

//! @}
