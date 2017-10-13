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
 * @class F21
 * @brief The F21 class.
 * @author T
 * @date 11/10/2017
 * @version 0.1.1
 * @file F21.h
 * @addtogroup Beam
 * @ingroup Element
 * @{
 */

#ifndef F21_H
#define F21_H

#include <Element/Element.h>

class F21 final : public Element {
    static const unsigned b_node, b_dof;

    const unsigned int_pt_num;

    double length = 0., inclination = 0.;

    vec direction_cosine; /**< direction cosine */

    struct IntegrationPoint {
        double coor, weight;
        unique_ptr<Section> b_section;
        mat B;
        vec current_section_deformation, trial_section_deformation;
        vec current_section_resistance, trial_section_resistance;
        IntegrationPoint(const double C, const double W, unique_ptr<Section>&& M)
            : coor(C)
            , weight(W)
            , b_section(move(M))
            , B(2, 3, fill::zeros)
            , current_section_deformation(2, fill::zeros)
            , trial_section_deformation(2, fill::zeros)
            , current_section_resistance(2, fill::zeros)
            , trial_section_resistance(2, fill::zeros) {}
        void commit_status() {
            current_section_deformation = trial_section_deformation;
            current_section_resistance = trial_section_resistance;
        }
        void clear_status() {
            current_section_deformation.zeros();
            trial_section_deformation.zeros();
            current_section_resistance.zeros();
            trial_section_resistance.zeros();
        }
        void reset_status() {
            trial_section_deformation = current_section_deformation;
            trial_section_resistance = current_section_resistance;
        }
    };

    vector<IntegrationPoint> int_pt;

    mat trans_mat, initial_local_flexibility;

    mat current_local_flexibility, trial_local_flexibility;
    vec current_local_deformation, trial_local_deformation;
    vec current_local_resistance, trial_local_resistance;

public:
    F21(const unsigned&,     // tag
        const uvec&,         // node tags
        const unsigned&,     // section tags
        const unsigned& = 3, // integration points
        const bool& = false  // nonliear geometry switch
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
