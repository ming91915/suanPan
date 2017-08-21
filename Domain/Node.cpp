#include "Node.h"

/**
 * \brief Empty constructor.
 * \param T `unique_tag`
 */
Node::Node(const unsigned& T)
    : Tag(T, CT_NODE)
{
    suanpan_debug("Node %u ctor() called.\n", T);
}

/**
 * \brief Initialize `coordinate` and set `num_dof` to the size of `coordinate`.
 * \param T `unique_tag`
 * \param C `coordinate`
 */
Node::Node(const unsigned& T, const vec& C)
    : Tag(T, CT_NODE)
    , num_dof(static_cast<unsigned>(C.n_elem))
    , coordinate(C)
{
    suanpan_debug("Node %u ctor() called.\n", T);
}

/**
 * \brief Initialize `num_dof` and set the size of `coordinate` to `num_dof`.
 * \param T `unique_tag`
 * \param D `num_dof`
 */
Node::Node(const unsigned& T, const unsigned& D)
    : Tag(T, CT_NODE)
    , num_dof(D)
    , coordinate(num_dof)
{
    suanpan_debug("Node %u ctor() called.\n", T);
}

/**
 * \brief Initialize `num_dof` and `coordinate`.
 * \param T `unique_tag`
 * \param D `num_dof`
 * \param C `coordinate`
 */
Node::Node(const unsigned& T, const unsigned& D, const vec& C)
    : Tag(T, CT_NODE)
    , num_dof(D)
    , coordinate(C)
{
    suanpan_debug("Node %u ctor() called.\n", T);
}

/**
 * \brief Default destructor.
 */
Node::~Node() { suanpan_debug("Node %u dtor() called.\n", getTag()); }

/**
 * \brief This method should be called after Element objects are set. Element objects will
 * set the minimum number of DoFs for all related Node objects. This method initialize all
 * member variables with the size of `num_dof` and fill `original_dof` with `-1` to
 * indicated it should be omitted from the system. Finally check if the size of
 * `coordinate` is the same of `num_dof`, if not, resize it to `num_dof`. This will be
 * necessary for beam/plate/shell problems which have more DoFs than coordinates.
 */
void Node::initialize()
{
    if(getStatus()) {
        if(num_dof != 0) {
            original_dof.zeros(num_dof);
            original_dof.fill(-1);

            reorder_dof.reset();

            if(current_displacement.is_empty())
                current_displacement.zeros(num_dof);
            else
                current_displacement.resize(num_dof);

            if(current_velocity.is_empty())
                current_velocity.zeros(num_dof);
            else
                current_velocity.resize(num_dof);

            if(current_acceleration.is_empty())
                current_acceleration.zeros(num_dof);
            else
                current_acceleration.resize(num_dof);

            if(incre_displacement.is_empty())
                incre_displacement.zeros(num_dof);
            else
                incre_displacement.resize(num_dof);

            if(incre_velocity.is_empty())
                incre_velocity.zeros(num_dof);
            else
                incre_velocity.resize(num_dof);

            if(incre_acceleration.is_empty())
                incre_acceleration.zeros(num_dof);
            else
                incre_acceleration.resize(num_dof);

            if(trial_displacement.is_empty())
                trial_displacement.zeros(num_dof);
            else
                trial_displacement.resize(num_dof);

            if(trial_velocity.is_empty())
                trial_velocity.zeros(num_dof);
            else
                trial_velocity.resize(num_dof);

            if(trial_acceleration.is_empty())
                trial_acceleration.zeros(num_dof);
            else
                trial_acceleration.resize(num_dof);

            // if(num_dof > coordinate.n_elem) coordinate.resize(num_dof);
        } else {
            printf("Node %u is not used in the problem, now disable it.\n", getTag());
            disable();
        }
    }
}

/**
 * \brief Method to return `num_dof`.
 * \return `num_dof`
 */
const unsigned& Node::getNumberDOF() const { return num_dof; }

/**
 * \brief Method to return `original_dof`.
 * \return `original_dof`
 */
const uvec& Node::getOriginalDOF() const { return original_dof; }

/**
 * \brief Method to return `reorder_dof`.
 * \return `reorder_dof`
 */
const uvec& Node::getReorderDOF() const { return reorder_dof; }

/**
 * \brief Method to return `coordinate`.
 * \return `coordinate`
 */
const vec& Node::getCoordinate() const { return coordinate; }

/**
 * \brief Method to return `current_displacement`.
 * \return `current_displacement`
 */
const vec& Node::getCurrentDisplacement() const { return current_displacement; }

/**
 * \brief Method to return `current_velocity`.
 * \return `current_velocity`
 */
const vec& Node::getCurrentVelocity() const { return current_velocity; }

/**
 * \brief Method to return `current_acceleration`.
 * \return `current_acceleration`
 */
const vec& Node::getCurrentAcceleration() const { return current_acceleration; }

/**
 * \brief Method to return `incre_displacement`.
 * \return `incre_displacement`
 */
const vec& Node::getIncreDisplacement() const { return incre_displacement; }

/**
 * \brief Method to return `incre_velocity`.
 * \return `incre_velocity`
 */
const vec& Node::getIncreVelocity() const { return incre_velocity; }

/**
 * \brief Method to return `incre_acceleration`.
 * \return `incre_acceleration`
 */
const vec& Node::getIncreAcceleration() const { return incre_acceleration; }

/**
 * \brief Method to return `trial_displacement`.
 * \return `trial_displacement`
 */
const vec& Node::getTrialDisplacement() const { return trial_displacement; }

/**
 * \brief Method to return `trial_velocity`.
 * \return `trial_velocity`
 */
const vec& Node::getTrialVelocity() const { return trial_velocity; }

/**
 * \brief Method to return `trial_acceleration`.
 * \return `trial_acceleration`
 */
const vec& Node::getTrialAcceleration() const { return trial_acceleration; }

/**
 * \brief Method to set `num_dof`.
 * \param D `num_dof`
 */
void Node::setNumberDOF(const unsigned& D) { num_dof = D; }

/**
 * \brief Method to set `original_dof`.
 * \param F Current Index Counter
 */
void Node::setOriginalDOF(unsigned& F)
{
    if(getStatus())
        if(original_dof.n_elem == num_dof)
            for(unsigned I = 0; I < num_dof; ++I) original_dof(I) = F++;
}

/**
 * \brief Method to set `original_dof`.
 * \param D `original_dof`
 */
void Node::setOriginalDOF(const uvec& D) { original_dof = D; }

/**
 * \brief Method to set `reorder_dof`.
 * \param R `reorder_dof`
 */
void Node::setReorderDOF(const uvec& R) { reorder_dof = R; }

/**
 * \brief Method to set `coordinate`.
 * \param C `coordinate`
 */
void Node::setCoordinate(const vec& C) { coordinate = C; }

/**
 * \brief Method to set variable independently.
 * \param D `current_displacement`
 */
void Node::setCurrentDisplacement(const vec& D) { current_displacement = D; }

/**
 * \brief Method to set variable independently.
 * \param V `current_velocity`
 */
void Node::setCurrentVelocity(const vec& V) { current_velocity = V; }

/**
 * \brief Method to set variable independently.
 * \param A `current_acceleration`
 */
void Node::setCurrentAcceleration(const vec& A) { current_acceleration = A; }

/**
 * \brief Method to set variable independently.
 * \param D `incre_displacement`
 */
void Node::setIncreDisplacement(const vec& D) { incre_displacement = D; }

/**
 * \brief Method to set variable independently.
 * \param V `incre_velocity`
 */
void Node::setIncreVelocity(const vec& V) { incre_velocity = V; }

/**
 * \brief Method to set variable independently.
 * \param A `incre_acceleration`
 */
void Node::setIncreAcceleration(const vec& A) { incre_acceleration = A; }

/**
 * \brief Method to set variable independently.
 * \param D `trial_displacement`
 */
void Node::setTrialDisplacement(const vec& D) { trial_displacement = D; }

/**
 * \brief Method to set variable independently.
 * \param V `trial_velocity`
 */
void Node::setTrialVelocity(const vec& V) { trial_velocity = V; }

/**
 * \brief Method to set variable independently.
 * \param A `trial_acceleration`
 */
void Node::setTrialAcceleration(const vec& A) { trial_acceleration = A; }

/**
 * \brief Method to commit the status variables.
 */
void Node::commitStatus()
{
    if(!trial_displacement.is_empty()) current_displacement = trial_displacement;
    if(!trial_velocity.is_empty()) current_velocity = trial_velocity;
    if(!trial_acceleration.is_empty()) current_acceleration = trial_acceleration;
}

/**
 * \brief Method to reset the status, there is no need to call this method as those
 * variables can be directly overwritten.
 */
void Node::resetStatus()
{
    if(!current_displacement.is_empty()) {
        trial_displacement = current_displacement;
        incre_displacement.zeros();
    }
    if(!current_velocity.is_empty()) {
        trial_velocity = current_velocity;
        incre_velocity.zeros();
    }
    if(!current_acceleration.is_empty()) {
        trial_acceleration = current_acceleration;
        incre_acceleration.zeros();
    }
}

/**
* \brief The method tests each status variable before filling it by zeros. For any of
* them, empty means it is not used in analysis, then just keeps it unchanged.
*/
void Node::clearStatus()
{
    if(!current_displacement.is_empty()) {
        current_displacement.zeros();
        incre_displacement.zeros();
        trial_displacement.zeros();
    }
    if(!current_velocity.is_empty()) {
        current_velocity.zeros();
        incre_velocity.zeros();
        trial_velocity.zeros();
    }
    if(!current_acceleration.is_empty()) {
        current_acceleration.zeros();
        incre_acceleration.zeros();
        trial_acceleration.zeros();
    }
}

/**
 * \brief Method to update displacement.
 * \param D `trial_displacement`
 */
void Node::updateTrialStatus(const vec& D)
{
    for(unsigned I = 0; I < num_dof; I++) trial_displacement(I) = D(reorder_dof(I));
    // trial_displacement = D;
    incre_displacement = trial_displacement - current_displacement;
}

/**
 * \brief Method to update velocity and call previous method to further update
 * displacement. This is used in Dynamic analysis.
 * \param D `trial_displacement`
 * \param V `trial_velocity`
 */
void Node::updateTrialStatus(const vec& D, const vec& V)
{
    for(unsigned I = 0; I < num_dof; I++) trial_velocity(I) = V(reorder_dof(I));
    // trial_velocity = V;
    incre_velocity = trial_velocity - current_velocity;
    updateTrialStatus(D);
}

/**
 * \brief Method to update acceleration and call previous method to further update
 * velocity and displacement. This is used in Dynamic analysis.
 * \param D `trial_displacement`
 * \param V `trial_velocity`
 * \param A `trial_acceleration`
 */
void Node::updateTrialStatus(const vec& D, const vec& V, const vec& A)
{
    for(unsigned I = 0; I < num_dof; I++) trial_acceleration(I) = A(reorder_dof(I));
    // trial_acceleration = A;
    incre_acceleration = trial_acceleration - current_acceleration;
    updateTrialStatus(D, V);
}

/**
 * \brief Method to update displacement.
 * \param D `incre_displacement`
 */
void Node::updateIncreStatus(const vec& D)
{
    for(unsigned I = 0; I < num_dof; I++) incre_displacement(I) = D(reorder_dof(I));
    // incre_displacement = D;
    trial_displacement = current_displacement + incre_displacement;
}

/**
 * \brief Method to update velocity and call previous method to further update
 * displacement. This is used in Dynamic analysis.
 * \param D `incre_displacement`
 * \param V `incre_velocity`
 */
void Node::updateIncreStatus(const vec& D, const vec& V)
{
    for(unsigned I = 0; I < num_dof; I++) incre_velocity(I) = V(reorder_dof(I));
    // incre_velocity = V;
    trial_velocity = current_velocity + incre_velocity;
    updateIncreStatus(D);
}

/**
 * \brief Method to update acceleration and call previous method to further update
 * velocity and displacement. This is used in Dynamic analysis.
 * \param D `incre_displacement`
 * \param V `incre_velocity`
 * \param A `incre_acceleration`
 */
void Node::updateIncreStatus(const vec& D, const vec& V, const vec& A)
{
    for(unsigned I = 0; I < num_dof; I++) incre_acceleration(I) = A(reorder_dof(I));
    // incre_acceleration = A;
    trial_acceleration = current_acceleration + incre_acceleration;
    updateIncreStatus(D, V);
}

/**
 * \brief Method to print basic information.
 */
void Node::print()
{
    suanpan_info("Node %u:\n", getTag());
    coordinate.print();
    suanpan_info("Displacement:\n");
    current_displacement.print();
}
