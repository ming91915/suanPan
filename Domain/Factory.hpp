
#ifndef FACTORY_HPP
#define FACTORY_HPP

#include <Domain/MetaMat/MetaMat>
#include <suanPan.h>

enum class AnalysisType { NONE, DISP, EIGEN, STATICS, DYNAMICS };
enum class StorageScheme { FULL, BAND, BANDSYMM, SYMMPACK };

template <typename T> class Factory final {
    unsigned n_size = 0; /**< number of DoFs */
    unsigned n_lobw = 0; /**< low bandwidth */
    unsigned n_upbw = 0; /**< up bandwidth */
    unsigned n_sfbw = n_lobw + n_upbw;

    AnalysisType analysis_type = AnalysisType::NONE;  /**< type of analysis */
    StorageScheme storage_type = StorageScheme::FULL; /**< type of analysis */

    T error = 0.; /**< error produced by certain solvers */

    Col<T> ninja; /**< the result from A*X=B */

    T trial_time = 0.;   /**< global trial (pseudo) time */
    T incre_time = 0.;   /**< global incremental (pseudo) time */
    T current_time = 0.; /**< global current (pseudo) time */
    T pre_time = 0.;     /**< global previous (pseudo) time */

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

    template <typename T1> friend Col<T1>& get_ninja(const shared_ptr<Factory<T1>>&);

    template <typename T1> friend T& get_trial_time(const shared_ptr<Factory<T1>>&);
    template <typename T1> friend Col<T1>& get_trial_load(const shared_ptr<Factory<T1>>&);
    template <typename T1> friend Col<T1>& get_trial_resistance(const shared_ptr<Factory<T1>>&);
    template <typename T1> friend Col<T1>& get_trial_displacement(const shared_ptr<Factory<T1>>&);
    template <typename T1> friend Col<T1>& get_trial_velocity(const shared_ptr<Factory<T1>>&);
    template <typename T1> friend Col<T1>& get_trial_acceleration(const shared_ptr<Factory<T1>>&);
    template <typename T1> friend Col<T1>& get_trial_temperature(const shared_ptr<Factory<T1>>&);

    template <typename T1> friend T& get_incre_time(const shared_ptr<Factory<T1>>&);
    template <typename T1> friend Col<T1>& get_incre_load(const shared_ptr<Factory<T1>>&);
    template <typename T1> friend Col<T1>& get_incre_resistance(const shared_ptr<Factory<T1>>&);
    template <typename T1> friend Col<T1>& get_incre_displacement(const shared_ptr<Factory<T1>>&);
    template <typename T1> friend Col<T1>& get_incre_velocity(const shared_ptr<Factory<T1>>&);
    template <typename T1> friend Col<T1>& get_incre_acceleration(const shared_ptr<Factory<T1>>&);
    template <typename T1> friend Col<T1>& get_incre_temperature(const shared_ptr<Factory<T1>>&);

    template <typename T1> friend T& get_current_time(const shared_ptr<Factory<T1>>&);
    template <typename T1> friend Col<T1>& get_current_load(const shared_ptr<Factory<T1>>&);
    template <typename T1> friend Col<T1>& get_current_resistance(const shared_ptr<Factory<T1>>&);
    template <typename T1> friend Col<T1>& get_current_displacement(const shared_ptr<Factory<T1>>&);
    template <typename T1> friend Col<T1>& get_current_velocity(const shared_ptr<Factory<T1>>&);
    template <typename T1> friend Col<T1>& get_current_acceleration(const shared_ptr<Factory<T1>>&);
    template <typename T1> friend Col<T1>& get_current_temperature(const shared_ptr<Factory<T1>>&);

    template <typename T1> friend T& get_pre_time(const shared_ptr<Factory<T1>>&);
    template <typename T1> friend Col<T1>& get_pre_load(const shared_ptr<Factory<T1>>&);
    template <typename T1> friend Col<T1>& get_pre_resistance(const shared_ptr<Factory<T1>>&);
    template <typename T1> friend Col<T1>& get_pre_displacement(const shared_ptr<Factory<T1>>&);
    template <typename T1> friend Col<T1>& get_pre_velocity(const shared_ptr<Factory<T1>>&);
    template <typename T1> friend Col<T1>& get_pre_acceleration(const shared_ptr<Factory<T1>>&);
    template <typename T1> friend Col<T1>& get_pre_temperature(const shared_ptr<Factory<T1>>&);

    template <typename T1> friend shared_ptr<MetaMat<T1>>& get_mass(const shared_ptr<Factory<T1>>&);
    template <typename T1> friend shared_ptr<MetaMat<T1>>& get_damping(const shared_ptr<Factory<T1>>&);
    template <typename T1> friend shared_ptr<MetaMat<T1>>& get_stiffness(const shared_ptr<Factory<T1>>&);

    template <typename T1> friend Col<T1>& get_eigenvalue(const shared_ptr<Factory<T1>>&);
    template <typename T1> friend Mat<T1>& get_eigenvector(const shared_ptr<Factory<T1>>&);

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

    void assemble_resistance(const Mat<T>&, const uvec&);
    void assemble_mass(const Mat<T>&, const uvec&);
    void assemble_damping(const Mat<T>&, const uvec&);
    void assemble_stiffness(const Mat<T>&, const uvec&);

    void print() const;
};

template <typename T>
Factory<T>::Factory(const unsigned& D, const AnalysisType& AT, const StorageScheme& SS)
    : n_size(D)
    , analysis_type(AT)
    , storage_type(SS) {}

template <typename T> void Factory<T>::set_size(const unsigned& D) { n_size = D; }

template <typename T> const unsigned& Factory<T>::get_size() const { return n_size; }

template <typename T> void Factory<T>::set_analysis_type(const AnalysisType& AT) { analysis_type = AT; }

template <typename T> const AnalysisType& Factory<T>::get_analysis_type() const { return analysis_type; }

template <typename T> void Factory<T>::set_storage_scheme(const StorageScheme& SS) { storage_type = SS; }

template <typename T> const StorageScheme& Factory<T>::get_storage_scheme() const { return storage_type; }

template <typename T> void Factory<T>::set_bandwidth(const unsigned& L, const unsigned& U) {
    n_lobw = L;
    n_upbw = U;
    n_sfbw = L + U;
}

template <typename T> void Factory<T>::get_bandwidth(unsigned& L, unsigned& U) const {
    L = n_lobw;
    U = n_upbw;
}

template <typename T> void Factory<T>::set_error(const T& E) { error = E; }

template <typename T> const T& Factory<T>::get_error() const { return error; }

template <typename T> int Factory<T>::initialize() {
    if(n_size != 0) {
        ninja.zeros(n_size);

        switch(analysis_type) {
        case AnalysisType::DISP:
            initialize_displacement(n_size);
            break;
        case AnalysisType::EIGEN:
            initialize_eigen(n_size);
            initialize_mass(n_size);
            initialize_stiffness(n_size);
            break;
        case AnalysisType::STATICS:
            initialize_load(n_size);
            initialize_resistance(n_size);
            initialize_displacement(n_size);
            initialize_stiffness(n_size);
            break;
        case AnalysisType::DYNAMICS:
            initialize_load(n_size);
            initialize_resistance(n_size);
            initialize_displacement(n_size);
            initialize_velocity(n_size);
            initialize_acceleration(n_size);
            initialize_mass(n_size);
            initialize_damping(n_size);
            initialize_stiffness(n_size);
            break;
        case AnalysisType::NONE:
            break;
        }
    }

    return 0;
}

template <typename T> void Factory<T>::initialize_load(const unsigned& D) {
    trial_load.zeros(D);
    incre_load.zeros(D);
    current_load.zeros(D);
}

template <typename T> void Factory<T>::initialize_resistance(const unsigned& D) {
    trial_resistance.zeros(D);
    incre_resistance.zeros(D);
    current_resistance.zeros(D);
}

template <typename T> void Factory<T>::initialize_displacement(const unsigned& D) {
    trial_displacement.zeros(D);
    incre_displacement.zeros(D);
    current_displacement.zeros(D);
}

template <typename T> void Factory<T>::initialize_velocity(const unsigned& D) {
    trial_velocity.zeros(D);
    incre_velocity.zeros(D);
    current_velocity.zeros(D);
}

template <typename T> void Factory<T>::initialize_acceleration(const unsigned& D) {
    trial_acceleration.zeros(D);
    incre_acceleration.zeros(D);
    current_acceleration.zeros(D);
}

template <typename T> void Factory<T>::initialize_temperature(const unsigned& D) {
    trial_temperature.zeros(D);
    incre_temperature.zeros(D);
    current_temperature.zeros(D);
}

template <typename T> void Factory<T>::initialize_eigen(const unsigned& D) {
    eigenvalue.zeros(D);
    eigenvector.zeros(D, D);
}

template <typename T> void Factory<T>::initialize_mass(const unsigned& D) {
    switch(storage_type) {
    case StorageScheme::FULL:
        global_mass = make_shared<FullMat<T>>(D);
        break;
    case StorageScheme::BAND:
        global_mass = make_shared<BandMat<T>>(D, n_lobw, n_upbw);
        break;
    case StorageScheme::BANDSYMM:
        global_mass = make_shared<BandSymmMat<T>>(D, n_lobw);
        break;
    case StorageScheme::SYMMPACK:
        global_mass = make_shared<SymmPackMat<T>>(D);
        break;
    }
}

template <typename T> void Factory<T>::initialize_damping(const unsigned& D) {
    switch(storage_type) {
    case StorageScheme::FULL:
        global_damping = make_shared<FullMat<T>>(D);
        break;
    case StorageScheme::BAND:
        global_damping = make_shared<BandMat<T>>(D, n_lobw, n_upbw);
        break;
    case StorageScheme::BANDSYMM:
        global_damping = make_shared<BandSymmMat<T>>(D, n_lobw);
        break;
    case StorageScheme::SYMMPACK:
        global_damping = make_shared<SymmPackMat<T>>(D);
        break;
    }
}

template <typename T> void Factory<T>::initialize_stiffness(const unsigned& D) {
    switch(storage_type) {
    case StorageScheme::FULL:
        global_stiffness = make_shared<FullMat<T>>(D);
        break;
    case StorageScheme::BAND:
        global_stiffness = make_shared<BandMat<T>>(D, n_lobw, n_upbw);
        break;
    case StorageScheme::BANDSYMM:
        global_stiffness = make_shared<BandSymmMat<T>>(D, n_lobw);
        break;
    case StorageScheme::SYMMPACK:
        global_stiffness = make_shared<SymmPackMat<T>>(D);
        break;
    }
}

template <typename T> void Factory<T>::update_ninja(const Col<T>& N) { ninja = N; }

template <typename T> void Factory<T>::update_trial_time(const T& M) {
    trial_time = M;
    incre_time = trial_time - current_time;
}

template <typename T> void Factory<T>::update_trial_load(const Col<T>& L) {
    trial_load = L;
    incre_load = trial_load - current_load;
}

template <typename T> void Factory<T>::update_trial_resistance(const Col<T>& R) {
    trial_resistance = R;
    incre_resistance = trial_resistance - current_resistance;
}

template <typename T> void Factory<T>::update_trial_displacement(const Col<T>& D) {
    trial_displacement = D;
    incre_displacement = trial_displacement - current_displacement;
}

template <typename T> void Factory<T>::update_trial_velocity(const Col<T>& V) {
    trial_velocity = V;
    incre_velocity = trial_velocity - current_velocity;
}

template <typename T> void Factory<T>::update_trial_acceleration(const Col<T>& A) {
    trial_acceleration = A;
    incre_acceleration = trial_acceleration - current_acceleration;
}

template <typename T> void Factory<T>::update_trial_temperature(const Col<T>& M) {
    trial_temperature = M;
    incre_temperature = trial_temperature - current_temperature;
}

template <typename T> void Factory<T>::update_incre_time(const T& M) {
    incre_time = M;
    trial_time = current_time + incre_time;
}

template <typename T> void Factory<T>::update_incre_load(const Col<T>& L) {
    incre_load = L;
    trial_load = current_load + incre_load;
}

template <typename T> void Factory<T>::update_incre_resistance(const Col<T>& R) {
    incre_resistance = R;
    trial_resistance = current_resistance + incre_resistance;
}

template <typename T> void Factory<T>::update_incre_displacement(const Col<T>& D) {
    incre_displacement = D;
    trial_displacement = current_displacement + incre_displacement;
}

template <typename T> void Factory<T>::update_incre_velocity(const Col<T>& V) {
    incre_velocity = V;
    trial_velocity = current_velocity + incre_velocity;
}

template <typename T> void Factory<T>::update_incre_acceleration(const Col<T>& A) {
    incre_acceleration = A;
    trial_acceleration = current_acceleration + incre_acceleration;
}

template <typename T> void Factory<T>::update_incre_temperature(const Col<T>& M) {
    incre_temperature = M;
    trial_temperature = current_temperature + incre_temperature;
}

template <typename T> void Factory<T>::update_current_time(const T& M) { current_time = M; }

template <typename T> void Factory<T>::update_current_load(const Col<T>& L) { current_load = L; }

template <typename T> void Factory<T>::update_current_resistance(const Col<T>& R) { current_resistance = R; }

template <typename T> void Factory<T>::update_current_displacement(const Col<T>& D) { current_displacement = D; }

template <typename T> void Factory<T>::update_current_velocity(const Col<T>& V) { current_velocity = V; }

template <typename T> void Factory<T>::update_current_acceleration(const Col<T>& A) { current_acceleration = A; }

template <typename T> void Factory<T>::update_current_temperature(const Col<T>& M) { current_temperature = M; }

template <typename T> void Factory<T>::update_pre_time(const T& M) { pre_time = M; }

template <typename T> void Factory<T>::update_pre_load(const Col<T>& L) { pre_load = L; }

template <typename T> void Factory<T>::update_pre_resistance(const Col<T>& R) { pre_resistance = R; }

template <typename T> void Factory<T>::update_pre_displacement(const Col<T>& D) { pre_displacement = D; }

template <typename T> void Factory<T>::update_pre_velocity(const Col<T>& V) { pre_velocity = V; }

template <typename T> void Factory<T>::update_pre_acceleration(const Col<T>& A) { pre_acceleration = A; }

template <typename T> void Factory<T>::update_pre_temperature(const Col<T>& M) { pre_temperature = M; }

template <typename T> void Factory<T>::update_mass(const shared_ptr<MetaMat<T>>& M) { global_mass = M; }

template <typename T> void Factory<T>::update_damping(const shared_ptr<MetaMat<T>>& D) { global_damping = D; }

template <typename T> void Factory<T>::update_stiffness(const shared_ptr<MetaMat<T>>& K) { global_stiffness = K; }

template <typename T> const Col<T>& Factory<T>::get_ninja() const { return ninja; }

template <typename T> const T& Factory<T>::get_trial_time() const { return trial_time; }

template <typename T> const Col<T>& Factory<T>::get_trial_load() const { return trial_load; }

template <typename T> const Col<T>& Factory<T>::get_trial_resistance() const { return trial_resistance; }

template <typename T> const Col<T>& Factory<T>::get_trial_displacement() const { return trial_displacement; }

template <typename T> const Col<T>& Factory<T>::get_trial_velocity() const { return trial_velocity; }

template <typename T> const Col<T>& Factory<T>::get_trial_acceleration() const { return trial_acceleration; }

template <typename T> const Col<T>& Factory<T>::get_trial_temperature() const { return trial_temperature; }

template <typename T> const T& Factory<T>::get_incre_time() const { return incre_time; }

template <typename T> const Col<T>& Factory<T>::get_incre_load() const { return incre_load; }

template <typename T> const Col<T>& Factory<T>::get_incre_resistance() const { return incre_resistance; }

template <typename T> const Col<T>& Factory<T>::get_incre_displacement() const { return incre_displacement; }

template <typename T> const Col<T>& Factory<T>::get_incre_velocity() const { return incre_velocity; }

template <typename T> const Col<T>& Factory<T>::get_incre_acceleration() const { return incre_acceleration; }

template <typename T> const Col<T>& Factory<T>::get_incre_temperature() const { return incre_temperature; }

template <typename T> const T& Factory<T>::get_current_time() const { return current_time; }

template <typename T> const Col<T>& Factory<T>::get_current_load() const { return current_load; }

template <typename T> const Col<T>& Factory<T>::get_current_resistance() const { return current_resistance; }

template <typename T> const Col<T>& Factory<T>::get_current_displacement() const { return current_displacement; }

template <typename T> const Col<T>& Factory<T>::get_current_velocity() const { return current_velocity; }

template <typename T> const Col<T>& Factory<T>::get_current_acceleration() const { return current_acceleration; }

template <typename T> const Col<T>& Factory<T>::get_current_temperature() const { return current_temperature; }

template <typename T> const T& Factory<T>::get_pre_time() const { return pre_time; }

template <typename T> const Col<T>& Factory<T>::get_pre_load() const { return pre_load; }

template <typename T> const Col<T>& Factory<T>::get_pre_resistance() const { return pre_resistance; }

template <typename T> const Col<T>& Factory<T>::get_pre_displacement() const { return pre_displacement; }

template <typename T> const Col<T>& Factory<T>::get_pre_velocity() const { return pre_velocity; }

template <typename T> const Col<T>& Factory<T>::get_pre_acceleration() const { return pre_acceleration; }

template <typename T> const Col<T>& Factory<T>::get_pre_temperature() const { return pre_temperature; }

template <typename T> const shared_ptr<MetaMat<T>>& Factory<T>::get_mass() const { return global_mass; }

template <typename T> const shared_ptr<MetaMat<T>>& Factory<T>::get_damping() const { return global_damping; }

template <typename T> const shared_ptr<MetaMat<T>>& Factory<T>::get_stiffness() const { return global_stiffness; }

template <typename T> const Col<T>& Factory<T>::get_eigenvalue() const { return eigenvalue; }

template <typename T> const Mat<T>& Factory<T>::get_eigenvector() const { return eigenvector; }

template <typename T> void Factory<T>::commit_status() {
    commit_time();

    switch(analysis_type) {
    case AnalysisType::DISP:
        commit_displacement();
        break;
    case AnalysisType::EIGEN:
        break;
    case AnalysisType::STATICS:
        commit_load();
        commit_resistance();
        commit_displacement();
        break;
    case AnalysisType::DYNAMICS:
        commit_load();
        commit_resistance();
        commit_displacement();
        commit_velocity();
        commit_acceleration();
        break;
    case AnalysisType::NONE:
        break;
    }
    // commit_temperature();
}

template <typename T> void Factory<T>::commit_time() { current_time = trial_time; }

template <typename T> void Factory<T>::commit_load() { current_load = trial_load; }

template <typename T> void Factory<T>::commit_resistance() { current_resistance = trial_resistance; }

template <typename T> void Factory<T>::commit_displacement() { current_displacement = trial_displacement; }

template <typename T> void Factory<T>::commit_velocity() { current_velocity = trial_velocity; }

template <typename T> void Factory<T>::commit_acceleration() { current_acceleration = trial_acceleration; }

template <typename T> void Factory<T>::commit_temperature() { current_temperature = trial_temperature; }

template <typename T> void Factory<T>::commit_pre_status() {
    commit_pre_time();

    switch(analysis_type) {
    case AnalysisType::DISP:
        commit_pre_displacement();
        break;
    case AnalysisType::EIGEN:
        break;
    case AnalysisType::STATICS:
        commit_pre_load();
        commit_pre_displacement();
        break;
    case AnalysisType::DYNAMICS:
        commit_pre_load();
        commit_pre_displacement();
        commit_pre_velocity();
        commit_pre_acceleration();
        break;
    case AnalysisType::NONE:
        break;
    }
    // commit_pre_temperature();
}

template <typename T> void Factory<T>::commit_pre_time() { pre_time = current_time; }

template <typename T> void Factory<T>::commit_pre_load() { pre_load = current_load; }

template <typename T> void Factory<T>::commit_pre_resistance() { pre_resistance = current_resistance; }

template <typename T> void Factory<T>::commit_pre_displacement() { pre_displacement = current_displacement; }

template <typename T> void Factory<T>::commit_pre_velocity() { pre_velocity = current_velocity; }

template <typename T> void Factory<T>::commit_pre_acceleration() { pre_acceleration = current_acceleration; }

template <typename T> void Factory<T>::commit_pre_temperature() { pre_temperature = current_temperature; }

template <typename T> void Factory<T>::clear_status() {
    ninja.zeros();

    clear_time();
    clear_load();
    clear_resistance();
    clear_displacement();
    clear_velocity();
    clear_acceleration();
    clear_temperature();

    clear_eigen();
    clear_mass();
    clear_damping();
    clear_stiffness();
}

template <typename T> void Factory<T>::clear_time() {
    trial_time = 0.;
    incre_time = 0.;
    current_time = 0.;
}

template <typename T> void Factory<T>::clear_load() {
    if(!pre_load.is_empty()) pre_load.zeros();
    if(!trial_load.is_empty()) trial_load.zeros();
    if(!incre_load.is_empty()) incre_load.zeros();
    if(!current_load.is_empty()) current_load.zeros();
}

template <typename T> void Factory<T>::clear_resistance() {
    if(!pre_resistance.is_empty()) pre_resistance.zeros();
    if(!trial_resistance.is_empty()) trial_resistance.zeros();
    if(!incre_resistance.is_empty()) incre_resistance.zeros();
    if(!current_resistance.is_empty()) current_resistance.zeros();
}

template <typename T> void Factory<T>::clear_displacement() {
    if(!pre_displacement.is_empty()) pre_displacement.zeros();
    if(!trial_displacement.is_empty()) trial_displacement.zeros();
    if(!incre_displacement.is_empty()) incre_displacement.zeros();
    if(!current_displacement.is_empty()) current_displacement.zeros();
}

template <typename T> void Factory<T>::clear_velocity() {
    if(!pre_velocity.is_empty()) pre_velocity.zeros();
    if(!trial_velocity.is_empty()) trial_velocity.zeros();
    if(!incre_velocity.is_empty()) incre_velocity.zeros();
    if(!current_velocity.is_empty()) current_velocity.zeros();
}

template <typename T> void Factory<T>::clear_acceleration() {
    if(!pre_acceleration.is_empty()) pre_acceleration.zeros();
    if(!trial_acceleration.is_empty()) trial_acceleration.zeros();
    if(!incre_acceleration.is_empty()) incre_acceleration.zeros();
    if(!current_acceleration.is_empty()) current_acceleration.zeros();
}

template <typename T> void Factory<T>::clear_temperature() {
    if(!pre_temperature.is_empty()) pre_temperature.zeros();
    if(!trial_temperature.is_empty()) trial_temperature.zeros();
    if(!incre_temperature.is_empty()) incre_temperature.zeros();
    if(!current_temperature.is_empty()) current_temperature.zeros();
}

template <typename T> void Factory<T>::reset_status() {
    ninja.zeros();

    reset_time();
    reset_load();
    reset_resistance();
    reset_displacement();
    reset_velocity();
    reset_acceleration();
    reset_temperature();
}

template <typename T> void Factory<T>::reset_time() {
    trial_time = current_time;
    incre_time = 0.;
}

template <typename T> void Factory<T>::reset_load() {
    if(!trial_load.is_empty()) {
        trial_load = current_load;
        incre_load.zeros();
    }
}

template <typename T> void Factory<T>::reset_resistance() {
    if(!trial_resistance.is_empty()) {
        trial_resistance = current_resistance;
        incre_resistance.zeros();
    }
}

template <typename T> void Factory<T>::reset_displacement() {
    if(!trial_displacement.is_empty()) {
        trial_displacement = current_displacement;
        incre_displacement.zeros();
    }
}

template <typename T> void Factory<T>::reset_velocity() {
    if(!trial_velocity.is_empty()) {
        trial_velocity = current_velocity;
        incre_velocity.zeros();
    }
}

template <typename T> void Factory<T>::reset_acceleration() {
    if(!trial_acceleration.is_empty()) {
        trial_acceleration = current_acceleration;
        incre_acceleration.zeros();
    }
}

template <typename T> void Factory<T>::reset_temperature() {
    if(!trial_temperature.is_empty()) {
        trial_temperature = current_temperature;
        incre_temperature.zeros();
    }
}

template <typename T> void Factory<T>::clear_eigen() {
    if(!eigenvalue.is_empty()) eigenvalue.zeros();
    if(!eigenvector.is_empty()) eigenvector.zeros();
}

template <typename T> void Factory<T>::clear_mass() {
    if(global_mass != nullptr && !global_mass->is_empty()) global_mass->zeros();
}

template <typename T> void Factory<T>::clear_damping() {
    if(global_damping != nullptr && !global_damping->is_empty()) global_damping->zeros();
}

template <typename T> void Factory<T>::clear_stiffness() {
    if(global_stiffness != nullptr && !global_stiffness->is_empty()) global_stiffness->zeros();
}

template <typename T> void Factory<T>::print() const { suanpan_info("This is a Factory object with size of %u.\n", n_size); }

template <typename T> void Factory<T>::assemble_resistance(const Mat<T>& ER, const uvec& EI) {
    for(unsigned I = 0; I < EI.n_elem; ++I) trial_resistance(EI(I)) += ER(I);
}

template <typename T> void Factory<T>::assemble_mass(const Mat<T>& EM, const uvec& EI) {
    for(unsigned I = 0; I < EI.n_elem; ++I)
        for(unsigned J = 0; J < EI.n_elem; ++J) (*global_mass)(EI(J), EI(I)) += EM(J, I);
}

template <typename T> void Factory<T>::assemble_damping(const Mat<T>& EC, const uvec& EI) {
    for(unsigned I = 0; I < EI.n_elem; ++I)
        for(unsigned J = 0; J < EI.n_elem; ++J) (*global_damping)(EI(J), EI(I)) += EC(J, I);
}

template <typename T> void Factory<T>::assemble_stiffness(const Mat<T>& EK, const uvec& EI) {
    for(unsigned I = 0; I < EI.n_elem; ++I)
        for(unsigned J = 0; J < EI.n_elem; ++J) (*global_stiffness)(EI(J), EI(I)) += EK(J, I);
}

template <typename T> Col<T>& get_ninja(const shared_ptr<Factory<T>>& W) { return W->ninja; }

template <typename T> T& get_trial_time(const shared_ptr<Factory<T>>& W) { return W->trial_time; }

template <typename T> Col<T>& get_trial_load(const shared_ptr<Factory<T>>& W) { return W->trial_load; }

template <typename T> Col<T>& get_trial_resistance(const shared_ptr<Factory<T>>& W) { return W->trial_resistance; }

template <typename T> Col<T>& get_trial_displacement(const shared_ptr<Factory<T>>& W) { return W->trial_displacement; }

template <typename T> Col<T>& get_trial_velocity(const shared_ptr<Factory<T>>& W) { return W->trial_velocity; }

template <typename T> Col<T>& get_trial_acceleration(const shared_ptr<Factory<T>>& W) { return W->trial_acceleration; }

template <typename T> Col<T>& get_trial_temperature(const shared_ptr<Factory<T>>& W) { return W->trial_temperature; }

template <typename T> T& get_incre_time(const shared_ptr<Factory<T>>& W) { return W->incre_time; }

template <typename T> Col<T>& get_incre_load(const shared_ptr<Factory<T>>& W) { return W->incre_load; }

template <typename T> Col<T>& get_incre_resistance(const shared_ptr<Factory<T>>& W) { return W->incre_resistance; }

template <typename T> Col<T>& get_incre_displacement(const shared_ptr<Factory<T>>& W) { return W->incre_displacement; }

template <typename T> Col<T>& get_incre_velocity(const shared_ptr<Factory<T>>& W) { return W->incre_velocity; }

template <typename T> Col<T>& get_incre_acceleration(const shared_ptr<Factory<T>>& W) { return W->incre_acceleration; }

template <typename T> Col<T>& get_incre_temperature(const shared_ptr<Factory<T>>& W) { return W->incre_temperature; }

template <typename T> T& get_current_time(const shared_ptr<Factory<T>>& W) { return W->current_time; }

template <typename T> Col<T>& get_current_load(const shared_ptr<Factory<T>>& W) { return W->current_load; }

template <typename T> Col<T>& get_current_resistance(const shared_ptr<Factory<T>>& W) { return W->current_resistance; }

template <typename T> Col<T>& get_current_displacement(const shared_ptr<Factory<T>>& W) { return W->current_displacement; }

template <typename T> Col<T>& get_current_velocity(const shared_ptr<Factory<T>>& W) { return W->current_velocity; }

template <typename T> Col<T>& get_current_acceleration(const shared_ptr<Factory<T>>& W) { return W->current_acceleration; }

template <typename T> Col<T>& get_current_temperature(const shared_ptr<Factory<T>>& W) { return W->current_temperature; }

template <typename T> T& get_pre_time(const shared_ptr<Factory<T>>& W) { return W->pre_time; }

template <typename T> Col<T>& get_pre_load(const shared_ptr<Factory<T>>& W) { return W->pre_load; }

template <typename T> Col<T>& get_pre_resistance(const shared_ptr<Factory<T>>& W) { return W->pre_resistance; }

template <typename T> Col<T>& get_pre_displacement(const shared_ptr<Factory<T>>& W) { return W->pre_displacement; }

template <typename T> Col<T>& get_pre_velocity(const shared_ptr<Factory<T>>& W) { return W->pre_velocity; }

template <typename T> Col<T>& get_pre_acceleration(const shared_ptr<Factory<T>>& W) { return W->pre_acceleration; }

template <typename T> Col<T>& get_pre_temperature(const shared_ptr<Factory<T>>& W) { return W->pre_temperature; }

template <typename T> shared_ptr<MetaMat<T>>& get_mass(const shared_ptr<Factory<T>>& W) { return W->global_mass; }

template <typename T> shared_ptr<MetaMat<T>>& get_damping(const shared_ptr<Factory<T>>& W) { return W->global_damping; }

template <typename T> shared_ptr<MetaMat<T>>& get_stiffness(const shared_ptr<Factory<T>>& W) { return W->global_stiffness; }

template <typename T> Col<T>& get_eigenvalue(const shared_ptr<Factory<T>>& W) { return W->eigenvalue; }

template <typename T> Mat<T>& get_eigenvector(const shared_ptr<Factory<T>>& W) { return W->eigenvector; }

#endif // FACTORY_HPP
