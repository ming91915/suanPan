/**
 * @class BC
 * @brief A BC class handles boundary conditions.
 *
 * The BC class is in charge of applying boundary conditions to the system. The BC class
 * only takes care of homogeneous Dirichlet conditions. Non-homogeneous displacement
 * boundary conditions are treated as Load so that can be solved iteratively. Others are
 * handled by general constraint class such as MPC. The BC class stores the boundary
 * condition category, type, node(s) and corresponding DoF(s). The Domain invokes
 * `process(const shared_ptr<Domain>&)` method to modify the global stiffness matrix.
 *
 * @author T
 * @date 23/07/2017
 * @version 0.1.0
 * @file BC.h
 */

#ifndef BC_H
#define BC_H

#include "../Constraint.h"

class BC final : public Constraint
{
protected:
    uvec nodes; /**< node indices */
    uvec dofs;  /**< DoF indices */
public:
    explicit BC(const unsigned& = 0,
        const unsigned& = 0,
        const uvec& = {},
        const unsigned& = 0);
    BC(const unsigned&, const uvec&, const unsigned&);
    BC(const unsigned&, const unsigned&, const uvec&, const uvec&);
    BC(const unsigned&, const uvec&, const uvec&);
    BC(const unsigned&, const unsigned&, const uvec&, const char*);
    BC(const unsigned&, const uvec&, const char*);
    ~BC();

    const uvec& getNode() const;

    const uvec& getDOF() const;

    int process(const shared_ptr<Domain>&) override;
};

#endif
