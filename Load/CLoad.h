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
 * @class CLoad
 * @brief A CLoad class.
 *
 * The CLoad class is in charge of handling concentrated load.
 *
 * @author T
 * @date 23/07/2017
 * @version 0.1.0
 * @file CLoad.h
 * @addtogroup Load
 * @{
 */

#ifndef CLOAD_H
#define CLOAD_H

#include "Load.h"

class CLoad : public Load {
public:
    explicit CLoad(const unsigned& = 0, // tag
        const unsigned& = 0,            // start step tag
        const double& = 0.,             // maginitude
        const uvec& = {},               // node tags
        const unsigned& = 0,            // dof tag
        const unsigned& = 0             // amplitude tag
    );
    CLoad(const unsigned&,  // tag
        const unsigned&,    // start step tag
        const double&,      // maginitude
        const uvec&,        // node tags
        const uvec&,        // dof tags
        const unsigned& = 0 // amplitude tag
    );

    int process(const shared_ptr<DomainBase>&) override;
};

#endif

//! @}
