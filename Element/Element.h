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
#include <Material/Material.h> // for derived class

class Domain;
class Material;
class Node;

class Element : public Tag
{
    const unsigned num_node; /**< number of nodes */
    const unsigned num_dof;  /**< number of DoFs */
protected:
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

    void update_dof_encoding();

    const unsigned& get_dof_number() const;
    const unsigned& get_node_number() const;
    const uvec& get_dof_encoding() const;
    const uvec& get_node_encoding() const;

    const vector<weak_ptr<Node>>& get_node_ptr() const;

    virtual const vec& get_resistance() const;
    virtual const mat& get_mass() const;
    virtual const mat& get_damping() const;
    virtual const mat& get_stiffness() const;
    virtual const mat& get_initial_stiffness() const;

    virtual int update_status() = 0;
    virtual int commit_status() = 0;
    virtual int clear_status() = 0;
    virtual int reset_status() = 0;

    virtual vector<vec> record(const OutputList&);
};

#endif
