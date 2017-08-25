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

enum class AnalysisType { NONE, DISP, EIGEN, STATICS, DYNAMICS };

#include <suanPan.h>

using T2 = mat; /**< alias */

class Workroom final
{
    // friend class Solver;

    bool initialized = false;

    bool symm_mat = false; /**< symmetric matrix storage */
    bool band_mat = false; /**< banded matrix storage */

    unsigned dof_number = 0; /**< number of DoFs */

    AnalysisType analysis_type = AnalysisType::NONE; /**< type of analysis */

    unsigned low_bandwidth = 0; /**< low bandwidth */
    unsigned up_bandwidth = 0;  /**< up bandwidth */
    unsigned shifted_bandwidth = 0;

    double trial_time = 0.;   /**< global trial (pseudo) time */
    double incre_time = 0.;   /**< global incremental (pseudo) time */
    double current_time = 0.; /**< global current (pseudo) time */
    double pre_time = 0.;     /**< global previous (pseudo) time */

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
    explicit Workroom(const unsigned& = 0, const AnalysisType& = AnalysisType::NONE);

    const bool& is_symm() const;
    const bool& is_band() const;

    void enable_symm();
    void disable_symm();

    void enable_band();
    void disable_band();

    void set_dof_number(const unsigned&);
    const unsigned& get_dof_number() const;

    void set_analysis_type(const AnalysisType&);
    const AnalysisType& get_analysis_type() const;

    void set_bandwidth(const unsigned&, const unsigned&);
    void get_bandwidth(unsigned&, unsigned&) const;

    void set_error(const double&);
    const double& get_error() const;

    const bool& is_initialized() const;

    int initialize();

    void initialize_load(const unsigned&);
    void initialize_resistance(const unsigned&);
    void initialize_displacement(const unsigned&);
    void initialize_velocity(const unsigned&);
    void initialize_acceleration(const unsigned&);
    void initialize_temperature(const unsigned&);

    void initialize_eigen(const unsigned&);
    void initialize_mass(const unsigned&);
    void initialize_damping(const unsigned&);
    void initialize_stiffness(const unsigned&);

    void update_ninja(const vec&);

    void update_trial_time(const double&);
    void update_trial_load(const vec&);
    void update_trial_resistance(const vec&);
    void update_trial_displacement(const vec&);
    void update_trial_velocity(const vec&);
    void update_trial_acceleration(const vec&);
    void update_trial_temperature(const vec&);

    void update_incre_time(const double&);
    void update_incre_load(const vec&);
    void update_incre_resistance(const vec&);
    void update_incre_displacement(const vec&);
    void update_incre_velocity(const vec&);
    void update_incre_acceleration(const vec&);
    void update_incre_temperature(const vec&);

    void update_current_time(const double&);
    void update_current_load(const vec&);
    void update_current_resistance(const vec&);
    void update_current_displacement(const vec&);
    void update_current_velocity(const vec&);
    void update_current_acceleration(const vec&);
    void update_current_temperature(const vec&);

    void update_pre_time(const double&);
    void update_pre_load(const vec&);
    void update_pre_resistance(const vec&);
    void update_pre_displacement(const vec&);
    void update_pre_velocity(const vec&);
    void update_pre_acceleration(const vec&);
    void update_pre_temperature(const vec&);

    void update_mass(const T2&);
    void update_damping(const T2&);
    void update_stiffness(const T2&);

    const vec& get_ninja() const;

    const double& get_trial_time() const;
    const vec& get_trial_load() const;
    const vec& get_trial_resistance() const;
    const vec& get_trial_displacement() const;
    const vec& get_trial_velocity() const;
    const vec& get_trial_acceleration() const;
    const vec& get_trial_temperature() const;

    const double& get_incre_time() const;
    const vec& get_incre_load() const;
    const vec& get_incre_resistance() const;
    const vec& get_incre_displacement() const;
    const vec& get_incre_velocity() const;
    const vec& get_incre_acceleration() const;
    const vec& get_incre_temperature() const;

    const double& get_current_time() const;
    const vec& get_current_load() const;
    const vec& get_current_resistance() const;
    const vec& get_current_displacement() const;
    const vec& get_current_velocity() const;
    const vec& get_current_acceleration() const;
    const vec& get_current_temperature() const;

    const double& get_pre_time() const;
    const vec& get_pre_load() const;
    const vec& get_pre_resistance() const;
    const vec& get_pre_displacement() const;
    const vec& get_pre_velocity() const;
    const vec& get_pre_acceleration() const;
    const vec& get_pre_temperature() const;

    const T2& get_mass() const;
    const T2& get_damping() const;
    const T2& get_stiffness() const;

    const vec& get_eigenvalue() const;
    const mat& get_eigenvector() const;

    friend vec& get_ninja(const shared_ptr<Workroom>&);

    friend double& get_trial_time(const shared_ptr<Workroom>&);
    friend vec& get_trial_load(const shared_ptr<Workroom>&);
    friend vec& get_trial_resistance(const shared_ptr<Workroom>&);
    friend vec& get_trial_displacement(const shared_ptr<Workroom>&);
    friend vec& get_trial_velocity(const shared_ptr<Workroom>&);
    friend vec& get_trial_acceleration(const shared_ptr<Workroom>&);
    friend vec& get_trial_temperature(const shared_ptr<Workroom>&);

    friend double& get_incre_time(const shared_ptr<Workroom>&);
    friend vec& get_incre_load(const shared_ptr<Workroom>&);
    friend vec& get_incre_resistance(const shared_ptr<Workroom>&);
    friend vec& get_incre_displacement(const shared_ptr<Workroom>&);
    friend vec& get_incre_velocity(const shared_ptr<Workroom>&);
    friend vec& get_incre_acceleration(const shared_ptr<Workroom>&);
    friend vec& get_incre_temperature(const shared_ptr<Workroom>&);

    friend double& get_current_time(const shared_ptr<Workroom>&);
    friend vec& get_current_load(const shared_ptr<Workroom>&);
    friend vec& get_current_resistance(const shared_ptr<Workroom>&);
    friend vec& get_current_displacement(const shared_ptr<Workroom>&);
    friend vec& get_current_velocity(const shared_ptr<Workroom>&);
    friend vec& get_current_acceleration(const shared_ptr<Workroom>&);
    friend vec& get_current_temperature(const shared_ptr<Workroom>&);

    friend double& get_pre_time(const shared_ptr<Workroom>&);
    friend vec& get_pre_load(const shared_ptr<Workroom>&);
    friend vec& get_pre_resistance(const shared_ptr<Workroom>&);
    friend vec& get_pre_displacement(const shared_ptr<Workroom>&);
    friend vec& get_pre_velocity(const shared_ptr<Workroom>&);
    friend vec& get_pre_acceleration(const shared_ptr<Workroom>&);
    friend vec& get_pre_temperature(const shared_ptr<Workroom>&);

    friend T2& get_mass(const shared_ptr<Workroom>&);
    friend T2& get_damping(const shared_ptr<Workroom>&);
    friend T2& get_stiffness(const shared_ptr<Workroom>&);

    friend vec& get_eigenvalue(const shared_ptr<Workroom>&);
    friend mat& get_eigenvector(const shared_ptr<Workroom>&);

    void commit_status();
    void commit_time();
    void commit_load();
    void commit_resistance();
    void commit_displacement();
    void commit_velocity();
    void commit_acceleration();
    void commit_temperature();

    void commit_pre_status();
    void commit_pre_time();
    void commit_pre_load();
    void commit_pre_resistance();
    void commit_pre_displacement();
    void commit_pre_velocity();
    void commit_pre_acceleration();
    void commit_pre_temperature();

    void clear_status();
    void clear_time();
    void clear_load();
    void clear_resistance();
    void clear_displacement();
    void clear_velocity();
    void clear_acceleration();
    void clear_temperature();

    void reset_status();
    void reset_time();
    void reset_load();
    void reset_resistance();
    void reset_displacement();
    void reset_velocity();
    void reset_acceleration();
    void reset_temperature();

    void clear_eigen();
    void clear_mass();
    void clear_damping();
    void clear_stiffness();

    static void print();

    void assemble_resistance(const mat&, const uvec&);
    void assemble_mass(const mat&, const uvec&);
    void assemble_damping(const mat&, const uvec&);
    void assemble_stiffness(const mat&, const uvec&);
};

#endif
