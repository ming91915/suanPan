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
 * @class AbsIncreDisp
 * @brief The AbsIncreDisp class that handles converger test to indicate if the iteration converges.
 * @author T
 * @date 27/07/2017
 * @version 0.1.0
 * @file AbsIncreDisp.h
 * @addtogroup Converger
 * @{
 */

#ifndef ABSINCREDISP_H
#define ABSINCREDISP_H

#include "Converger.h"

class AbsIncreDisp : public Converger {
public:
    explicit AbsIncreDisp(const unsigned& = 0, const shared_ptr<DomainBase>& = nullptr, const double& = 1E-8, const unsigned& = 7, const bool& = false);
    explicit AbsIncreDisp(const shared_ptr<DomainBase>&, const double& = 1E-8, const unsigned& = 7, const bool& = false);
    explicit AbsIncreDisp(const double&, const unsigned& = 7, const bool& = false);

    const bool& if_converged() override;
};

#endif

//! @}
