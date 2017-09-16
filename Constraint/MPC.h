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
 * @class MPC
 * @brief A MPC class.
 *
 * @author T
 * @date 05/09/2017
 * @version 0.1.0
 * @file MPC.h
 * @addtogroup Constraint
 * @{
 */

#ifndef MPC_H
#define MPC_H

#include "Constraint.h"

class MPC : public Constraint {
public:
    explicit MPC(const unsigned& = 0, const unsigned& = CT_MPC, const unsigned& = 0);
    virtual ~MPC();
};

#endif

//! @}
