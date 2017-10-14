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
 * @brief The Mass class represents the point mass appiled at unique points.
 *
 * There are many different treatments to apply point mass to the system, for example, it can be deemed as external load, or could be an attribute of the target node.
 *
 * The first method is a common way, the acceleration can be converted to D'Alembert force then added to the right hand side of the system.
 *
 * But before applying the acceleration, the mass matrix should be formed. As the acceleration is also treated as external loads, and the program has no idea about the order of the loads.
 *
 * This simply means, the acceleration load can be applied before the mass load. This can only be resolved if the storage is ordered and the mass load is defined before acceleration load in the input file.
 *
 * Or it can be treated as a node attribute. It is not consistent with the logic concepts. Nodes are simply abstract points in space. It is alright to associate displacement, velocity, etc. with the node but the mass is a physical concept thus should be handle by the elemnt.
 *
 * Here we define mass as a special element, that only has mass matrix and optional damping matrix but no stiffness matrix. By such the matrices are formed readily before applying any external constraints or loads.
 *
 * @author T
 * @date 06/10/2017
 * @version 0.1.0
 * @file Mass.h
 * @addtogroup Element
 * @{
 */

#ifndef MASS_H
#define MASS_H

#include <Element/Element.h>

class Mass : public Element {
    const double magnitude;

    const uvec dof_label;

public:
    Mass(const unsigned&, const unsigned&, const double&, const uvec&);

    void initialize(const shared_ptr<DomainBase>&) override;

    int update_status() override;

    int commit_status() override;
    int clear_status() override;
    int reset_status() override;
};

#endif

//! @}
