#include "Workshop.h"

Workshop::Workshop(const unsigned& D, const AnalysisType& T)
    : dof_number(D)
    , analysis_type(T)
{
}

const bool& Workshop::is_symm() const { return symm_mat; }

const bool& Workshop::is_band() const { return band_mat; }

void Workshop::set_symm(const bool& B) { symm_mat = B; }

void Workshop::set_band(const bool& B) { band_mat = B; }

void Workshop::set_dof_number(const unsigned& D) { dof_number = D; }

const unsigned& Workshop::get_dof_number() const { return dof_number; }

void Workshop::set_analysis_type(const AnalysisType& T) { analysis_type = T; }

const AnalysisType& Workshop::get_analysis_type() const { return analysis_type; }

void Workshop::set_bandwidth(const unsigned& L, const unsigned& U)
{
    low_bandwidth = L;
    up_bandwidth = U;
    shifted_bandwidth = L + U;
}

void Workshop::get_bandwidth(unsigned& L, unsigned& U) const
{
    L = low_bandwidth;
    U = up_bandwidth;
}

void Workshop::set_error(const double& E) { error = E; }

const double& Workshop::get_error() const { return error; }

const bool& Workshop::is_initialized() const { return initialized; }

int Workshop::initialize()
{
    if(!initialized) initialized = true;

    if(dof_number != 0) {
        ninja.zeros(dof_number);
        switch(analysis_type) {
        case AnalysisType::DISP:
            initialize_displacement(dof_number);
            break;
        case AnalysisType::EIGEN:
            initialize_eigen(dof_number);
            initialize_mass(dof_number);
            initialize_stiffness(dof_number);
            break;
        case AnalysisType::STATICS:
            initialize_load(dof_number);
            initialize_resistance(dof_number);
            initialize_displacement(dof_number);
            initialize_stiffness(dof_number);
            break;
        case AnalysisType::DYNAMICS:
            initialize_load(dof_number);
            initialize_resistance(dof_number);
            initialize_displacement(dof_number);
            initialize_velocity(dof_number);
            initialize_acceleration(dof_number);
            initialize_mass(dof_number);
            initialize_damping(dof_number);
            initialize_stiffness(dof_number);
            break;
        case AnalysisType::NONE:
        default:
            break;
        }
    }

    return 0;
}

void Workshop::initialize_load(const unsigned& D)
{
    trial_load.zeros(D);
    incre_load.zeros(D);
    current_load.zeros(D);
}

void Workshop::initialize_resistance(const unsigned& D)
{
    trial_resistance.zeros(D);
    incre_resistance.zeros(D);
    current_resistance.zeros(D);
}

void Workshop::initialize_displacement(const unsigned& D)
{
    trial_displacement.zeros(D);
    incre_displacement.zeros(D);
    current_displacement.zeros(D);
}

void Workshop::initialize_velocity(const unsigned& D)
{
    trial_velocity.zeros(D);
    incre_velocity.zeros(D);
    current_velocity.zeros(D);
}

void Workshop::initialize_acceleration(const unsigned& D)
{
    trial_acceleration.zeros(D);
    incre_acceleration.zeros(D);
    current_acceleration.zeros(D);
}

void Workshop::initialize_temperature(const unsigned& D)
{
    trial_temperature.zeros(D);
    incre_temperature.zeros(D);
    current_temperature.zeros(D);
}

void Workshop::initialize_eigen(const unsigned& D)
{
    eigenvalue.zeros(D);
    eigenvector.zeros(D, D);
}

void Workshop::initialize_mass(const unsigned& D)
{
    if(is_band())
        if(is_symm())
            global_mass.zeros(low_bandwidth + 1, D);
        else
            global_mass.zeros(2 * low_bandwidth + up_bandwidth + 1, D);
    else
        global_mass.zeros(D, D);
}

void Workshop::initialize_damping(const unsigned& D)
{
    if(is_band())
        if(is_symm())
            global_damping.zeros(low_bandwidth + 1, D);
        else
            global_damping.zeros(2 * low_bandwidth + up_bandwidth + 1, D);
    else
        global_damping.zeros(D, D);
}

void Workshop::initialize_stiffness(const unsigned& D)
{
    if(is_band())
        if(is_symm())
            global_stiffness.zeros(low_bandwidth + 1, D);
        else
            global_stiffness.zeros(2 * low_bandwidth + up_bandwidth + 1, D);
    else
        global_stiffness.zeros(D, D);
}

void Workshop::update_ninja(const vec& N) { ninja = N; }

void Workshop::update_trial_time(const double& T)
{
    trial_time = T;
    incre_time = trial_time - current_time;
}

void Workshop::update_trial_load(const vec& L)
{
    trial_load = L;
    incre_load = trial_load - current_load;
}

void Workshop::update_trial_resistance(const vec& R)
{
    trial_resistance = R;
    incre_resistance = trial_resistance - current_resistance;
}

void Workshop::update_trial_displacement(const vec& D)
{
    trial_displacement = D;
    incre_displacement = trial_displacement - current_displacement;
}

void Workshop::update_trial_velocity(const vec& V)
{
    trial_velocity = V;
    incre_velocity = trial_velocity - current_velocity;
}

void Workshop::update_trial_acceleration(const vec& A)
{
    trial_acceleration = A;
    incre_acceleration = trial_acceleration - current_acceleration;
}

void Workshop::update_trial_temperature(const vec& T)
{
    trial_temperature = T;
    incre_temperature = trial_temperature - current_temperature;
}

void Workshop::update_incre_time(const double& T)
{
    incre_time = T;
    trial_time = current_time + incre_time;
}

void Workshop::update_incre_load(const vec& L)
{
    incre_load = L;
    trial_load = current_load + incre_load;
}

void Workshop::update_incre_resistance(const vec& R)
{
    incre_resistance = R;
    trial_resistance = current_resistance + incre_resistance;
}

void Workshop::update_incre_displacement(const vec& D)
{
    incre_displacement = D;
    trial_displacement = current_displacement + incre_displacement;
}

void Workshop::update_incre_velocity(const vec& V)
{
    incre_velocity = V;
    trial_velocity = current_velocity + incre_velocity;
}

void Workshop::update_incre_acceleration(const vec& A)
{
    incre_acceleration = A;
    trial_acceleration = current_acceleration + incre_acceleration;
}

void Workshop::update_incre_temperature(const vec& T)
{
    incre_temperature = T;
    trial_temperature = current_temperature + incre_temperature;
}

void Workshop::update_current_time(const double& T) { current_time = T; }

void Workshop::update_current_load(const vec& L) { current_load = L; }

void Workshop::update_current_resistance(const vec& R) { current_resistance = R; }

void Workshop::update_current_displacement(const vec& D) { current_displacement = D; }

void Workshop::update_current_velocity(const vec& V) { current_velocity = V; }

void Workshop::update_current_acceleration(const vec& A) { current_acceleration = A; }

void Workshop::update_current_temperature(const vec& T) { current_temperature = T; }

void Workshop::update_pre_time(const double& T) { pre_time = T; }

void Workshop::update_pre_load(const vec& L) { pre_load = L; }

void Workshop::update_pre_resistance(const vec& R) { pre_resistance = R; }

void Workshop::update_pre_displacement(const vec& D) { pre_displacement = D; }

void Workshop::update_pre_velocity(const vec& V) { pre_velocity = V; }

void Workshop::update_pre_acceleration(const vec& A) { pre_acceleration = A; }

void Workshop::update_pre_temperature(const vec& T) { pre_temperature = T; }

void Workshop::update_mass(const mat& M) { global_mass = M; }

void Workshop::update_damping(const mat& D) { global_damping = D; }

void Workshop::update_stiffness(const mat& K) { global_stiffness = K; }

const vec& Workshop::get_ninja() const { return ninja; }

const double& Workshop::get_trial_time() const { return trial_time; }

const vec& Workshop::get_trial_load() const { return trial_load; }

const vec& Workshop::get_trial_resistance() const { return trial_resistance; }

const vec& Workshop::get_trial_displacement() const { return trial_displacement; }

const vec& Workshop::get_trial_velocity() const { return trial_velocity; }

const vec& Workshop::get_trial_acceleration() const { return trial_acceleration; }

const vec& Workshop::get_trial_temperature() const { return trial_temperature; }

const double& Workshop::get_incre_time() const { return incre_time; }

const vec& Workshop::get_incre_load() const { return incre_load; }

const vec& Workshop::get_incre_resistance() const { return incre_resistance; }

const vec& Workshop::get_incre_displacement() const { return incre_displacement; }

const vec& Workshop::get_incre_velocity() const { return incre_velocity; }

const vec& Workshop::get_incre_acceleration() const { return incre_acceleration; }

const vec& Workshop::get_incre_temperature() const { return incre_temperature; }

const double& Workshop::get_current_time() const { return current_time; }

const vec& Workshop::get_current_load() const { return current_load; }

const vec& Workshop::get_current_resistance() const { return current_resistance; }

const vec& Workshop::get_current_displacement() const { return current_displacement; }

const vec& Workshop::get_current_velocity() const { return current_velocity; }

const vec& Workshop::get_current_acceleration() const { return current_acceleration; }

const vec& Workshop::get_current_temperature() const { return current_temperature; }

const double& Workshop::get_pre_time() const { return pre_time; }

const vec& Workshop::get_pre_load() const { return pre_load; }

const vec& Workshop::get_pre_resistance() const { return pre_resistance; }

const vec& Workshop::get_pre_displacement() const { return pre_displacement; }

const vec& Workshop::get_pre_velocity() const { return pre_velocity; }

const vec& Workshop::get_pre_acceleration() const { return pre_acceleration; }

const vec& Workshop::get_pre_temperature() const { return pre_temperature; }

const mat& Workshop::get_mass() const { return global_mass; }

const mat& Workshop::get_damping() const { return global_damping; }

const mat& Workshop::get_stiffness() const { return global_stiffness; }

const vec& Workshop::get_eigenvalue() const { return eigenvalue; }

const mat& Workshop::get_eigenvector() const { return eigenvector; }

void Workshop::commit_status()
{
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
    default:
        break;
    }
    // commit_temperature();
}

void Workshop::commit_time() { current_time = trial_time; }

void Workshop::commit_load() { current_load = trial_load; }

void Workshop::commit_resistance() { current_resistance = trial_resistance; }

void Workshop::commit_displacement() { current_displacement = trial_displacement; }

void Workshop::commit_velocity() { current_velocity = trial_velocity; }

void Workshop::commit_acceleration() { current_acceleration = trial_acceleration; }

void Workshop::commit_temperature() { current_temperature = trial_temperature; }

void Workshop::commit_pre_status()
{
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
    default:
        break;
    }
    // commit_pre_temperature();
}

void Workshop::commit_pre_time() { pre_time = current_time; }

void Workshop::commit_pre_load() { pre_load = current_load; }

void Workshop::commit_pre_resistance() { pre_resistance = current_resistance; }

void Workshop::commit_pre_displacement() { pre_displacement = current_displacement; }

void Workshop::commit_pre_velocity() { pre_velocity = current_velocity; }

void Workshop::commit_pre_acceleration() { pre_acceleration = current_acceleration; }

void Workshop::commit_pre_temperature() { pre_temperature = current_temperature; }

void Workshop::clear_status()
{
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

void Workshop::clear_time()
{
    trial_time = 0.;
    incre_time = 0.;
    current_time = 0.;
}

void Workshop::clear_load()
{
    if(!pre_load.is_empty()) pre_load.zeros();
    if(!trial_load.is_empty()) trial_load.zeros();
    if(!incre_load.is_empty()) incre_load.zeros();
    if(!current_load.is_empty()) current_load.zeros();
}

void Workshop::clear_resistance()
{
    if(!pre_resistance.is_empty()) pre_resistance.zeros();
    if(!trial_resistance.is_empty()) trial_resistance.zeros();
    if(!incre_resistance.is_empty()) incre_resistance.zeros();
    if(!current_resistance.is_empty()) current_resistance.zeros();
}

void Workshop::clear_displacement()
{
    if(!pre_displacement.is_empty()) pre_displacement.zeros();
    if(!trial_displacement.is_empty()) trial_displacement.zeros();
    if(!incre_displacement.is_empty()) incre_displacement.zeros();
    if(!current_displacement.is_empty()) current_displacement.zeros();
}

void Workshop::clear_velocity()
{
    if(!pre_velocity.is_empty()) pre_velocity.zeros();
    if(!trial_velocity.is_empty()) trial_velocity.zeros();
    if(!incre_velocity.is_empty()) incre_velocity.zeros();
    if(!current_velocity.is_empty()) current_velocity.zeros();
}

void Workshop::clear_acceleration()
{
    if(!pre_acceleration.is_empty()) pre_acceleration.zeros();
    if(!trial_acceleration.is_empty()) trial_acceleration.zeros();
    if(!incre_acceleration.is_empty()) incre_acceleration.zeros();
    if(!current_acceleration.is_empty()) current_acceleration.zeros();
}

void Workshop::clear_temperature()
{
    if(!pre_temperature.is_empty()) pre_temperature.zeros();
    if(!trial_temperature.is_empty()) trial_temperature.zeros();
    if(!incre_temperature.is_empty()) incre_temperature.zeros();
    if(!current_temperature.is_empty()) current_temperature.zeros();
}

void Workshop::reset_status()
{
    ninja.zeros();

    reset_time();
    reset_load();
    reset_resistance();
    reset_displacement();
    reset_velocity();
    reset_acceleration();
    reset_temperature();
}

void Workshop::reset_time()
{
    trial_time = current_time;
    incre_time = 0.;
}

void Workshop::reset_load()
{
    if(!trial_load.is_empty()) {
        trial_load = current_load;
        incre_load.zeros();
    }
}

void Workshop::reset_resistance()
{
    if(!trial_resistance.is_empty()) {
        trial_resistance = current_resistance;
        incre_resistance.zeros();
    }
}

void Workshop::reset_displacement()
{
    if(!trial_displacement.is_empty()) {
        trial_displacement = current_displacement;
        incre_displacement.zeros();
    }
}

void Workshop::reset_velocity()
{
    if(!trial_velocity.is_empty()) {
        trial_velocity = current_velocity;
        incre_velocity.zeros();
    }
}

void Workshop::reset_acceleration()
{
    if(!trial_acceleration.is_empty()) {
        trial_acceleration = current_acceleration;
        incre_acceleration.zeros();
    }
}

void Workshop::reset_temperature()
{
    if(!trial_temperature.is_empty()) {
        trial_temperature = current_temperature;
        incre_temperature.zeros();
    }
}

void Workshop::clear_eigen()
{
    eigenvalue.zeros();
    eigenvector.zeros();
}

void Workshop::clear_mass() { global_mass.zeros(); }

void Workshop::clear_damping() { global_damping.zeros(); }

void Workshop::clear_stiffness() { global_stiffness.zeros(); }

void Workshop::print() const
{
    suanpan_info("This is a Workshop object with size of %u.\n", dof_number);
}

void Workshop::assemble_resistance(const mat& ER, const uvec& EI)
{
    for(unsigned I = 0; I < EI.n_elem; ++I) trial_resistance(EI(I)) += ER(I);
}

void Workshop::assemble_mass(const mat& EM, const uvec& EI)
{
    if(is_symm() && is_band())
        for(unsigned I = 0; I < EI.n_elem; ++I)
            for(auto J = I; J < EI.n_elem; ++J)
                if(EI(J) > EI(I))
                    global_mass(EI(J) - EI(I), EI(I)) += EM(J, I);
                else
                    global_mass(EI(I) - EI(J), EI(J)) += EM(J, I);
    else if(is_symm() && !is_band())
        for(unsigned I = 0; I < EI.n_elem; ++I)
            for(auto J = I; J < EI.n_elem; ++J)
                if(EI(J) > EI(I))
                    global_mass(EI(J), EI(I)) += EM(J, I);
                else
                    global_mass(EI(I), EI(J)) += EM(J, I);
    else if(!is_symm() && !is_band())
        for(unsigned I = 0; I < EI.n_elem; ++I)
            for(unsigned J = 0; J < EI.n_elem; ++J) global_mass(EI(J), EI(I)) += EM(J, I);
    else if(!is_symm() && is_band())
        for(unsigned I = 0; I < EI.n_elem; ++I)
            for(unsigned J = 0; J < EI.n_elem; ++J)
                global_mass(EI(J) - EI(I) + shifted_bandwidth, EI(I)) += EM(J, I);
}

void Workshop::assemble_damping(const mat& EC, const uvec& EI)
{
    if(is_symm() && is_band())
        for(unsigned I = 0; I < EI.n_elem; ++I)
            for(auto J = I; J < EI.n_elem; ++J)
                if(EI(J) > EI(I))
                    global_damping(EI(J) - EI(I), EI(I)) += EC(J, I);
                else
                    global_damping(EI(I) - EI(J), EI(J)) += EC(J, I);
    else if(is_symm() && !is_band())
        for(unsigned I = 0; I < EI.n_elem; ++I)
            for(auto J = I; J < EI.n_elem; ++J)
                if(EI(J) > EI(I))
                    global_damping(EI(J), EI(I)) += EC(J, I);
                else
                    global_damping(EI(I), EI(J)) += EC(J, I);
    else if(!is_symm() && !is_band())
        for(unsigned I = 0; I < EI.n_elem; ++I)
            for(unsigned J = 0; J < EI.n_elem; ++J)
                global_damping(EI(J), EI(I)) += EC(J, I);
    else if(!is_symm() && is_band())
        for(unsigned I = 0; I < EI.n_elem; ++I)
            for(unsigned J = 0; J < EI.n_elem; ++J)
                global_damping(EI(J) - EI(I) + shifted_bandwidth, EI(I)) += EC(J, I);
}

void Workshop::assemble_stiffness(const mat& EK, const uvec& EI)
{
    if(is_symm() && is_band())
        for(unsigned I = 0; I < EI.n_elem; ++I)
            for(auto J = I; J < EI.n_elem; ++J)
                if(EI(J) > EI(I))
                    global_stiffness(EI(J) - EI(I), EI(I)) += EK(J, I);
                else
                    global_stiffness(EI(I) - EI(J), EI(J)) += EK(J, I);
    else if(is_symm() && !is_band())
        for(unsigned I = 0; I < EI.n_elem; ++I)
            for(auto J = I; J < EI.n_elem; ++J)
                if(EI(J) > EI(I))
                    global_stiffness(EI(J), EI(I)) += EK(J, I);
                else
                    global_stiffness(EI(I), EI(J)) += EK(J, I);
    else if(!is_symm() && !is_band())
        for(unsigned I = 0; I < EI.n_elem; ++I)
            for(unsigned J = 0; J < EI.n_elem; ++J)
                global_stiffness(EI(J), EI(I)) += EK(J, I);
    else if(!is_symm() && is_band())
        for(unsigned I = 0; I < EI.n_elem; ++I)
            for(unsigned J = 0; J < EI.n_elem; ++J)
                global_stiffness(EI(J) - EI(I) + shifted_bandwidth, EI(I)) += EK(J, I);
}

vec& get_ninja(const shared_ptr<Workshop>& W) { return W->ninja; }

double& get_trial_time(const shared_ptr<Workshop>& W) { return W->trial_time; }

vec& get_trial_load(const shared_ptr<Workshop>& W) { return W->trial_load; }

vec& get_trial_resistance(const shared_ptr<Workshop>& W) { return W->trial_resistance; }

vec& get_trial_displacement(const shared_ptr<Workshop>& W)
{
    return W->trial_displacement;
}

vec& get_trial_velocity(const shared_ptr<Workshop>& W) { return W->trial_velocity; }

vec& get_trial_acceleration(const shared_ptr<Workshop>& W)
{
    return W->trial_acceleration;
}

vec& get_trial_temperature(const shared_ptr<Workshop>& W) { return W->trial_temperature; }

double& get_incre_time(const shared_ptr<Workshop>& W) { return W->incre_time; }

vec& get_incre_load(const shared_ptr<Workshop>& W) { return W->incre_load; }

vec& get_incre_resistance(const shared_ptr<Workshop>& W) { return W->incre_resistance; }

vec& get_incre_displacement(const shared_ptr<Workshop>& W)
{
    return W->incre_displacement;
}

vec& get_incre_velocity(const shared_ptr<Workshop>& W) { return W->incre_velocity; }

vec& get_incre_acceleration(const shared_ptr<Workshop>& W)
{
    return W->incre_acceleration;
}

vec& get_incre_temperature(const shared_ptr<Workshop>& W) { return W->incre_temperature; }

double& get_current_time(const shared_ptr<Workshop>& W) { return W->current_time; }

vec& get_current_load(const shared_ptr<Workshop>& W) { return W->current_load; }

vec& get_current_resistance(const shared_ptr<Workshop>& W)
{
    return W->current_resistance;
}

vec& get_current_displacement(const shared_ptr<Workshop>& W)
{
    return W->current_displacement;
}

vec& get_current_velocity(const shared_ptr<Workshop>& W) { return W->current_velocity; }

vec& get_current_acceleration(const shared_ptr<Workshop>& W)
{
    return W->current_acceleration;
}

vec& get_current_temperature(const shared_ptr<Workshop>& W)
{
    return W->current_temperature;
}

double& get_pre_time(const shared_ptr<Workshop>& W) { return W->pre_time; }

vec& get_pre_load(const shared_ptr<Workshop>& W) { return W->pre_load; }

vec& get_pre_resistance(const shared_ptr<Workshop>& W) { return W->pre_resistance; }

vec& get_pre_displacement(const shared_ptr<Workshop>& W) { return W->pre_displacement; }

vec& get_pre_velocity(const shared_ptr<Workshop>& W) { return W->pre_velocity; }

vec& get_pre_acceleration(const shared_ptr<Workshop>& W) { return W->pre_acceleration; }

vec& get_pre_temperature(const shared_ptr<Workshop>& W) { return W->pre_temperature; }

mat& get_mass(const shared_ptr<Workshop>& W) { return W->global_mass; }

mat& get_damping(const shared_ptr<Workshop>& W) { return W->global_damping; }

mat& get_stiffness(const shared_ptr<Workshop>& W) { return W->global_stiffness; }

vec& get_eigenvalue(const shared_ptr<Workshop>& W) { return W->eigenvalue; }

mat& get_eigenvector(const shared_ptr<Workshop>& W) { return W->eigenvector; }