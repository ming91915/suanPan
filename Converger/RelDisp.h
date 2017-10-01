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
 * @class RelDisp
 * @brief The RelDisp class that handles converger test to indicate if the iteration converges.
 * @author T
 * @date 08/08/2017
 * @version 0.2.0
 * @file RelDisp.h
 * @addtogroup Converger
 * @{
 */

#ifndef RELDISP_H
#define RELDISP_H

#include "Converger.h"

class RelDisp final : public Converger {
public:
    explicit RelDisp(const unsigned& = 0, const double& = 1E-8, const unsigned& = 7, const bool& = false);

    const bool& is_converged() override;
};

#endif

//! @}
