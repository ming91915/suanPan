/**
 * @class Element
 * @brief A Element class.
 * @author T
 * @date 21/07/2017
 * @version 0.1.0
 * @file Element.h
 * @addtogroup Element
 * @{
 */

#ifndef ELEMENT_H
#define ELEMENT_H

#include <Domain/Domain.h>
#include <Domain/Node.h>
#include <Domain/Tag.h>
#include <Material/Material.h>

class Domain;
class Material;
class Node;

class Element : public Tag
{
protected:
    unsigned num_node = 0; /**< number of nodes */
    unsigned num_dof = 0;  /**< number of DoFs */

    uvec node_encoding; /**< node encoding vector */
    uvec material_tag;  /**< material tags */

    bool nlgeom = false; /**< nonlinear geometry switch */

    uvec dof_encoding; /**< DoF encoding vector */

    vec resistance; /**< resistance vector. */

    mat mass;              /**< mass matrix */
    mat damping;           /**< damping matrix */
    mat stiffness;         /**< stiffness matrix */
    mat initial_stiffness; /**< initial stiffness matrix */

    vector<weak_ptr<Node>> node_ptr; /**< node pointers */
public:
    explicit Element(const unsigned& = 0,
        const unsigned& = CT_ELEMENT,
        const unsigned& = 0,
        const unsigned& = 0,
        const uvec& = {},
        const uvec& = {},
        const bool& = false);
    Element(const Element&) = delete;
    Element& operator=(const Element&) = delete;

    virtual ~Element();

    virtual void initialize(const shared_ptr<Domain>&) = 0;

    void updateEncodingDOF();

    const unsigned& getNumberDOF() const;
    const unsigned& getNumberNode() const;
    const uvec& getEncodingDOF() const;
    const uvec& getEncodingNode() const;

    const vector<weak_ptr<Node>>& getNodePtr() const;

    virtual const vec& getResistance() const;
    virtual const mat& getMass() const;
    virtual const mat& getDamping() const;
    virtual const mat& getStiffness() const;
    virtual const mat& getInitialStiffness() const;

    virtual int updateStatus() = 0;
    virtual int commitStatus() = 0;
    virtual int clearStatus() = 0;
    virtual int resetStatus() = 0;
};

#endif
