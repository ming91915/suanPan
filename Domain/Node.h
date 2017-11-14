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
 * @class Node
 * @brief The Node class holds the number of DoFs, coordinate, displacement, velocity and acceleration.
 *
 * The current/committed, incremental and trial status of displacement, velocity and acceleration are stored. These variables will be the communication bridge(s) between Domain, Workshop and Element objects. That is, Element objects do not directly get information from the Workshop. Instead, Workshop passes information to Node objects through the Domain, Element objects acquire new status from associated Node objects only. In this manner, the relationship between those modules remains simple.
 *
 * @author T
 * @date 22/07/2017
 * @version 0.2.0
 * @file Node.h
 * @addtogroup Domain
 * @{
 */

#ifndef NODE_H
#define NODE_H

#include <Domain/Tag.h>

using std::vector;

class DomainBase;
enum class OutputType;

class Node final : public Tag {
    bool initialized = false;

    unsigned num_dof = 0; /**< number of DoFs */

    vec coordinate; /**< coordinates of the node */

    uvec original_dof;  /**< original indices */
    uvec reordered_dof; /**< renumbered indices */

    vec current_resistance;
    vec current_displacement; /**< current displacement */
    vec current_velocity;     /**< current velocity */
    vec current_acceleration; /**< current acceleration */

    vec incre_resistance;
    vec incre_displacement; /**< incremental displacement */
    vec incre_velocity;     /**< incremental velocity */
    vec incre_acceleration; /**< incremental acceleration */

    vec trial_resistance;
    vec trial_displacement; /**< trial displacement */
    vec trial_velocity;     /**< trial velocity */
    vec trial_acceleration; /**< trial acceleration */

public:
    explicit Node(const unsigned& = 0);
    Node(const unsigned&, const vec&);
    Node(const unsigned&, const unsigned&);
    Node(const unsigned&, const unsigned&, const vec&);
    Node(const Node&) = delete;            // copy forbidden
    Node(Node&&) = delete;                 // move forbidden
    Node& operator=(const Node&) = delete; // assign forbidden
    Node& operator=(Node&&) = delete;      // assign forbidden
    ~Node();

    void initialize(const shared_ptr<DomainBase>&);

    void set_dof_number(const unsigned&);
    const unsigned& get_dof_number() const;

    void set_original_dof(unsigned&);
    void set_original_dof(const uvec&);
    const uvec& get_original_dof() const;

    void set_reordered_dof(const uvec&);
    const uvec& get_reordered_dof() const;

    void set_coordinate(const vec&);
    const vec& get_coordinate() const;

    void set_current_resistance(const vec&);
    void set_current_displacement(const vec&);
    void set_current_velocity(const vec&);
    void set_current_acceleration(const vec&);

    void set_incre_resistance(const vec&);
    void set_incre_displacement(const vec&);
    void set_incre_velocity(const vec&);
    void set_incre_acceleration(const vec&);

    void set_trial_resistance(const vec&);
    void set_trial_displacement(const vec&);
    void set_trial_velocity(const vec&);
    void set_trial_acceleration(const vec&);

    const vec& get_current_resistance() const;
    const vec& get_current_displacement() const;
    const vec& get_current_velocity() const;
    const vec& get_current_acceleration() const;

    const vec& get_incre_resistance() const;
    const vec& get_incre_displacement() const;
    const vec& get_incre_velocity() const;
    const vec& get_incre_acceleration() const;

    const vec& get_trial_resistance() const;
    const vec& get_trial_displacement() const;
    const vec& get_trial_velocity() const;
    const vec& get_trial_acceleration() const;

    void update_current_resistance(const vec&);
    void update_incre_resistance(const vec&);
    void update_trial_resistance(const vec&);

    void update_current_status(const vec&);
    void update_current_status(const vec&, const vec&);
    void update_current_status(const vec&, const vec&, const vec&);

    void update_incre_status(const vec&);
    void update_incre_status(const vec&, const vec&);
    void update_incre_status(const vec&, const vec&, const vec&);

    void update_trial_status(const vec&);
    void update_trial_status(const vec&, const vec&);
    void update_trial_status(const vec&, const vec&, const vec&);

    void commit_status();
    void reset_status();
    void clear_status();

    vector<vec> record(const OutputType&) const;

    void print() override;
};

#endif

//! @}
