/**
 * @class Node
 * @brief The Node class holds the number of DoFs, coordinate, displacement, velocity and
 * acceleration.
 *
 * The current/committed, incremental and trial status of displacement, velocity and
 * acceleration are stored. These variables will be the communication bridge(s) between
 * Domain, Workroom and Element objects. That is, Element objects do not directly get
 * information from the Workroom. Instead, Workroom passes information to Node objects
 * through the Domain, Element objects acquire new status from associated Node objects
 * only. In this manner, the relationship between those modules remains simple.
 *
 * @author T
 * @date 22/07/2017
 * @version 0.2.0
 * @file Node.h
 */

#ifndef NODE_H
#define NODE_H

#include <Domain/Tag.h>
#include <Recorder/OutputList.h>

using std::vector;

class Node final : public Tag
{
    unsigned num_dof = 0; /**< number of DoFs */

    vec coordinate; /**< coordinates of the node */

    uvec original_dof;  /**< original indices */
    uvec reordered_dof; /**< renumbered indices */

    vec current_displacement; /**< current displacement */
    vec current_velocity;     /**< current velocity */
    vec current_acceleration; /**< current acceleration */

    vec incre_displacement; /**< incremental displacement */
    vec incre_velocity;     /**< incremental velocity */
    vec incre_acceleration; /**< incremental acceleration */

    vec trial_displacement; /**< trial displacement */
    vec trial_velocity;     /**< trial velocity */
    vec trial_acceleration; /**< trial acceleration */

public:
    explicit Node(const unsigned& = 0);
    Node(const unsigned&, const vec&);
    Node(const unsigned&, const unsigned&);
    Node(const unsigned&, const unsigned&, const vec&);
    ~Node();

    void initialize();

    const unsigned& get_dof_number() const;

    const uvec& get_original_dof() const;
    const uvec& get_reordered_dof() const;

    const vec& get_coordinate() const;

    const vec& get_current_displacement() const;
    const vec& get_current_velocity() const;
    const vec& get_current_acceleration() const;

    const vec& get_incre_displacement() const;
    const vec& get_incre_velocity() const;
    const vec& get_incre_acceleration() const;

    const vec& get_trial_displacement() const;
    const vec& get_trial_velocity() const;
    const vec& get_trial_acceleration() const;

    void set_dof_number(const unsigned&);

    void set_original_dof(unsigned&);
    void set_original_dof(const uvec&);

    void set_reordered_dof(const uvec&);

    void set_coordinate(const vec&);

    void set_current_displacement(const vec&);
    void set_current_velocity(const vec&);
    void set_current_acceleration(const vec&);

    void set_incre_displacement(const vec&);
    void set_incre_velocity(const vec&);
    void set_incre_acceleration(const vec&);

    void set_trial_displacement(const vec&);
    void set_trial_velocity(const vec&);
    void set_trial_acceleration(const vec&);

    void commit_status();
    void reset_status();
    void clear_status();

    void update_trial_status(const vec&);
    void update_trial_status(const vec&, const vec&);
    void update_trial_status(const vec&, const vec&, const vec&);

    void update_incre_status(const vec&);
    void update_incre_status(const vec&, const vec&);
    void update_incre_status(const vec&, const vec&, const vec&);

    vector<vec> record(const OutputList&) const;

    void print() override final;
};

#endif
