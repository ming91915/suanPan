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
 * @class Mass
 * @brief A Mass class.
 *
 * The Mass class is in charge of handling concentrated load.
 *
 * @author T
 * @date 04/10/2017
 * @version 0.1.0
 * @file Mass.h
 * @addtogroup Load
 * @{
 */

#ifndef MASS_H
#define MASS_H

#include "Load.h"

class Mass : public Load {
public:
    explicit Mass(const unsigned& = 0, // tag
        const unsigned& = 0,           // start step tag
        const double& = 0.,            // maginitude
        const uvec& = {},              // node tags
        const unsigned& = 0            // dof tag
    );

    int process(const shared_ptr<DomainBase>&) override;
};

#endif

//! @}
