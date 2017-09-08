#pragma once

template <typename T>
Factory<T>::Factory(const unsigned& D, const AnalysisType& AT, const StorageScheme& SS)
    : n_size(D)
    , analysis_type(AT)
    , storage_type(SS) {}

template <typename T>
void Factory<T>::set_size(const unsigned& D) {
    n_size = D;
}

template <typename T>
const unsigned& Factory<T>::get_size() const {
    return n_size;
}

template <typename T>
void Factory<T>::set_analysis_type(const AnalysisType& AT) {
    analysis_type = AT;
}

template <typename T>
const AnalysisType& Factory<T>::get_analysis_type() const {
    return analysis_type;
}

template <typename T>
void Factory<T>::set_storage_scheme(const StorageScheme& SS) {
    storage_type = SS;
}

template <typename T>
const StorageScheme& Factory<T>::get_storage_scheme() const {
    return storage_type;
}

template <typename T>
void Factory<T>::set_bandwidth(const unsigned& L, const unsigned& U) {
    n_lobw = L;
    n_upbw = U;
    n_sfbw = L + U;
}

template <typename T>
void Factory<T>::get_bandwidth(unsigned& L, unsigned& U) const {
    L = n_lobw;
    U = n_upbw;
}

template <typename T>
void Factory<T>::set_error(const T& E) {
    error = E;
}

template <typename T>
const T& Factory<T>::get_error() const {
    return error;
}

template <typename T>
const bool& Factory<T>::is_initialized() const {
    return initialized;
}

template <typename T>
int Factory<T>::initialize() {
    if(!initialized) initialized = true;

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

template <typename T>
void Factory<T>::initialize_load(const unsigned& D) {
    trial_load.zeros(D);
    incre_load.zeros(D);
    current_load.zeros(D);
}

template <typename T>
void Factory<T>::initialize_resistance(const unsigned& D) {
    trial_resistance.zeros(D);
    incre_resistance.zeros(D);
    current_resistance.zeros(D);
}

template <typename T>
void Factory<T>::initialize_displacement(const unsigned& D) {
    trial_displacement.zeros(D);
    incre_displacement.zeros(D);
    current_displacement.zeros(D);
}

template <typename T>
void Factory<T>::initialize_velocity(const unsigned& D) {
    trial_velocity.zeros(D);
    incre_velocity.zeros(D);
    current_velocity.zeros(D);
}

template <typename T>
void Factory<T>::initialize_acceleration(const unsigned& D) {
    trial_acceleration.zeros(D);
    incre_acceleration.zeros(D);
    current_acceleration.zeros(D);
}

template <typename T>
void Factory<T>::initialize_temperature(const unsigned& D) {
    trial_temperature.zeros(D);
    incre_temperature.zeros(D);
    current_temperature.zeros(D);
}

template <typename T>
void Factory<T>::initialize_eigen(const unsigned& D) {
    eigenvalue.zeros(D);
    eigenvector.zeros(D, D);
}

template <typename T>
void Factory<T>::initialize_mass(const unsigned& D) {
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

template <typename T>
void Factory<T>::initialize_damping(const unsigned& D) {
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

template <typename T>
void Factory<T>::initialize_stiffness(const unsigned& D) {
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

template <typename T>
void Factory<T>::update_ninja(const Col<T>& N) {
    ninja = N;
}

template <typename T>
void Factory<T>::update_trial_time(const T& M) {
    trial_time = M;
    incre_time = trial_time - current_time;
}

template <typename T>
void Factory<T>::update_trial_load(const Col<T>& L) {
    trial_load = L;
    incre_load = trial_load - current_load;
}

template <typename T>
void Factory<T>::update_trial_resistance(const Col<T>& R) {
    trial_resistance = R;
    incre_resistance = trial_resistance - current_resistance;
}

template <typename T>
void Factory<T>::update_trial_displacement(const Col<T>& D) {
    trial_displacement = D;
    incre_displacement = trial_displacement - current_displacement;
}

template <typename T>
void Factory<T>::update_trial_velocity(const Col<T>& V) {
    trial_velocity = V;
    incre_velocity = trial_velocity - current_velocity;
}

template <typename T>
void Factory<T>::update_trial_acceleration(const Col<T>& A) {
    trial_acceleration = A;
    incre_acceleration = trial_acceleration - current_acceleration;
}

template <typename T>
void Factory<T>::update_trial_temperature(const Col<T>& M) {
    trial_temperature = M;
    incre_temperature = trial_temperature - current_temperature;
}

template <typename T>
void Factory<T>::update_incre_time(const T& M) {
    incre_time = M;
    trial_time = current_time + incre_time;
}

template <typename T>
void Factory<T>::update_incre_load(const Col<T>& L) {
    incre_load = L;
    trial_load = current_load + incre_load;
}

template <typename T>
void Factory<T>::update_incre_resistance(const Col<T>& R) {
    incre_resistance = R;
    trial_resistance = current_resistance + incre_resistance;
}

template <typename T>
void Factory<T>::update_incre_displacement(const Col<T>& D) {
    incre_displacement = D;
    trial_displacement = current_displacement + incre_displacement;
}

template <typename T>
void Factory<T>::update_incre_velocity(const Col<T>& V) {
    incre_velocity = V;
    trial_velocity = current_velocity + incre_velocity;
}

template <typename T>
void Factory<T>::update_incre_acceleration(const Col<T>& A) {
    incre_acceleration = A;
    trial_acceleration = current_acceleration + incre_acceleration;
}

template <typename T>
void Factory<T>::update_incre_temperature(const Col<T>& M) {
    incre_temperature = M;
    trial_temperature = current_temperature + incre_temperature;
}

template <typename T>
void Factory<T>::update_current_time(const T& M) {
    current_time = M;
}

template <typename T>
void Factory<T>::update_current_load(const Col<T>& L) {
    current_load = L;
}

template <typename T>
void Factory<T>::update_current_resistance(const Col<T>& R) {
    current_resistance = R;
}

template <typename T>
void Factory<T>::update_current_displacement(const Col<T>& D) {
    current_displacement = D;
}

template <typename T>
void Factory<T>::update_current_velocity(const Col<T>& V) {
    current_velocity = V;
}

template <typename T>
void Factory<T>::update_current_acceleration(const Col<T>& A) {
    current_acceleration = A;
}

template <typename T>
void Factory<T>::update_current_temperature(const Col<T>& M) {
    current_temperature = M;
}

template <typename T>
void Factory<T>::update_pre_time(const T& M) {
    pre_time = M;
}

template <typename T>
void Factory<T>::update_pre_load(const Col<T>& L) {
    pre_load = L;
}

template <typename T>
void Factory<T>::update_pre_resistance(const Col<T>& R) {
    pre_resistance = R;
}

template <typename T>
void Factory<T>::update_pre_displacement(const Col<T>& D) {
    pre_displacement = D;
}

template <typename T>
void Factory<T>::update_pre_velocity(const Col<T>& V) {
    pre_velocity = V;
}

template <typename T>
void Factory<T>::update_pre_acceleration(const Col<T>& A) {
    pre_acceleration = A;
}

template <typename T>
void Factory<T>::update_pre_temperature(const Col<T>& M) {
    pre_temperature = M;
}

template <typename T>
void Factory<T>::update_mass(const shared_ptr<MetaMat<T>>& M) {
    global_mass = M;
}

template <typename T>
void Factory<T>::update_damping(const shared_ptr<MetaMat<T>>& D) {
    global_damping = D;
}

template <typename T>
void Factory<T>::update_stiffness(const shared_ptr<MetaMat<T>>& K) {
    global_stiffness = K;
}

template <typename T>
const Col<T>& Factory<T>::get_ninja() const {
    return ninja;
}

template <typename T>
const T& Factory<T>::get_trial_time() const {
    return trial_time;
}

template <typename T>
const Col<T>& Factory<T>::get_trial_load() const {
    return trial_load;
}

template <typename T>
const Col<T>& Factory<T>::get_trial_resistance() const {
    return trial_resistance;
}

template <typename T>
const Col<T>& Factory<T>::get_trial_displacement() const {
    return trial_displacement;
}

template <typename T>
const Col<T>& Factory<T>::get_trial_velocity() const {
    return trial_velocity;
}

template <typename T>
const Col<T>& Factory<T>::get_trial_acceleration() const {
    return trial_acceleration;
}

template <typename T>
const Col<T>& Factory<T>::get_trial_temperature() const {
    return trial_temperature;
}

template <typename T>
const T& Factory<T>::get_incre_time() const {
    return incre_time;
}

template <typename T>
const Col<T>& Factory<T>::get_incre_load() const {
    return incre_load;
}

template <typename T>
const Col<T>& Factory<T>::get_incre_resistance() const {
    return incre_resistance;
}

template <typename T>
const Col<T>& Factory<T>::get_incre_displacement() const {
    return incre_displacement;
}

template <typename T>
const Col<T>& Factory<T>::get_incre_velocity() const {
    return incre_velocity;
}

template <typename T>
const Col<T>& Factory<T>::get_incre_acceleration() const {
    return incre_acceleration;
}

template <typename T>
const Col<T>& Factory<T>::get_incre_temperature() const {
    return incre_temperature;
}

template <typename T>
const T& Factory<T>::get_current_time() const {
    return current_time;
}

template <typename T>
const Col<T>& Factory<T>::get_current_load() const {
    return current_load;
}

template <typename T>
const Col<T>& Factory<T>::get_current_resistance() const {
    return current_resistance;
}

template <typename T>
const Col<T>& Factory<T>::get_current_displacement() const {
    return current_displacement;
}

template <typename T>
const Col<T>& Factory<T>::get_current_velocity() const {
    return current_velocity;
}

template <typename T>
const Col<T>& Factory<T>::get_current_acceleration() const {
    return current_acceleration;
}

template <typename T>
const Col<T>& Factory<T>::get_current_temperature() const {
    return current_temperature;
}

template <typename T>
const T& Factory<T>::get_pre_time() const {
    return pre_time;
}

template <typename T>
const Col<T>& Factory<T>::get_pre_load() const {
    return pre_load;
}

template <typename T>
const Col<T>& Factory<T>::get_pre_resistance() const {
    return pre_resistance;
}

template <typename T>
const Col<T>& Factory<T>::get_pre_displacement() const {
    return pre_displacement;
}

template <typename T>
const Col<T>& Factory<T>::get_pre_velocity() const {
    return pre_velocity;
}

template <typename T>
const Col<T>& Factory<T>::get_pre_acceleration() const {
    return pre_acceleration;
}

template <typename T>
const Col<T>& Factory<T>::get_pre_temperature() const {
    return pre_temperature;
}

template <typename T>
const shared_ptr<MetaMat<T>>& Factory<T>::get_mass() const {
    return global_mass;
}

template <typename T>
const shared_ptr<MetaMat<T>>& Factory<T>::get_damping() const {
    return global_damping;
}

template <typename T>
const shared_ptr<MetaMat<T>>& Factory<T>::get_stiffness() const {
    return global_stiffness;
}

template <typename T>
const Col<T>& Factory<T>::get_eigenvalue() const {
    return eigenvalue;
}

template <typename T>
const Mat<T>& Factory<T>::get_eigenvector() const {
    return eigenvector;
}

template <typename T>
void Factory<T>::commit_status() {
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

template <typename T>
void Factory<T>::commit_time() {
    current_time = trial_time;
}

template <typename T>
void Factory<T>::commit_load() {
    current_load = trial_load;
}

template <typename T>
void Factory<T>::commit_resistance() {
    current_resistance = trial_resistance;
}

template <typename T>
void Factory<T>::commit_displacement() {
    current_displacement = trial_displacement;
}

template <typename T>
void Factory<T>::commit_velocity() {
    current_velocity = trial_velocity;
}

template <typename T>
void Factory<T>::commit_acceleration() {
    current_acceleration = trial_acceleration;
}

template <typename T>
void Factory<T>::commit_temperature() {
    current_temperature = trial_temperature;
}

template <typename T>
void Factory<T>::commit_pre_status() {
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

template <typename T>
void Factory<T>::commit_pre_time() {
    pre_time = current_time;
}

template <typename T>
void Factory<T>::commit_pre_load() {
    pre_load = current_load;
}

template <typename T>
void Factory<T>::commit_pre_resistance() {
    pre_resistance = current_resistance;
}

template <typename T>
void Factory<T>::commit_pre_displacement() {
    pre_displacement = current_displacement;
}

template <typename T>
void Factory<T>::commit_pre_velocity() {
    pre_velocity = current_velocity;
}

template <typename T>
void Factory<T>::commit_pre_acceleration() {
    pre_acceleration = current_acceleration;
}

template <typename T>
void Factory<T>::commit_pre_temperature() {
    pre_temperature = current_temperature;
}

template <typename T>
void Factory<T>::clear_status() {
    if(!initialized) return;

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

template <typename T>
void Factory<T>::clear_time() {
    trial_time = 0.;
    incre_time = 0.;
    current_time = 0.;
}

template <typename T>
void Factory<T>::clear_load() {
    if(!pre_load.is_empty()) pre_load.zeros();
    if(!trial_load.is_empty()) trial_load.zeros();
    if(!incre_load.is_empty()) incre_load.zeros();
    if(!current_load.is_empty()) current_load.zeros();
}

template <typename T>
void Factory<T>::clear_resistance() {
    if(!pre_resistance.is_empty()) pre_resistance.zeros();
    if(!trial_resistance.is_empty()) trial_resistance.zeros();
    if(!incre_resistance.is_empty()) incre_resistance.zeros();
    if(!current_resistance.is_empty()) current_resistance.zeros();
}

template <typename T>
void Factory<T>::clear_displacement() {
    if(!pre_displacement.is_empty()) pre_displacement.zeros();
    if(!trial_displacement.is_empty()) trial_displacement.zeros();
    if(!incre_displacement.is_empty()) incre_displacement.zeros();
    if(!current_displacement.is_empty()) current_displacement.zeros();
}

template <typename T>
void Factory<T>::clear_velocity() {
    if(!pre_velocity.is_empty()) pre_velocity.zeros();
    if(!trial_velocity.is_empty()) trial_velocity.zeros();
    if(!incre_velocity.is_empty()) incre_velocity.zeros();
    if(!current_velocity.is_empty()) current_velocity.zeros();
}

template <typename T>
void Factory<T>::clear_acceleration() {
    if(!pre_acceleration.is_empty()) pre_acceleration.zeros();
    if(!trial_acceleration.is_empty()) trial_acceleration.zeros();
    if(!incre_acceleration.is_empty()) incre_acceleration.zeros();
    if(!current_acceleration.is_empty()) current_acceleration.zeros();
}

template <typename T>
void Factory<T>::clear_temperature() {
    if(!pre_temperature.is_empty()) pre_temperature.zeros();
    if(!trial_temperature.is_empty()) trial_temperature.zeros();
    if(!incre_temperature.is_empty()) incre_temperature.zeros();
    if(!current_temperature.is_empty()) current_temperature.zeros();
}

template <typename T>
void Factory<T>::reset_status() {
    ninja.zeros();

    reset_time();
    reset_load();
    reset_resistance();
    reset_displacement();
    reset_velocity();
    reset_acceleration();
    reset_temperature();
}

template <typename T>
void Factory<T>::reset_time() {
    trial_time = current_time;
    incre_time = 0.;
}

template <typename T>
void Factory<T>::reset_load() {
    if(!trial_load.is_empty()) {
        trial_load = current_load;
        incre_load.zeros();
    }
}

template <typename T>
void Factory<T>::reset_resistance() {
    if(!trial_resistance.is_empty()) {
        trial_resistance = current_resistance;
        incre_resistance.zeros();
    }
}

template <typename T>
void Factory<T>::reset_displacement() {
    if(!trial_displacement.is_empty()) {
        trial_displacement = current_displacement;
        incre_displacement.zeros();
    }
}

template <typename T>
void Factory<T>::reset_velocity() {
    if(!trial_velocity.is_empty()) {
        trial_velocity = current_velocity;
        incre_velocity.zeros();
    }
}

template <typename T>
void Factory<T>::reset_acceleration() {
    if(!trial_acceleration.is_empty()) {
        trial_acceleration = current_acceleration;
        incre_acceleration.zeros();
    }
}

template <typename T>
void Factory<T>::reset_temperature() {
    if(!trial_temperature.is_empty()) {
        trial_temperature = current_temperature;
        incre_temperature.zeros();
    }
}

template <typename T>
void Factory<T>::clear_eigen() {
    eigenvalue.zeros();
    eigenvector.zeros();
}

template <typename T>
void Factory<T>::clear_mass() {
    *global_mass.zeros();
}

template <typename T>
void Factory<T>::clear_damping() {
    *global_damping.zeros();
}

template <typename T>
void Factory<T>::clear_stiffness() {
    *global_stiffness.zeros();
}

template <typename T>
void Factory<T>::print() const {
    suanpan_info("This is a Factory object with size of %u.\n", n_size);
}

template <typename T>
void Factory<T>::assemble_resistance(const Mat<T>& ER, const uvec& EI) {
    for(unsigned I = 0; I < EI.n_elem; ++I) trial_resistance(EI(I)) += ER(I);
}

template <typename T>
void Factory<T>::assemble_mass(const Mat<T>& EM, const uvec& EI) {
    for(unsigned I = 0; I < EI.n_elem; ++I)
        for(unsigned J = 0; J < EI.n_elem; ++J) *global_mass(EI(J), EI(I)) += EM(J, I);
}

template <typename T>
void Factory<T>::assemble_damping(const Mat<T>& EC, const uvec& EI) {
    for(unsigned I = 0; I < EI.n_elem; ++I)
        for(unsigned J = 0; J < EI.n_elem; ++J) *global_damping(EI(J), EI(I)) += EC(J, I);
}

template <typename T>
void Factory<T>::assemble_stiffness(const Mat<T>& EK, const uvec& EI) {
    for(unsigned I = 0; I < EI.n_elem; ++I)
        for(unsigned J = 0; J < EI.n_elem; ++J) *global_stiffness(EI(J), EI(I)) += EK(J, I);
}

template <typename T>
Col<T>& get_ninja(const shared_ptr<Factory<T>>& W) {
    return W->ninja;
}

template <typename T>
T& get_trial_time(const shared_ptr<Factory<T>>& W) {
    return W->trial_time;
}

template <typename T>
Col<T>& get_trial_load(const shared_ptr<Factory<T>>& W) {
    return W->trial_load;
}

template <typename T>
Col<T>& get_trial_resistance(const shared_ptr<Factory<T>>& W) {
    return W->trial_resistance;
}

template <typename T>
Col<T>& get_trial_displacement(const shared_ptr<Factory<T>>& W) {
    return W->trial_displacement;
}

template <typename T>
Col<T>& get_trial_velocity(const shared_ptr<Factory<T>>& W) {
    return W->trial_velocity;
}

template <typename T>
Col<T>& get_trial_acceleration(const shared_ptr<Factory<T>>& W) {
    return W->trial_acceleration;
}

template <typename T>
Col<T>& get_trial_temperature(const shared_ptr<Factory<T>>& W) {
    return W->trial_temperature;
}

template <typename T>
T& get_incre_time(const shared_ptr<Factory<T>>& W) {
    return W->incre_time;
}

template <typename T>
Col<T>& get_incre_load(const shared_ptr<Factory<T>>& W) {
    return W->incre_load;
}

template <typename T>
Col<T>& get_incre_resistance(const shared_ptr<Factory<T>>& W) {
    return W->incre_resistance;
}

template <typename T>
Col<T>& get_incre_displacement(const shared_ptr<Factory<T>>& W) {
    return W->incre_displacement;
}

template <typename T>
Col<T>& get_incre_velocity(const shared_ptr<Factory<T>>& W) {
    return W->incre_velocity;
}

template <typename T>
Col<T>& get_incre_acceleration(const shared_ptr<Factory<T>>& W) {
    return W->incre_acceleration;
}

template <typename T>
Col<T>& get_incre_temperature(const shared_ptr<Factory<T>>& W) {
    return W->incre_temperature;
}

template <typename T>
T& get_current_time(const shared_ptr<Factory<T>>& W) {
    return W->current_time;
}

template <typename T>
Col<T>& get_current_load(const shared_ptr<Factory<T>>& W) {
    return W->current_load;
}

template <typename T>
Col<T>& get_current_resistance(const shared_ptr<Factory<T>>& W) {
    return W->current_resistance;
}

template <typename T>
Col<T>& get_current_displacement(const shared_ptr<Factory<T>>& W) {
    return W->current_displacement;
}

template <typename T>
Col<T>& get_current_velocity(const shared_ptr<Factory<T>>& W) {
    return W->current_velocity;
}

template <typename T>
Col<T>& get_current_acceleration(const shared_ptr<Factory<T>>& W) {
    return W->current_acceleration;
}

template <typename T>
Col<T>& get_current_temperature(const shared_ptr<Factory<T>>& W) {
    return W->current_temperature;
}

template <typename T>
T& get_pre_time(const shared_ptr<Factory<T>>& W) {
    return W->pre_time;
}

template <typename T>
Col<T>& get_pre_load(const shared_ptr<Factory<T>>& W) {
    return W->pre_load;
}

template <typename T>
Col<T>& get_pre_resistance(const shared_ptr<Factory<T>>& W) {
    return W->pre_resistance;
}

template <typename T>
Col<T>& get_pre_displacement(const shared_ptr<Factory<T>>& W) {
    return W->pre_displacement;
}

template <typename T>
Col<T>& get_pre_velocity(const shared_ptr<Factory<T>>& W) {
    return W->pre_velocity;
}

template <typename T>
Col<T>& get_pre_acceleration(const shared_ptr<Factory<T>>& W) {
    return W->pre_acceleration;
}

template <typename T>
Col<T>& get_pre_temperature(const shared_ptr<Factory<T>>& W) {
    return W->pre_temperature;
}

template <typename T>
shared_ptr<MetaMat<T>>& get_mass(const shared_ptr<Factory<T>>& W) {
    return W->global_mass;
}

template <typename T>
shared_ptr<MetaMat<T>>& get_damping(const shared_ptr<Factory<T>>& W) {
    return W->global_damping;
}

template <typename T>
shared_ptr<MetaMat<T>>& get_stiffness(const shared_ptr<Factory<T>>& W) {
    return W->global_stiffness;
}

template <typename T>
Col<T>& get_eigenvalue(const shared_ptr<Factory<T>>& W) {
    return W->eigenvalue;
}

template <typename T>
Mat<T>& get_eigenvector(const shared_ptr<Factory<T>>& W) {
    return W->eigenvector;
}
