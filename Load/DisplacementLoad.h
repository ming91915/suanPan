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
 * @class DisplacementLoad
 * @brief A DisplacementLoad class.
 *
 * The DisplacementLoad class is in charge of handling displacement load.
 *
 * @author T
 * @date 17/09/2017
 * @version 0.1.0
 * @file DisplacementLoad.h
 * @addtogroup Load
 * @{
 */

#ifndef DISPLACEMENTLOAD_H
#define DISPLACEMENTLOAD_H

#include "Load.h"

class DisplacementLoad : public Load {
    double pattern = 0.;

    uvec nodes;
    uvec dofs;

public:
    explicit DisplacementLoad(const unsigned& = 0, // tag
        const unsigned& = 0,                       // step tag
        const double& = 0.,                        // magnitude
        const uvec& = {},                          // node tags
        const unsigned& = 0,                       // dof tag
        const unsigned& = 0);                      // amplitude tag
    DisplacementLoad(const unsigned&,              // tag
        const unsigned&,                           // step tag
        const double&,                             // magnitude
        const uvec&,                               // node tags
        const uvec&,                               // dof tags
        const unsigned& = 0);                      // amplitude tag

    int process(const shared_ptr<DomainBase>&) override;
};

#endif // DISPLACEMENTLOAD_H

//! @}
