/**
 * @class Workroom
 * @brief A Workroom class that holds global variables.
 *
 * The class is basically a container that contains global variables such as time, load,
 * resistance, displacement, velocity and acceleration. All the member variables are
 * exposed to outside of the class so users can directly access those variables. The class
 * provides several common methods to manipulate the status. The updateTrial*() and
 * updateIncre*() methods update the trial variables and incre variables simultaneously.
 * If one wants to update any specific variable, please consider to access the variable
 * directly instead of using a mutable method.
 *
 * @author T
 * @date 27/07/2017
 * @version 0.1.3
 * @file Workroom.h
 */

#ifndef WORKROOM_H
#define WORKROOM_H

#ifndef SUANPAN_NONE
#define SUANPAN_NONE 0 /**< Nothing */
#endif
#ifndef SUANPAN_DISP
#define SUANPAN_DISP 1 /**< Displacement Only */
#endif
#ifndef SUANPAN_BUCKLE
#define SUANPAN_BUCKLE 2 /**< Eigenvalue Eigenvector */
#endif
#ifndef SUANPAN_STATICS
#define SUANPAN_STATICS 3 /**< Displacement Load Resistance Matrices */
#endif
#ifndef SUANPAN_DYNAMICS
#define SUANPAN_DYNAMICS 4 /**< All */
#endif

#include <suanPan.h>

using T2 = mat; /**< alias */
// using T2 = sp_mat; /**< alias */

class Workroom final
{
    // friend class Solver;

    unsigned number_dof = 0;    /**< number of DoFs */
    unsigned analysis_type = 0; /**< type of analysis */
    unsigned low_bandwidth = 0; /**< low bandwidth */
    unsigned up_bandwidth = 0;  /**< up bandwidth */

    bool symm_mat = false; /**< symmetric matrix storage */
    bool band_mat = false; /**< banded matrix storage */

    double trial_time = 0.;   /**< global trial (pseudo) time */
    double incre_time = 0.;   /**< global incremental (pseudo) time */
    double current_time = 0.; /**< global current (pseudo) time */

    double error = 0.; /**< error produced by certain solvers */

    vec ninja; /**< the result from A*X=B */

    vec trial_load;         /**< global trial load vector */
    vec trial_resistance;   /**< global trial resistance vector */
    vec trial_displacement; /**< global trial displacement vector */
    vec trial_velocity;     /**< global trial velocity vector */
    vec trial_acceleration; /**< global trial acceleration vector */
    vec trial_temperature;  /**< global trial temperature vector */

    vec incre_load;         /**< global incremental load vector */
    vec incre_resistance;   /**< global incremental resistance vector */
    vec incre_displacement; /**< global incremental displacement vector */
    vec incre_velocity;     /**< global incremental velocity vector */
    vec incre_acceleration; /**< global incremental acceleration vector */
    vec incre_temperature;  /**< global incremental temperature vector */

    vec current_load;         /**< global current load vector */
    vec current_resistance;   /**< global current resistance vector */
    vec current_displacement; /**< global current displacement vector */
    vec current_velocity;     /**< global current velocity vector */
    vec current_acceleration; /**< global current acceleration vector */
    vec current_temperature;  /**< global current temperature vector */

    vec pre_load;         /**< global previous load vector */
    vec pre_resistance;   /**< global previous resistance vector */
    vec pre_displacement; /**< global previous displacement vector */
    vec pre_velocity;     /**< global previous velocity vector */
    vec pre_acceleration; /**< global previous acceleration vector */
    vec pre_temperature;  /**< global previous temperature vector */

    T2 global_mass;      /**< global mass matrix */
    T2 global_damping;   /**< global damping matrix */
    T2 global_stiffness; /**< global stiffness matrix */

    vec eigenvalue; /**< eigenvalues */

    mat eigenvector; /**< eigenvectors */
public:
    explicit Workroom(const unsigned& = 0, const unsigned& = SUANPAN_NONE);

    const bool& is_symm() const;
    const bool& is_band() const;

    void enable_symm();
    void disable_symm();

    void enable_band();
    void disable_band();

    void setNumberDOF(const unsigned&);
    const unsigned& getNumberDOF() const;

    void setAnalysisType(const unsigned&);
    const unsigned& getAnalysisType() const;

    void setBandwidth(const unsigned&, const unsigned&);
    void getBandwidth(unsigned&, unsigned&) const;

    void setError(const double&);
    const double& getError() const;

    void initialize(const unsigned& = 0);

    void initializeLoad(const unsigned&);
    void initializeResistance(const unsigned&);
    void initializeDisplacement(const unsigned&);
    void initializeVelocity(const unsigned&);
    void initializeAcceleration(const unsigned&);
    void initializeTemperature(const unsigned&);

    void initializeEigen(const unsigned&);
    void initializeMass(const unsigned&);
    void initializeDamping(const unsigned&);
    void initializeStiffness(const unsigned&);

    void updateNinja(const vec&);

    void updateTrialTime(const double&);
    void updateTrialLoad(const vec&);
    void updateTrialResistance(const vec&);
    void updateTrialDisplacement(const vec&);
    void updateTrialVelocity(const vec&);
    void updateTrialAcceleration(const vec&);
    void updateTrialTemperature(const vec&);

    void updateIncreTime(const double&);
    void updateIncreLoad(const vec&);
    void updateIncreResistance(const vec&);
    void updateIncreDisplacement(const vec&);
    void updateIncreVelocity(const vec&);
    void updateIncreAcceleration(const vec&);
    void updateIncreTemperature(const vec&);

    void updateCurrentTime(const double&);
    void updateCurrentLoad(const vec&);
    void updateCurrentResistance(const vec&);
    void updateCurrentDisplacement(const vec&);
    void updateCurrentVelocity(const vec&);
    void updateCurrentAcceleration(const vec&);
    void updateCurrentTemperature(const vec&);

    void updatePreviousLoad(const vec&);
    void updatePreviousResistance(const vec&);
    void updatePreviousDisplacement(const vec&);
    void updatePreviousVelocity(const vec&);
    void updatePreviousAcceleration(const vec&);
    void updatePreviousTemperature(const vec&);

    void updateMass(const T2&);
    void updateDamping(const T2&);
    void updateStiffness(const T2&);

    const vec& getNinja() const;

    const double& getTrialTime() const;
    const vec& getTrialLoad() const;
    const vec& getTrialResistance() const;
    const vec& getTrialDisplacement() const;
    const vec& getTrialVelocity() const;
    const vec& getTrialAcceleration() const;
    const vec& getTrialTemperature() const;

    const double& getIncreTime() const;
    const vec& getIncreLoad() const;
    const vec& getIncreResistance() const;
    const vec& getIncreDisplacement() const;
    const vec& getIncreVelocity() const;
    const vec& getIncreAcceleration() const;
    const vec& getIncreTemperature() const;

    const double& getCurrentTime() const;
    const vec& getCurrentLoad() const;
    const vec& getCurrentResistance() const;
    const vec& getCurrentDisplacement() const;
    const vec& getCurrentVelocity() const;
    const vec& getCurrentAcceleration() const;
    const vec& getCurrentTemperature() const;

    const vec& getPreviousLoad() const;
    const vec& getPreviousResistance() const;
    const vec& getPreviousDisplacement() const;
    const vec& getPreviousVelocity() const;
    const vec& getPreviousAcceleration() const;
    const vec& getPreviousTemperature() const;

    const T2& getMass() const;
    const T2& getDamping() const;
    const T2& getStiffness() const;

    const vec& getEigenvalue() const;
    const mat& getEigenvector() const;

    friend vec& getNinja(const shared_ptr<Workroom>&);

    friend double& getTrialTime(const shared_ptr<Workroom>&);
    friend vec& getTrialLoad(const shared_ptr<Workroom>&);
    friend vec& getTrialResistance(const shared_ptr<Workroom>&);
    friend vec& getTrialDisplacement(const shared_ptr<Workroom>&);
    friend vec& getTrialVelocity(const shared_ptr<Workroom>&);
    friend vec& getTrialAcceleration(const shared_ptr<Workroom>&);
    friend vec& getTrialTemperature(const shared_ptr<Workroom>&);

    friend double& getIncreTime(const shared_ptr<Workroom>&);
    friend vec& getIncreLoad(const shared_ptr<Workroom>&);
    friend vec& getIncreResistance(const shared_ptr<Workroom>&);
    friend vec& getIncreDisplacement(const shared_ptr<Workroom>&);
    friend vec& getIncreVelocity(const shared_ptr<Workroom>&);
    friend vec& getIncreAcceleration(const shared_ptr<Workroom>&);
    friend vec& getIncreTemperature(const shared_ptr<Workroom>&);

    friend double& getCurrentTime(const shared_ptr<Workroom>&);
    friend vec& getCurrentLoad(const shared_ptr<Workroom>&);
    friend vec& getCurrentResistance(const shared_ptr<Workroom>&);
    friend vec& getCurrentDisplacement(const shared_ptr<Workroom>&);
    friend vec& getCurrentVelocity(const shared_ptr<Workroom>&);
    friend vec& getCurrentAcceleration(const shared_ptr<Workroom>&);
    friend vec& getCurrentTemperature(const shared_ptr<Workroom>&);

    friend vec& getPreviousLoad(const shared_ptr<Workroom>&);
    friend vec& getPreviousResistance(const shared_ptr<Workroom>&);
    friend vec& getPreviousDisplacement(const shared_ptr<Workroom>&);
    friend vec& getPreviousVelocity(const shared_ptr<Workroom>&);
    friend vec& getPreviousAcceleration(const shared_ptr<Workroom>&);
    friend vec& getPreviousTemperature(const shared_ptr<Workroom>&);

    friend T2& getMass(const shared_ptr<Workroom>&);
    friend T2& getDamping(const shared_ptr<Workroom>&);
    friend T2& getStiffness(const shared_ptr<Workroom>&);

    friend vec& getEigenvalue(const shared_ptr<Workroom>&);
    friend mat& getEigenvector(const shared_ptr<Workroom>&);

    void commitStatus(const unsigned& = SUANPAN_STATICS);
    void commitTime();
    void commitLoad();
    void commitResistance();
    void commitDisplacement();
    void commitVelocity();
    void commitAcceleration();
    void commitTemperature();

    void commitPreStatus(const unsigned& = SUANPAN_STATICS);
    void commitPreLoad();
    void commitPreResistance();
    void commitPreDisplacement();
    void commitPreVelocity();
    void commitPreAcceleration();
    void commitPreTemperature();

    void clearStatus();
    void clearTime();
    void clearLoad();
    void clearResistance();
    void clearDisplacement();
    void clearVelocity();
    void clearAcceleration();
    void clearTemperature();

    void resetStatus();
    void resetTime();
    void resetLoad();
    void resetResistance();
    void resetDisplacement();
    void resetVelocity();
    void resetAcceleration();
    void resetTemperature();

    void clearEigen();
    void clearMass();
    void clearDamping();
    void clearStiffness();

    static void print();

    void assembleResistance(const mat&, const uvec&);
    void assembleMass(const mat&, const uvec&);
    void assembleDamping(const mat&, const uvec&);
    void assembleStiffness(const mat&, const uvec&);
};

vec& getNinja(const shared_ptr<Workroom>&);

double& getTrialTime(const shared_ptr<Workroom>&);
vec& getTrialLoad(const shared_ptr<Workroom>&);
vec& getTrialResistance(const shared_ptr<Workroom>&);
vec& getTrialDisplacement(const shared_ptr<Workroom>&);
vec& getTrialVelocity(const shared_ptr<Workroom>&);
vec& getTrialAcceleration(const shared_ptr<Workroom>&);
vec& getTrialTemperature(const shared_ptr<Workroom>&);

double& getIncreTime(const shared_ptr<Workroom>&);
vec& getIncreLoad(const shared_ptr<Workroom>&);
vec& getIncreResistance(const shared_ptr<Workroom>&);
vec& getIncreDisplacement(const shared_ptr<Workroom>&);
vec& getIncreVelocity(const shared_ptr<Workroom>&);
vec& getIncreAcceleration(const shared_ptr<Workroom>&);
vec& getIncreTemperature(const shared_ptr<Workroom>&);

double& getCurrentTime(const shared_ptr<Workroom>&);
vec& getCurrentLoad(const shared_ptr<Workroom>&);
vec& getCurrentResistance(const shared_ptr<Workroom>&);
vec& getCurrentDisplacement(const shared_ptr<Workroom>&);
vec& getCurrentVelocity(const shared_ptr<Workroom>&);
vec& getCurrentAcceleration(const shared_ptr<Workroom>&);
vec& getCurrentTemperature(const shared_ptr<Workroom>&);

vec& getPreviousLoad(const shared_ptr<Workroom>&);
vec& getPreviousResistance(const shared_ptr<Workroom>&);
vec& getPreviousDisplacement(const shared_ptr<Workroom>&);
vec& getPreviousVelocity(const shared_ptr<Workroom>&);
vec& getPreviousAcceleration(const shared_ptr<Workroom>&);
vec& getPreviousTemperature(const shared_ptr<Workroom>&);

T2& getMass(const shared_ptr<Workroom>&);
T2& getDamping(const shared_ptr<Workroom>&);
T2& getStiffness(const shared_ptr<Workroom>&);

vec& getEigenvalue(const shared_ptr<Workroom>&);
mat& getEigenvector(const shared_ptr<Workroom>&);

#endif
