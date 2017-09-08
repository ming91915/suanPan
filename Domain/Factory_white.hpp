#pragma once

#include "Workshop.h"
#include <suanPan.h>

enum class StorageScheme { FULL, BAND, BANDSYMM, SYMMPACK };

template <typename T>
class Factory final {
    bool initialized = false;

    unsigned n_size = 0; /**< number of DoFs */
    unsigned n_lobw = 0; /**< low bandwidth */
    unsigned n_upbw = 0; /**< up bandwidth */
    unsigned n_sfbw = n_lobw + n_upbw;

    AnalysisType analysis_type = AnalysisType::NONE;  /**< type of analysis */
    StorageScheme storage_type = StorageScheme::FULL; /**< type of analysis */

    T trial_time = 0.;   /**< global trial (pseudo) time */
    T incre_time = 0.;   /**< global incremental (pseudo) time */
    T current_time = 0.; /**< global current (pseudo) time */
    T pre_time = 0.;     /**< global previous (pseudo) time */

    T error = 0.; /**< error produced by certain solvers */

    Col<T> ninja; /**< the result from A*X=B */

    Col<T> trial_load;         /**< global trial load vector */
    Col<T> trial_resistance;   /**< global trial resistance vector */
    Col<T> trial_displacement; /**< global trial displacement vector */
    Col<T> trial_velocity;     /**< global trial velocity vector */
    Col<T> trial_acceleration; /**< global trial acceleration vector */
    Col<T> trial_temperature;  /**< global trial temperature vector */

    Col<T> incre_load;         /**< global incremental load vector */
    Col<T> incre_resistance;   /**< global incremental resistance vector */
    Col<T> incre_displacement; /**< global incremental displacement vector */
    Col<T> incre_velocity;     /**< global incremental velocity vector */
    Col<T> incre_acceleration; /**< global incremental acceleration vector */
    Col<T> incre_temperature;  /**< global incremental temperature vector */

    Col<T> current_load;         /**< global current load vector */
    Col<T> current_resistance;   /**< global current resistance vector */
    Col<T> current_displacement; /**< global current displacement vector */
    Col<T> current_velocity;     /**< global current velocity vector */
    Col<T> current_acceleration; /**< global current acceleration vector */
    Col<T> current_temperature;  /**< global current temperature vector */

    Col<T> pre_load;         /**< global previous load vector */
    Col<T> pre_resistance;   /**< global previous resistance vector */
    Col<T> pre_displacement; /**< global previous displacement vector */
    Col<T> pre_velocity;     /**< global previous velocity vector */
    Col<T> pre_acceleration; /**< global previous acceleration vector */
    Col<T> pre_temperature;  /**< global previous temperature vector */

    shared_ptr<MetaMat<T>> global_mass = nullptr;      /**< global mass matrix */
    shared_ptr<MetaMat<T>> global_damping = nullptr;   /**< global damping matrix */
    shared_ptr<MetaMat<T>> global_stiffness = nullptr; /**< global stiffness matrix */

    Col<T> eigenvalue; /**< eigenvalues */

    Mat<T> eigenvector; /**< eigenvectors */
public:
    explicit Factory(const unsigned& = 0, const AnalysisType& = AnalysisType::NONE, const StorageScheme& = StorageScheme::FULL);

    void set_size(const unsigned&);
    const unsigned& get_size() const;

    void set_analysis_type(const AnalysisType&);
    const AnalysisType& get_analysis_type() const;

    void set_storage_scheme(const StorageScheme&);
    const StorageScheme& get_storage_scheme() const;

    void set_bandwidth(const unsigned&, const unsigned&);
    void get_bandwidth(unsigned&, unsigned&) const;

    void set_error(const T&);
    const T& get_error() const;

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

    void update_ninja(const Col<T>&);

    void update_trial_time(const T&);
    void update_trial_load(const Col<T>&);
    void update_trial_resistance(const Col<T>&);
    void update_trial_displacement(const Col<T>&);
    void update_trial_velocity(const Col<T>&);
    void update_trial_acceleration(const Col<T>&);
    void update_trial_temperature(const Col<T>&);

    void update_incre_time(const T&);
    void update_incre_load(const Col<T>&);
    void update_incre_resistance(const Col<T>&);
    void update_incre_displacement(const Col<T>&);
    void update_incre_velocity(const Col<T>&);
    void update_incre_acceleration(const Col<T>&);
    void update_incre_temperature(const Col<T>&);

    void update_current_time(const T&);
    void update_current_load(const Col<T>&);
    void update_current_resistance(const Col<T>&);
    void update_current_displacement(const Col<T>&);
    void update_current_velocity(const Col<T>&);
    void update_current_acceleration(const Col<T>&);
    void update_current_temperature(const Col<T>&);

    void update_pre_time(const T&);
    void update_pre_load(const Col<T>&);
    void update_pre_resistance(const Col<T>&);
    void update_pre_displacement(const Col<T>&);
    void update_pre_velocity(const Col<T>&);
    void update_pre_acceleration(const Col<T>&);
    void update_pre_temperature(const Col<T>&);

    void update_mass(const shared_ptr<MetaMat<T>>&);
    void update_damping(const shared_ptr<MetaMat<T>>&);
    void update_stiffness(const shared_ptr<MetaMat<T>>&);

    const Col<T>& get_ninja() const;

    const T& get_trial_time() const;
    const Col<T>& get_trial_load() const;
    const Col<T>& get_trial_resistance() const;
    const Col<T>& get_trial_displacement() const;
    const Col<T>& get_trial_velocity() const;
    const Col<T>& get_trial_acceleration() const;
    const Col<T>& get_trial_temperature() const;

    const T& get_incre_time() const;
    const Col<T>& get_incre_load() const;
    const Col<T>& get_incre_resistance() const;
    const Col<T>& get_incre_displacement() const;
    const Col<T>& get_incre_velocity() const;
    const Col<T>& get_incre_acceleration() const;
    const Col<T>& get_incre_temperature() const;

    const T& get_current_time() const;
    const Col<T>& get_current_load() const;
    const Col<T>& get_current_resistance() const;
    const Col<T>& get_current_displacement() const;
    const Col<T>& get_current_velocity() const;
    const Col<T>& get_current_acceleration() const;
    const Col<T>& get_current_temperature() const;

    const T& get_pre_time() const;
    const Col<T>& get_pre_load() const;
    const Col<T>& get_pre_resistance() const;
    const Col<T>& get_pre_displacement() const;
    const Col<T>& get_pre_velocity() const;
    const Col<T>& get_pre_acceleration() const;
    const Col<T>& get_pre_temperature() const;

    const shared_ptr<MetaMat<T>>& get_mass() const;
    const shared_ptr<MetaMat<T>>& get_damping() const;
    const shared_ptr<MetaMat<T>>& get_stiffness() const;

    const Col<T>& get_eigenvalue() const;
    const Mat<T>& get_eigenvector() const;

    template <T>
    friend Col<T>& get_ninja(const shared_ptr<Factory<T>>&);

    template <T>
    friend T& get_trial_time(const shared_ptr<Factory<T>>&);
    template <T>
    friend Col<T>& get_trial_load(const shared_ptr<Factory<T>>&);
    template <T>
    friend Col<T>& get_trial_resistance(const shared_ptr<Factory<T>>&);
    template <T>
    friend Col<T>& get_trial_displacement(const shared_ptr<Factory<T>>&);
    template <T>
    friend Col<T>& get_trial_velocity(const shared_ptr<Factory<T>>&);
    template <T>
    friend Col<T>& get_trial_acceleration(const shared_ptr<Factory<T>>&);
    template <T>
    friend Col<T>& get_trial_temperature(const shared_ptr<Factory<T>>&);

    template <T>
    friend T& get_incre_time(const shared_ptr<Factory<T>>&);
    template <T>
    friend Col<T>& get_incre_load(const shared_ptr<Factory<T>>&);
    template <T>
    friend Col<T>& get_incre_resistance(const shared_ptr<Factory<T>>&);
    template <T>
    friend Col<T>& get_incre_displacement(const shared_ptr<Factory<T>>&);
    template <T>
    friend Col<T>& get_incre_velocity(const shared_ptr<Factory<T>>&);
    template <T>
    friend Col<T>& get_incre_acceleration(const shared_ptr<Factory<T>>&);
    template <T>
    friend Col<T>& get_incre_temperature(const shared_ptr<Factory<T>>&);

    template <T>
    friend T& get_current_time(const shared_ptr<Factory<T>>&);
    template <T>
    friend Col<T>& get_current_load(const shared_ptr<Factory<T>>&);
    template <T>
    friend Col<T>& get_current_resistance(const shared_ptr<Factory<T>>&);
    template <T>
    friend Col<T>& get_current_displacement(const shared_ptr<Factory<T>>&);
    template <T>
    friend Col<T>& get_current_velocity(const shared_ptr<Factory<T>>&);
    template <T>
    friend Col<T>& get_current_acceleration(const shared_ptr<Factory<T>>&);
    template <T>
    friend Col<T>& get_current_temperature(const shared_ptr<Factory<T>>&);

    template <T>
    friend T& get_pre_time(const shared_ptr<Factory<T>>&);
    template <T>
    friend Col<T>& get_pre_load(const shared_ptr<Factory<T>>&);
    template <T>
    friend Col<T>& get_pre_resistance(const shared_ptr<Factory<T>>&);
    template <T>
    friend Col<T>& get_pre_displacement(const shared_ptr<Factory<T>>&);
    template <T>
    friend Col<T>& get_pre_velocity(const shared_ptr<Factory<T>>&);
    template <T>
    friend Col<T>& get_pre_acceleration(const shared_ptr<Factory<T>>&);
    template <T>
    friend Col<T>& get_pre_temperature(const shared_ptr<Factory<T>>&);

    template <T>
    friend shared_ptr<MetaMat<T>>& get_mass(const shared_ptr<Factory<T>>&);
    template <T>
    friend shared_ptr<MetaMat<T>>& get_damping(const shared_ptr<Factory<T>>&);
    template <T>
    friend shared_ptr<MetaMat<T>>& get_stiffness(const shared_ptr<Factory<T>>&);

    template <T>
    friend Col<T>& get_eigenvalue(const shared_ptr<Factory<T>>&);
    template <T>
    friend Mat<T>& get_eigenvector(const shared_ptr<Factory<T>>&);

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

    void print() const;

    void assemble_resistance(const Mat<T>&, const uvec&);
    void assemble_mass(const Mat<T>&, const uvec&);
    void assemble_damping(const Mat<T>&, const uvec&);
    void assemble_stiffness(const Mat<T>&, const uvec&);
};
