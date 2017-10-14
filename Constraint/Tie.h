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
 * @class Tie
 * @brief A Tie class.
 *
 * The Tie class.
 *
 * @author T
 * @date 29/07/2017
 * @version 0.1.0
 * @file Tie.h
 * @addtogroup Constraint
 * @{
 */

#ifndef TIE_H
#define TIE_H

#include <Constraint/MPC.h>

class Tie final : public MPC {
    unsigned node_i;
    unsigned dof_i;
    unsigned node_j;
    unsigned dof_j;

public:
    Tie(const unsigned& T,   // tag
        const unsigned& S,   // step tag
        const unsigned& NA,  // node a
        const unsigned& DA,  // dof a
        const unsigned& NB,  // node b
        const unsigned& DB); // dof b
    Tie(const unsigned& S, const unsigned& NA, const unsigned& DA, const unsigned& NB, const unsigned& DB);

    int process(const shared_ptr<DomainBase>&) override;
};

#endif

//! @}
