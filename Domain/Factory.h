/**
 * @class Factory
 * @brief A Factory class that holds global variables.
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
 * @date 30/08/2017
 * @version 0.1.3
 * @file Factory.h
 */

#ifndef FACTORY_H
#define FACTORY_H

#include "Workroom.h"
#include <suanPan.h>

template <typename eT, typename T> class Factory final
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

    eT trial_time = 0.;   /**< global trial (pseudo) time */
    eT incre_time = 0.;   /**< global incremental (pseudo) time */
    eT current_time = 0.; /**< global current (pseudo) time */
    eT pre_time = 0.;     /**< global previous (pseudo) time */

    eT error = 0.; /**< error produced by certain solvers */

    Col<eT> ninja; /**< the result from A*X=B */

    Col<eT> trial_load;         /**< global trial load vector */
    Col<eT> trial_resistance;   /**< global trial resistance vector */
    Col<eT> trial_displacement; /**< global trial displacement vector */
    Col<eT> trial_velocity;     /**< global trial velocity vector */
    Col<eT> trial_acceleration; /**< global trial acceleration vector */
    Col<eT> trial_temperature;  /**< global trial temperature vector */

    Col<eT> incre_load;         /**< global incremental load vector */
    Col<eT> incre_resistance;   /**< global incremental resistance vector */
    Col<eT> incre_displacement; /**< global incremental displacement vector */
    Col<eT> incre_velocity;     /**< global incremental velocity vector */
    Col<eT> incre_acceleration; /**< global incremental acceleration vector */
    Col<eT> incre_temperature;  /**< global incremental temperature vector */

    Col<eT> current_load;         /**< global current load vector */
    Col<eT> current_resistance;   /**< global current resistance vector */
    Col<eT> current_displacement; /**< global current displacement vector */
    Col<eT> current_velocity;     /**< global current velocity vector */
    Col<eT> current_acceleration; /**< global current acceleration vector */
    Col<eT> current_temperature;  /**< global current temperature vector */

    Col<eT> pre_load;         /**< global previous load vector */
    Col<eT> pre_resistance;   /**< global previous resistance vector */
    Col<eT> pre_displacement; /**< global previous displacement vector */
    Col<eT> pre_velocity;     /**< global previous velocity vector */
    Col<eT> pre_acceleration; /**< global previous acceleration vector */
    Col<eT> pre_temperature;  /**< global previous temperature vector */

    T global_mass;      /**< global mass matrix */
    T global_damping;   /**< global damping matrix */
    T global_stiffness; /**< global stiffness matrix */

    Col<eT> eigenvalue; /**< eigenvalues */

    Mat<eT> eigenvector; /**< eigenvectors */
public:
    explicit Factory(const unsigned& = 0, const AnalysisType& = AnalysisType::NONE);

    const bool& is_symm() const;
    const bool& is_band() const;

    void set_symm(const bool&);
    void set_band(const bool&);

    void set_dof_number(const unsigned&);
    const unsigned& get_dof_number() const;

    void set_analysis_type(const AnalysisType&);
    const AnalysisType& get_analysis_type() const;

    void set_bandwidth(const unsigned&, const unsigned&);
    void get_bandwidth(unsigned&, unsigned&) const;

    void set_error(const eT&);
    const eT& get_error() const;

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

    void update_ninja(const Col<eT>&);

    void update_trial_time(const eT&);
    void update_trial_load(const Col<eT>&);
    void update_trial_resistance(const Col<eT>&);
    void update_trial_displacement(const Col<eT>&);
    void update_trial_velocity(const Col<eT>&);
    void update_trial_acceleration(const Col<eT>&);
    void update_trial_temperature(const Col<eT>&);

    void update_incre_time(const eT&);
    void update_incre_load(const Col<eT>&);
    void update_incre_resistance(const Col<eT>&);
    void update_incre_displacement(const Col<eT>&);
    void update_incre_velocity(const Col<eT>&);
    void update_incre_acceleration(const Col<eT>&);
    void update_incre_temperature(const Col<eT>&);

    void update_current_time(const eT&);
    void update_current_load(const Col<eT>&);
    void update_current_resistance(const Col<eT>&);
    void update_current_displacement(const Col<eT>&);
    void update_current_velocity(const Col<eT>&);
    void update_current_acceleration(const Col<eT>&);
    void update_current_temperature(const Col<eT>&);

    void update_pre_time(const eT&);
    void update_pre_load(const Col<eT>&);
    void update_pre_resistance(const Col<eT>&);
    void update_pre_displacement(const Col<eT>&);
    void update_pre_velocity(const Col<eT>&);
    void update_pre_acceleration(const Col<eT>&);
    void update_pre_temperature(const Col<eT>&);

    void update_mass(const T&);
    void update_damping(const T&);
    void update_stiffness(const T&);

    const Col<eT>& get_ninja() const;

    const eT& get_trial_time() const;
    const Col<eT>& get_trial_load() const;
    const Col<eT>& get_trial_resistance() const;
    const Col<eT>& get_trial_displacement() const;
    const Col<eT>& get_trial_velocity() const;
    const Col<eT>& get_trial_acceleration() const;
    const Col<eT>& get_trial_temperature() const;

    const eT& get_incre_time() const;
    const Col<eT>& get_incre_load() const;
    const Col<eT>& get_incre_resistance() const;
    const Col<eT>& get_incre_displacement() const;
    const Col<eT>& get_incre_velocity() const;
    const Col<eT>& get_incre_acceleration() const;
    const Col<eT>& get_incre_temperature() const;

    const eT& get_current_time() const;
    const Col<eT>& get_current_load() const;
    const Col<eT>& get_current_resistance() const;
    const Col<eT>& get_current_displacement() const;
    const Col<eT>& get_current_velocity() const;
    const Col<eT>& get_current_acceleration() const;
    const Col<eT>& get_current_temperature() const;

    const eT& get_pre_time() const;
    const Col<eT>& get_pre_load() const;
    const Col<eT>& get_pre_resistance() const;
    const Col<eT>& get_pre_displacement() const;
    const Col<eT>& get_pre_velocity() const;
    const Col<eT>& get_pre_acceleration() const;
    const Col<eT>& get_pre_temperature() const;

    const T& get_mass() const;
    const T& get_damping() const;
    const T& get_stiffness() const;

    const Col<eT>& get_eigenvalue() const;
    const Mat<eT>& get_eigenvector() const;

    friend Col<eT>& get_ninja(const shared_ptr<Factory>&);

    friend eT& get_trial_time(const shared_ptr<Factory>&);
    friend Col<eT>& get_trial_load(const shared_ptr<Factory>&);
    friend Col<eT>& get_trial_resistance(const shared_ptr<Factory>&);
    friend Col<eT>& get_trial_displacement(const shared_ptr<Factory>&);
    friend Col<eT>& get_trial_velocity(const shared_ptr<Factory>&);
    friend Col<eT>& get_trial_acceleration(const shared_ptr<Factory>&);
    friend Col<eT>& get_trial_temperature(const shared_ptr<Factory>&);

    friend eT& get_incre_time(const shared_ptr<Factory>&);
    friend Col<eT>& get_incre_load(const shared_ptr<Factory>&);
    friend Col<eT>& get_incre_resistance(const shared_ptr<Factory>&);
    friend Col<eT>& get_incre_displacement(const shared_ptr<Factory>&);
    friend Col<eT>& get_incre_velocity(const shared_ptr<Factory>&);
    friend Col<eT>& get_incre_acceleration(const shared_ptr<Factory>&);
    friend Col<eT>& get_incre_temperature(const shared_ptr<Factory>&);

    friend eT& get_current_time(const shared_ptr<Factory>&);
    friend Col<eT>& get_current_load(const shared_ptr<Factory>&);
    friend Col<eT>& get_current_resistance(const shared_ptr<Factory>&);
    friend Col<eT>& get_current_displacement(const shared_ptr<Factory>&);
    friend Col<eT>& get_current_velocity(const shared_ptr<Factory>&);
    friend Col<eT>& get_current_acceleration(const shared_ptr<Factory>&);
    friend Col<eT>& get_current_temperature(const shared_ptr<Factory>&);

    friend eT& get_pre_time(const shared_ptr<Factory>&);
    friend Col<eT>& get_pre_load(const shared_ptr<Factory>&);
    friend Col<eT>& get_pre_resistance(const shared_ptr<Factory>&);
    friend Col<eT>& get_pre_displacement(const shared_ptr<Factory>&);
    friend Col<eT>& get_pre_velocity(const shared_ptr<Factory>&);
    friend Col<eT>& get_pre_acceleration(const shared_ptr<Factory>&);
    friend Col<eT>& get_pre_temperature(const shared_ptr<Factory>&);

    friend T& get_mass(const shared_ptr<Factory>&);
    friend T& get_damping(const shared_ptr<Factory>&);
    friend T& get_stiffness(const shared_ptr<Factory>&);

    friend Col<eT>& get_eigenvalue(const shared_ptr<Factory>&);
    friend Mat<eT>& get_eigenvector(const shared_ptr<Factory>&);

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

    void assemble_resistance(const Mat<eT>&, const uvec&);
    void assemble_mass(const Mat<eT>&, const uvec&);
    void assemble_damping(const Mat<eT>&, const uvec&);
    void assemble_stiffness(const Mat<eT>&, const uvec&);
};

#endif
