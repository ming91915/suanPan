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
 * @class BC
 * @brief A BC class handles boundary conditions.
 *
 * The BC class is in charge of applying boundary conditions to the system. The BC class only takes care of homogeneous Dirichlet conditions. Non-homogeneous displacement boundary conditions are treated as Load so that can be solved iteratively. Others are handled by general constraint class such as MPC. The BC class stores the boundary condition category, type, node(s) and corresponding DoF(s). The Domain invokes `process(const shared_ptr<Domain>&)` method to modify the global stiffness matrix.
 *
 * @author T
 * @date 23/07/2017
 * @version 0.1.0
 * @file BC.h
 * @addtogroup Constraint
 * @{
 */

#ifndef BC_H
#define BC_H

#include <Constraint/Constraint.h>

class BC final : public Constraint {
protected:
    uvec nodes; /**< node indices */
    uvec dofs;  /**< DoF indices */
public:
    explicit BC(const unsigned& = 0, const unsigned& = 0, const uvec& = {}, const unsigned& = 0);
    BC(const unsigned&, const uvec&, const unsigned&);
    BC(const unsigned&, const unsigned&, const uvec&, const uvec&);
    BC(const unsigned&, const uvec&, const uvec&);
    BC(const unsigned&, const unsigned&, const uvec&, const char*);
    BC(const unsigned&, const uvec&, const char*);
    ~BC();

    const uvec& get_node() const;

    const uvec& get_dof() const;

    int process(const shared_ptr<DomainBase>&) override;
};

#endif

//! @}
