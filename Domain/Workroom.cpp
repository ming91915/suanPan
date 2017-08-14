#include "Workroom.h"

Workroom::Workroom(const unsigned& D, const unsigned& T)
    : number_dof(D)
    , analysis_type(T)
{
}

const bool& Workroom::is_symm() const { return symm_mat; }

const bool& Workroom::is_band() const { return band_mat; }

void Workroom::enable_symm() { symm_mat = true; }

void Workroom::disable_symm() { symm_mat = false; }

void Workroom::enable_band() { band_mat = true; }

void Workroom::disable_band() { band_mat = false; }

void Workroom::setNumberDOF(const unsigned& D) { number_dof = D; }

const unsigned& Workroom::getNumberDOF() const { return number_dof; }

void Workroom::setAnalysisType(const unsigned& T) { analysis_type = T; }

const unsigned& Workroom::getAnalysisType() const { return analysis_type; }

void Workroom::setBandwidth(const unsigned& L, const unsigned& U)
{
    low_bandwidth = L;
    up_bandwidth = U;
}

void Workroom::getBandwidth(unsigned& L, unsigned& U) const
{
    L = low_bandwidth;
    U = up_bandwidth;
}

void Workroom::setError(const double& E) { error = E; }

const double& Workroom::getError() const { return error; }

void Workroom::initialize(const unsigned& S)
{
    auto D = S == 0 ? number_dof : S;

    if(D != 0) {
        ninja.zeros(D);
        switch(analysis_type) {
        case SUANPAN_DISP:
            initializeDisplacement(D);
            break;
        case SUANPAN_BUCKLE:
            initializeEigen(D);
            initializeMass(D);
            initializeStiffness(D);
            break;
        case SUANPAN_STATICS:
            initializeLoad(D);
            initializeResistance(D);
            initializeDisplacement(D);
            initializeMass(D);
            initializeStiffness(D);
            break;
        case SUANPAN_DYNAMICS:
            initializeLoad(D);
            initializeResistance(D);
            initializeDisplacement(D);
            initializeVelocity(D);
            initializeAcceleration(D);
            initializeMass(D);
            initializeDamping(D);
            initializeStiffness(D);
            break;
        case SUANPAN_NONE:
        default:
            break;
        }
    }
}

void Workroom::initializeLoad(const unsigned& D)
{
    trial_load.zeros(D);
    incre_load.zeros(D);
    current_load.zeros(D);
}

void Workroom::initializeResistance(const unsigned& D)
{
    trial_resistance.zeros(D);
    incre_resistance.zeros(D);
    current_resistance.zeros(D);
}

void Workroom::initializeDisplacement(const unsigned& D)
{
    trial_displacement.zeros(D);
    incre_displacement.zeros(D);
    current_displacement.zeros(D);
}

void Workroom::initializeVelocity(const unsigned& D)
{
    trial_velocity.zeros(D);
    incre_velocity.zeros(D);
    current_velocity.zeros(D);
}

void Workroom::initializeAcceleration(const unsigned& D)
{
    trial_acceleration.zeros(D);
    incre_acceleration.zeros(D);
    current_acceleration.zeros(D);
}

void Workroom::initializeTemperature(const unsigned& D)
{
    trial_temperature.zeros(D);
    incre_temperature.zeros(D);
    current_temperature.zeros(D);
}

void Workroom::initializeEigen(const unsigned& D)
{
    eigenvalue.zeros(D);
    eigenvector.zeros(D, D);
}

void Workroom::initializeMass(const unsigned& D)
{
    if(is_band())
        if(is_symm())
            global_mass.zeros(low_bandwidth + 1, D);
        else
            global_mass.zeros(2 * low_bandwidth + up_bandwidth + 1, D);
    else
        global_mass.zeros(D, D);
}

void Workroom::initializeDamping(const unsigned& D)
{
    if(is_band())
        if(is_symm())
            global_damping.zeros(low_bandwidth + 1, D);
        else
            global_damping.zeros(2 * low_bandwidth + up_bandwidth + 1, D);
    else
        global_damping.zeros(D, D);
}

void Workroom::initializeStiffness(const unsigned& D)
{
    if(is_band())
        if(is_symm())
            global_stiffness.zeros(low_bandwidth + 1, D);
        else
            global_stiffness.zeros(2 * low_bandwidth + up_bandwidth + 1, D);
    else
        global_stiffness.zeros(D, D);
}

void Workroom::updateNinja(const vec& N) { ninja = N; }

void Workroom::updateTrialTime(const double& T)
{
    trial_time = T;
    incre_time = trial_time - current_time;
}

void Workroom::updateTrialLoad(const vec& L)
{
    trial_load = L;
    incre_load = trial_load - current_load;
}

void Workroom::updateTrialResistance(const vec& R)
{
    trial_resistance = R;
    incre_resistance = trial_resistance - current_resistance;
}

void Workroom::updateTrialDisplacement(const vec& D)
{
    trial_displacement = D;
    incre_displacement = trial_displacement - current_displacement;
}

void Workroom::updateTrialVelocity(const vec& V)
{
    trial_velocity = V;
    incre_velocity = trial_velocity - current_velocity;
}

void Workroom::updateTrialAcceleration(const vec& A)
{
    trial_acceleration = A;
    incre_acceleration = trial_acceleration - current_acceleration;
}

void Workroom::updateTrialTemperature(const vec& T)
{
    trial_temperature = T;
    incre_temperature = trial_temperature - current_temperature;
}

void Workroom::updateIncreTime(const double& T)
{
    incre_time = T;
    trial_time = current_time + incre_time;
}

void Workroom::updateIncreLoad(const vec& L)
{
    incre_load = L;
    trial_load = current_load + incre_load;
}

void Workroom::updateIncreResistance(const vec& R)
{
    incre_resistance = R;
    trial_resistance = current_resistance + incre_resistance;
}

void Workroom::updateIncreDisplacement(const vec& D)
{
    incre_displacement = D;
    trial_displacement = current_displacement + incre_displacement;
}

void Workroom::updateIncreVelocity(const vec& V)
{
    incre_velocity = V;
    trial_velocity = current_velocity + incre_velocity;
}

void Workroom::updateIncreAcceleration(const vec& A)
{
    incre_acceleration = A;
    trial_acceleration = current_acceleration + incre_acceleration;
}

void Workroom::updateIncreTemperature(const vec& T)
{
    incre_temperature = T;
    trial_temperature = current_temperature + incre_temperature;
}

void Workroom::updateCurrentTime(const double& T) { current_time = T; }

void Workroom::updateCurrentLoad(const vec& L) { current_load = L; }

void Workroom::updateCurrentResistance(const vec& R) { current_resistance = R; }

void Workroom::updateCurrentDisplacement(const vec& D) { current_displacement = D; }

void Workroom::updateCurrentVelocity(const vec& V) { current_velocity = V; }

void Workroom::updateCurrentAcceleration(const vec& A) { current_acceleration = A; }

void Workroom::updateCurrentTemperature(const vec& T) { current_temperature = T; }

void Workroom::updatePreviousLoad(const vec& L) { pre_load = L; }

void Workroom::updatePreviousResistance(const vec& R) { pre_resistance = R; }

void Workroom::updatePreviousDisplacement(const vec& D) { pre_displacement = D; }

void Workroom::updatePreviousVelocity(const vec& V) { pre_velocity = V; }

void Workroom::updatePreviousAcceleration(const vec& A) { pre_acceleration = A; }

void Workroom::updatePreviousTemperature(const vec& T) { pre_temperature = T; }

void Workroom::updateMass(const T2& M) { global_mass = M; }

void Workroom::updateDamping(const T2& D) { global_damping = D; }

void Workroom::updateStiffness(const T2& K) { global_stiffness = K; }

const vec& Workroom::getNinja() const { return ninja; }

const double& Workroom::getTrialTime() const { return trial_time; }

const vec& Workroom::getTrialLoad() const { return trial_load; }

const vec& Workroom::getTrialResistance() const { return trial_resistance; }

const vec& Workroom::getTrialDisplacement() const { return trial_displacement; }

const vec& Workroom::getTrialVelocity() const { return trial_velocity; }

const vec& Workroom::getTrialAcceleration() const { return trial_acceleration; }

const vec& Workroom::getTrialTemperature() const { return trial_temperature; }

const double& Workroom::getIncreTime() const { return incre_time; }

const vec& Workroom::getIncreLoad() const { return incre_load; }

const vec& Workroom::getIncreResistance() const { return incre_resistance; }

const vec& Workroom::getIncreDisplacement() const { return incre_displacement; }

const vec& Workroom::getIncreVelocity() const { return incre_velocity; }

const vec& Workroom::getIncreAcceleration() const { return incre_acceleration; }

const vec& Workroom::getIncreTemperature() const { return incre_temperature; }

const double& Workroom::getCurrentTime() const { return current_time; }

const vec& Workroom::getCurrentLoad() const { return current_load; }

const vec& Workroom::getCurrentResistance() const { return current_resistance; }

const vec& Workroom::getCurrentDisplacement() const { return current_displacement; }

const vec& Workroom::getCurrentVelocity() const { return current_velocity; }

const vec& Workroom::getCurrentAcceleration() const { return current_acceleration; }

const vec& Workroom::getCurrentTemperature() const { return current_temperature; }

const vec& Workroom::getPreviousLoad() const { return pre_load; }

const vec& Workroom::getPreviousResistance() const { return pre_resistance; }

const vec& Workroom::getPreviousDisplacement() const { return pre_displacement; }

const vec& Workroom::getPreviousVelocity() const { return pre_velocity; }

const vec& Workroom::getPreviousAcceleration() const { return pre_acceleration; }

const vec& Workroom::getPreviousTemperature() const { return pre_temperature; }

const T2& Workroom::getMass() const { return global_mass; }

const T2& Workroom::getDamping() const { return global_damping; }

const T2& Workroom::getStiffness() const { return global_stiffness; }

const vec& Workroom::getEigenvalue() const { return eigenvalue; }

const mat& Workroom::getEigenvector() const { return eigenvector; }

void Workroom::commitStatus(const unsigned& T)
{
    commitTime();

    switch(T) {
    case SUANPAN_DISP:
        commitDisplacement();
        break;
    case SUANPAN_BUCKLE:
        break;
    case SUANPAN_STATICS:
        commitLoad();
        commitDisplacement();
        break;
    case SUANPAN_DYNAMICS:
        commitLoad();
        commitDisplacement();
        commitVelocity();
        commitAcceleration();
        break;
    default:
        break;
    }
    // commitTemperature();
}

void Workroom::commitTime() { current_time = trial_time; }

void Workroom::commitLoad() { current_load = trial_load; }

void Workroom::commitResistance() { current_resistance = trial_resistance; }

void Workroom::commitDisplacement() { current_displacement = trial_displacement; }

void Workroom::commitVelocity() { current_velocity = trial_velocity; }

void Workroom::commitAcceleration() { current_acceleration = trial_acceleration; }

void Workroom::commitTemperature() { current_temperature = trial_temperature; }

void Workroom::commitPreStatus(const unsigned& T)
{
    switch(T) {
    case SUANPAN_DISP:
        commitPreDisplacement();
        break;
    case SUANPAN_BUCKLE:
        break;
    case SUANPAN_STATICS:
        commitPreLoad();
        commitPreDisplacement();
        break;
    case SUANPAN_DYNAMICS:
        commitPreLoad();
        commitPreDisplacement();
        commitPreVelocity();
        commitPreAcceleration();
        break;
    default:
        break;
    }
    // commitPreTemperature();
}

void Workroom::commitPreLoad() { pre_load = current_load; }

void Workroom::commitPreResistance() { pre_resistance = current_resistance; }

void Workroom::commitPreDisplacement() { pre_displacement = current_displacement; }

void Workroom::commitPreVelocity() { pre_velocity = current_velocity; }

void Workroom::commitPreAcceleration() { pre_acceleration = current_acceleration; }

void Workroom::commitPreTemperature() { pre_temperature = current_temperature; }

void Workroom::clearStatus()
{
    clearTime();
    clearLoad();
    clearResistance();
    clearDisplacement();
    clearVelocity();
    clearAcceleration();
    clearTemperature();

    clearEigen();
    clearMass();
    clearDamping();
    clearStiffness();
}

void Workroom::clearTime()
{
    trial_time = 0.;
    incre_time = 0.;
    current_time = 0.;
}

void Workroom::clearLoad()
{
    if(!pre_load.is_empty()) pre_load.zeros();
    if(!trial_load.is_empty()) trial_load.zeros();
    if(!incre_load.is_empty()) incre_load.zeros();
    if(!current_load.is_empty()) current_load.zeros();
}

void Workroom::clearResistance()
{
    if(!pre_resistance.is_empty()) pre_resistance.zeros();
    if(!trial_resistance.is_empty()) trial_resistance.zeros();
    if(!incre_resistance.is_empty()) incre_resistance.zeros();
    if(!current_resistance.is_empty()) current_resistance.zeros();
}

void Workroom::clearDisplacement()
{
    if(!pre_displacement.is_empty()) pre_displacement.zeros();
    if(!trial_displacement.is_empty()) trial_displacement.zeros();
    if(!incre_displacement.is_empty()) incre_displacement.zeros();
    if(!current_displacement.is_empty()) current_displacement.zeros();
}

void Workroom::clearVelocity()
{
    if(!pre_velocity.is_empty()) pre_velocity.zeros();
    if(!trial_velocity.is_empty()) trial_velocity.zeros();
    if(!incre_velocity.is_empty()) incre_velocity.zeros();
    if(!current_velocity.is_empty()) current_velocity.zeros();
}

void Workroom::clearAcceleration()
{
    if(!pre_acceleration.is_empty()) pre_acceleration.zeros();
    if(!trial_acceleration.is_empty()) trial_acceleration.zeros();
    if(!incre_acceleration.is_empty()) incre_acceleration.zeros();
    if(!current_acceleration.is_empty()) current_acceleration.zeros();
}

void Workroom::clearTemperature()
{
    if(!pre_temperature.is_empty()) pre_temperature.zeros();
    if(!trial_temperature.is_empty()) trial_temperature.zeros();
    if(!incre_temperature.is_empty()) incre_temperature.zeros();
    if(!current_temperature.is_empty()) current_temperature.zeros();
}

void Workroom::resetStatus()
{
    ninja.zeros();

    resetTime();
    resetLoad();
    resetResistance();
    resetDisplacement();
    resetVelocity();
    resetAcceleration();
    resetTemperature();
}

void Workroom::resetTime()
{
    trial_time = current_time;
    incre_time = 0.;
}

void Workroom::resetLoad()
{
    if(!trial_load.is_empty()) {
        trial_load = current_load;
        incre_load.zeros();
    }
}

void Workroom::resetResistance()
{
    if(!trial_resistance.is_empty()) {
        trial_resistance = current_resistance;
        incre_resistance.zeros();
    }
}

void Workroom::resetDisplacement()
{
    if(!trial_displacement.is_empty()) {
        trial_displacement = current_displacement;
        incre_displacement.zeros();
    }
}

void Workroom::resetVelocity()
{
    if(!trial_velocity.is_empty()) {
        trial_velocity = current_velocity;
        incre_velocity.zeros();
    }
}

void Workroom::resetAcceleration()
{
    if(!trial_acceleration.is_empty()) {
        trial_acceleration = current_acceleration;
        incre_acceleration.zeros();
    }
}

void Workroom::resetTemperature()
{
    if(!trial_temperature.is_empty()) {
        trial_temperature = current_temperature;
        incre_temperature.zeros();
    }
}

void Workroom::clearEigen()
{
    eigenvalue.zeros();
    eigenvector.zeros();
}

void Workroom::clearMass() { global_mass.zeros(); }

void Workroom::clearDamping() { global_damping.zeros(); }

void Workroom::clearStiffness() { global_stiffness.zeros(); }

void Workroom::print() { printf("This is a Workroom object.\n"); }

void Workroom::assembleResistance(const mat& ER, const uvec& EI)
{
    for(unsigned I = 0; I < EI.n_elem; ++I) trial_resistance(EI(I)) += ER(I);
}

void Workroom::assembleMass(const mat& EM, const uvec& EI)
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
    else if(!is_symm() && is_band()) {
        auto tmp_shift = low_bandwidth + up_bandwidth;
        for(unsigned I = 0; I < EI.n_elem; ++I)
            for(unsigned J = 0; J < EI.n_elem; ++J)
                global_mass(EI(J) - EI(I) + tmp_shift, EI(I)) += EM(J, I);
    }
}

void Workroom::assembleDamping(const mat& EC, const uvec& EI)
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
    else if(!is_symm() && is_band()) {
        auto tmp_shift = low_bandwidth + up_bandwidth;
        for(unsigned I = 0; I < EI.n_elem; ++I)
            for(unsigned J = 0; J < EI.n_elem; ++J)
                global_damping(EI(J) - EI(I) + tmp_shift, EI(I)) += EC(J, I);
    }
}

void Workroom::assembleStiffness(const mat& EK, const uvec& EI)
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
    else if(!is_symm() && is_band()) {
        auto tmp_shift = low_bandwidth + up_bandwidth;
        for(unsigned I = 0; I < EI.n_elem; ++I)
            for(unsigned J = 0; J < EI.n_elem; ++J)
                global_stiffness(EI(J) - EI(I) + tmp_shift, EI(I)) += EK(J, I);
    }
}

vec& getNinja(const shared_ptr<Workroom>& W) { return W->ninja; }

double& getTrialTime(const shared_ptr<Workroom>& W) { return W->trial_time; }

vec& getTrialLoad(const shared_ptr<Workroom>& W) { return W->trial_load; }

vec& getTrialResistance(const shared_ptr<Workroom>& W) { return W->trial_resistance; }

vec& getTrialDisplacement(const shared_ptr<Workroom>& W) { return W->trial_displacement; }

vec& getTrialVelocity(const shared_ptr<Workroom>& W) { return W->trial_velocity; }

vec& getTrialAcceleration(const shared_ptr<Workroom>& W) { return W->trial_acceleration; }

vec& getTrialTemperature(const shared_ptr<Workroom>& W) { return W->trial_temperature; }

double& getIncreTime(const shared_ptr<Workroom>& W) { return W->incre_time; }

vec& getIncreLoad(const shared_ptr<Workroom>& W) { return W->incre_load; }

vec& getIncreResistance(const shared_ptr<Workroom>& W) { return W->incre_resistance; }

vec& getIncreDisplacement(const shared_ptr<Workroom>& W) { return W->incre_displacement; }

vec& getIncreVelocity(const shared_ptr<Workroom>& W) { return W->incre_velocity; }

vec& getIncreAcceleration(const shared_ptr<Workroom>& W) { return W->incre_acceleration; }

vec& getIncreTemperature(const shared_ptr<Workroom>& W) { return W->incre_temperature; }

double& getCurrentTime(const shared_ptr<Workroom>& W) { return W->current_time; }

vec& getCurrentLoad(const shared_ptr<Workroom>& W) { return W->current_load; }

vec& getCurrentResistance(const shared_ptr<Workroom>& W) { return W->current_resistance; }

vec& getCurrentDisplacement(const shared_ptr<Workroom>& W)
{
    return W->current_displacement;
}

vec& getCurrentVelocity(const shared_ptr<Workroom>& W) { return W->current_velocity; }

vec& getCurrentAcceleration(const shared_ptr<Workroom>& W)
{
    return W->current_acceleration;
}

vec& getCurrentTemperature(const shared_ptr<Workroom>& W)
{
    return W->current_temperature;
}

vec& getPreviousLoad(const shared_ptr<Workroom>& W) { return W->pre_load; }

vec& getPreviousResistance(const shared_ptr<Workroom>& W) { return W->pre_resistance; }

vec& getPreviousDisplacement(const shared_ptr<Workroom>& W)
{
    return W->pre_displacement;
}

vec& getPreviousVelocity(const shared_ptr<Workroom>& W) { return W->pre_velocity; }

vec& getPreviousAcceleration(const shared_ptr<Workroom>& W)
{
    return W->pre_acceleration;
}

vec& getPreviousTemperature(const shared_ptr<Workroom>& W) { return W->pre_temperature; }

T2& getMass(const shared_ptr<Workroom>& W) { return W->global_mass; }

T2& getDamping(const shared_ptr<Workroom>& W) { return W->global_damping; }

T2& getStiffness(const shared_ptr<Workroom>& W) { return W->global_stiffness; }

vec& getEigenvalue(const shared_ptr<Workroom>& W) { return W->eigenvalue; }

mat& getEigenvector(const shared_ptr<Workroom>& W) { return W->eigenvector; }
