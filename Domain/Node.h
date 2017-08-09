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

#include "Tag.h"

class Node final : public Tag
{
    unsigned num_dof = 0; /**< number of DoFs */

    vec coordinate; /**< coordinates of the node */

    uvec original_dof; /**< original indices */
    uvec reorder_dof;  /**< renumbered indices */

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

    const unsigned& getNumberDOF() const;

    const uvec& getOriginalDOF() const;
    const uvec& getReorderDOF() const;

    const vec& getCoordinate() const;

    const vec& getCurrentDisplacement() const;
    const vec& getCurrentVelocity() const;
    const vec& getCurrentAcceleration() const;

    const vec& getIncreDisplacement() const;
    const vec& getIncreVelocity() const;
    const vec& getIncreAcceleration() const;

    const vec& getTrialDisplacement() const;
    const vec& getTrialVelocity() const;
    const vec& getTrialAcceleration() const;

    void setNumberDOF(const unsigned&);

    void setOriginalDOF(unsigned&);
    void setOriginalDOF(const uvec&);

    void setReorderDOF(const uvec&);

    void setCoordinate(const vec&);

    void setCurrentDisplacement(const vec&);
    void setCurrentVelocity(const vec&);
    void setCurrentAcceleration(const vec&);

    void setIncreDisplacement(const vec&);
    void setIncreVelocity(const vec&);
    void setIncreAcceleration(const vec&);

    void setTrialDisplacement(const vec&);
    void setTrialVelocity(const vec&);
    void setTrialAcceleration(const vec&);

    void commitStatus();
    void resetStatus();
    void clearStatus();

    void updateTrialStatus(const vec&);
    void updateTrialStatus(const vec&, const vec&);
    void updateTrialStatus(const vec&, const vec&, const vec&);

    void updateIncreStatus(const vec&);
    void updateIncreStatus(const vec&, const vec&);
    void updateIncreStatus(const vec&, const vec&, const vec&);

    void print() override final;
};

#endif
