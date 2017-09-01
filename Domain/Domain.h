/**
* @class Domain
* @brief A Domain class holds all FE model components.
* @author T
* @date 27/07/2017
* @version 0.2.0
* @file Domain.h
* @{
*/

#ifndef DOMAIN_H
#define DOMAIN_H

#include "Tag.h"
#include <unordered_map>
#include <unordered_set>
#include <vector>

class Workshop;
class Constraint;
class Element;
class Load;
class Material;
class Node;
class Recorder;

using std::unordered_map;
using std::unordered_set;
using std::vector;

using std::enable_shared_from_this;

class Domain : public Tag, public enable_shared_from_this<Domain>
{
    bool updated = false;
    bool initialized = false;

    shared_ptr<Workshop> workroom; /**< working room */

    unordered_map<unsigned, shared_ptr<Constraint>> constraint_pool; /**< data storage */
    unordered_map<unsigned, shared_ptr<Element>> element_pool;       /**< data storage */
    unordered_map<unsigned, shared_ptr<Load>> load_pool;             /**< data storage */
    unordered_map<unsigned, shared_ptr<Material>> material_pool;     /**< data storage */
    unordered_map<unsigned, shared_ptr<Node>> node_pool;             /**< data storage */
    unordered_map<unsigned, shared_ptr<Recorder>> recorder_pool;     /**< data storage */

    vector<shared_ptr<Constraint>> tmp_constraint_pool;
    vector<shared_ptr<Element>> tmp_element_pool;
    vector<shared_ptr<Load>> tmp_load_pool;
    vector<shared_ptr<Node>> tmp_node_pool;

    unordered_set<unsigned> disabled_constraint; /**< data storage */
    unordered_set<unsigned> disabled_element;    /**< data storage */
    unordered_set<unsigned> disabled_load;       /**< data storage */
    unordered_set<unsigned> disabled_material;   /**< data storage */
    unordered_set<unsigned> disabled_node;       /**< data storage */
    unordered_set<unsigned> disabled_recorder;   /**< data storage */

    unordered_set<unsigned> restrained_dofs;  /**< data storage */
    unordered_set<unsigned> constrained_dofs; /**< data storage */
    unordered_set<unsigned> loaded_dofs;      /**< data storage */
public:
    explicit Domain(const unsigned& = 0);
    ~Domain();

    const bool& is_updated() const;
    const bool& is_initialized() const;

    int initialize();

    void process(const unsigned& = 0);

    void record();

    void set_workshop(const shared_ptr<Workshop>&);
    const shared_ptr<Workshop>& get_workshop() const;

    bool insert(const shared_ptr<Constraint>&);
    bool insert(const shared_ptr<Element>&);
    bool insert(const shared_ptr<Load>&);
    bool insert(const shared_ptr<Material>&);
    bool insert(const shared_ptr<Node>&);
    bool insert(const shared_ptr<Recorder>&);

    void erase_constraint(const unsigned&);
    void erase_element(const unsigned&);
    void erase_load(const unsigned&);
    void erase_material(const unsigned&);
    void erase_node(const unsigned&);
    void erase_recorder(const unsigned&);

    void disable_constraint(const unsigned&);
    void disable_element(const unsigned&);
    void disable_load(const unsigned&);
    void disable_material(const unsigned&);
    void disable_node(const unsigned&);
    void disable_recorder(const unsigned&);

    void enable_all();

    void enable_constraint(const unsigned&);
    void enable_element(const unsigned&);
    void enable_load(const unsigned&);
    void enable_material(const unsigned&);
    void enable_node(const unsigned&);
    void enable_recorder(const unsigned&);

    const shared_ptr<Constraint>& get_constraint(const unsigned&) const;
    const shared_ptr<Element>& get_element(const unsigned&) const;
    const shared_ptr<Load>& get_load(const unsigned&) const;
    const shared_ptr<Material>& get_material(const unsigned&) const;
    const shared_ptr<Node>& get_node(const unsigned&) const;
    const shared_ptr<Recorder>& get_recorder(const unsigned&) const;

    friend shared_ptr<Constraint>& get_constraint(const shared_ptr<Domain>&,
        const unsigned&);
    friend shared_ptr<Element>& get_element(const shared_ptr<Domain>&, const unsigned&);
    friend shared_ptr<Load>& get_load(const shared_ptr<Domain>&, const unsigned&);
    friend shared_ptr<Material>& get_material(const shared_ptr<Domain>&, const unsigned&);
    friend shared_ptr<Node>& get_node(const shared_ptr<Domain>&, const unsigned&);
    friend shared_ptr<Recorder>& get_recorder(const shared_ptr<Domain>&, const unsigned&);

    unsigned get_constraint() const;
    unsigned get_element() const;
    unsigned get_load() const;
    unsigned get_material() const;
    unsigned get_node() const;
    unsigned get_recorder() const;

    bool find_constraint(const unsigned&) const;
    bool find_element(const unsigned&) const;
    bool find_load(const unsigned&) const;
    bool find_material(const unsigned&) const;
    bool find_node(const unsigned&) const;
    bool find_recorder(const unsigned&) const;

    friend const vector<shared_ptr<Node>>& get_node_pool(const shared_ptr<Domain>&);

    void update_resistance() const;
    void update_mass() const;
    void update_initial_stiffness() const;
    void update_stiffness() const;
    void update_damping() const;

    void update_trial_time(const double&) const;
    void update_incre_time(const double&) const;
    void update_trial_status() const;
    void update_incre_status() const;

    void commit_status();
    void clear_status();
    void reset_status();

    bool insert_loaded_dof(const unsigned&);
    bool insert_restrained_dof(const unsigned&);
    bool insert_constrained_dof(const unsigned&);

    const unordered_set<unsigned>& get_loaded_dof() const;
    const unordered_set<unsigned>& get_restrained_dof() const;
    const unordered_set<unsigned>& get_constrained_dof() const;

    void summary() const;
};

#endif

//! @}
