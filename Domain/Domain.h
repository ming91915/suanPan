/**
 * @class Domain
 * @brief A Domain class holds all FE model components.
 * @author T
 * @date 27/07/2017
 * @version 0.2.0
 * @file Domain.h
 */

#ifndef DOMAIN_H
#define DOMAIN_H

#include "Tag.h"

class BC;
class Constraint;
class Element;
class Load;
class Material;
class Node;
class Workroom;

class Domain : public Tag, public enable_shared_from_this<Domain>
{
    bool updated = false;

    shared_ptr<Workroom> factory; /**< working room */

    unordered_map<unsigned, shared_ptr<BC>> bc_pool;                 /**< data storage */
    unordered_map<unsigned, shared_ptr<Constraint>> constraint_pool; /**< data storage */
    unordered_map<unsigned, shared_ptr<Element>> element_pool;       /**< data storage */
    unordered_map<unsigned, shared_ptr<Load>> load_pool;             /**< data storage */
    unordered_map<unsigned, shared_ptr<Material>> material_pool;     /**< data storage */
    unordered_map<unsigned, shared_ptr<Node>> node_pool;             /**< data storage */

    vector<shared_ptr<Element>> tmp_element_pool;
    vector<shared_ptr<Node>> tmp_node_pool;

    unordered_set<unsigned> disabled_bc;         /**< data storage */
    unordered_set<unsigned> disabled_constraint; /**< data storage */
    unordered_set<unsigned> disabled_element;    /**< data storage */
    unordered_set<unsigned> disabled_load;       /**< data storage */
    unordered_set<unsigned> disabled_material;   /**< data storage */
    unordered_set<unsigned> disabled_node;       /**< data storage */

    unordered_set<unsigned> restrained_dofs;  /**< data storage */
    unordered_set<unsigned> constrained_dofs; /**< data storage */
    unordered_set<unsigned> loaded_dofs;      /**< data storage */
public:
    explicit Domain(const unsigned& = 0);
    ~Domain();

    const bool& is_updated() const;

    void initialize();

    void process(const unsigned& = 0);

    void setWorkroom(const shared_ptr<Workroom>& W);
    const shared_ptr<Workroom>& getWorkroom() const;

    void insert(const shared_ptr<BC>&);
    void insert(const shared_ptr<Constraint>&);
    void insert(const shared_ptr<Element>&);
    void insert(const shared_ptr<Load>&);
    void insert(const shared_ptr<Material>&);
    void insert(const shared_ptr<Node>&);

    void erase_bc(const unsigned&);
    void erase_constraint(const unsigned&);
    void erase_element(const unsigned&);
    void erase_load(const unsigned&);
    void erase_material(const unsigned&);
    void erase_node(const unsigned&);

    void disable_bc(const unsigned&);
    void disable_constraint(const unsigned&);
    void disable_element(const unsigned&);
    void disable_load(const unsigned&);
    void disable_material(const unsigned&);
    void disable_node(const unsigned&);

    const shared_ptr<BC>& getBC(const unsigned&) const;
    const shared_ptr<Constraint>& getConstraint(const unsigned&) const;
    const shared_ptr<Element>& getElement(const unsigned&) const;
    const shared_ptr<Load>& getLoad(const unsigned&) const;
    const shared_ptr<Material>& getMaterial(const unsigned&) const;
    const shared_ptr<Node>& getNode(const unsigned&) const;

    unsigned getNumberBC() const;
    unsigned getNumberConstraint() const;
    unsigned getNumberElement() const;
    unsigned getNumberLoad() const;
    unsigned getNumberMaterial() const;
    unsigned getNumberNode() const;

    void updateResistance() const;
    void updateMass() const;
    void updateInitialStiffness() const;
    void updateStiffness() const;
    void updateDamping() const;

    void updateTrialStatus() const;
    void updateIncreStatus() const;

    void commitStatus();
    void clearStatus();
    void resetStatus();

    bool insertLoadedDOF(const unsigned&);
    bool insertRestrainedDOF(const unsigned&);
    bool insertConstrainedDOF(const unsigned&);

    const unordered_set<unsigned>& getLoadedDOF() const;
    const unordered_set<unsigned>& getRestrainedDOF() const;
    const unordered_set<unsigned>& getConstrainedDOF() const;
};

#endif
