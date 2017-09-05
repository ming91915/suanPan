#include "MPF.h"
#include <Toolbox/utility.h>

MPF::MPF(const unsigned& T, const double& E, const double& Y, const double& H, const double& R, const double& AA1, const double& AA2, const double& AA3, const double& AA4, const bool& ISO, const bool& CON, const double& D)
    : Material(T, MT_MPF)
    , elastic_modulus(E)
    , yield_stress(Y)
    , hardening_ratio(H)
    , R0(R)
    , A1(AA1)
    , A2(AA2)
    , A3(AA3)
    , A4(AA4)
    , isotropic_hardening(ISO)
    , constant_radius(CON)
    , yield_strain(Y / E) {
    density = D;
    MPF::initialize();
}

void MPF::initialize() {
    current_strain.zeros(1);
    trial_strain.zeros(1);

    current_stress.zeros(1);
    trial_stress.zeros(1);

    initial_stiffness = elastic_modulus;
    current_stiffness = initial_stiffness;
    trial_stiffness = initial_stiffness;

    current_history.zeros(7);
    trial_history.zeros(7);

    current_history(2) = yield_stress;
    current_history(3) = yield_strain;
}

unique_ptr<Material> MPF::get_copy() { return make_unique<MPF>(*this); }

int MPF::update_incre_status(const vec& i_strain) { return update_trial_status(current_strain + i_strain); }

int MPF::update_trial_status(const vec& t_strain) {
    trial_strain = t_strain;
    incre_strain = trial_strain - current_strain;

    if(incre_strain(0) == 0.) return 0;

    auto reverse_stress = current_history(0);
    auto reverse_strain = current_history(1);
    auto inter_stress = current_history(2);
    auto inter_strain = current_history(3);
    auto pre_inter_strain = current_history(4);
    auto max_strain = current_history(5);
    auto load_sign = current_history(6);

    auto shift_stress = 0.;
    if(isotropic_hardening) {
        if(abs(trial_strain(0)) > max_strain) max_strain = abs(trial_strain(0));
        shift_stress = A3 * yield_stress * (max_strain / yield_strain - A4);
        if(shift_stress < 0.) shift_stress = 0.;
    }

    const auto trial_load_sign = sign(incre_strain(0));

    if(load_sign == 0.)
        load_sign = trial_load_sign;
    else if(trial_load_sign != load_sign) {
        reverse_stress = current_stress(0);
        reverse_strain = current_strain(0);
        pre_inter_strain = inter_strain;
        inter_strain = yield_strain * hardening_ratio * elastic_modulus - yield_stress - shift_stress;
        if(trial_load_sign > 0.) inter_strain *= -1.;
        inter_strain = (inter_strain + elastic_modulus * reverse_strain - reverse_stress) / (1. - hardening_ratio) / elastic_modulus;
        inter_stress = elastic_modulus * (inter_strain - reverse_strain) + reverse_stress;
        load_sign = trial_load_sign;
    }

    auto R = R0;

    if(!constant_radius && reverse_strain != 0.) {
        const auto XI = abs(reverse_strain - pre_inter_strain) / yield_strain;
        R -= A1 * XI / (A2 + XI);
    }

    const auto normal_strain = (trial_strain(0) - reverse_strain) / (inter_strain - reverse_strain);
    const auto tmp_a = 1. + pow(normal_strain, R);
    const auto tmp_b = (1. - hardening_ratio) / pow(tmp_a, 1. / R);

    const auto normal_stress = (hardening_ratio + tmp_b) * normal_strain;

    trial_stress(0) = normal_stress * (inter_stress - reverse_stress) + reverse_stress;

    trial_stiffness(0) = (inter_stress - reverse_stress) / (inter_strain - reverse_strain) * (hardening_ratio + tmp_b / tmp_a);

    trial_history(0) = reverse_stress;
    trial_history(1) = reverse_strain;
    trial_history(2) = inter_stress;
    trial_history(3) = inter_strain;
    trial_history(4) = pre_inter_strain;
    trial_history(5) = max_strain;
    trial_history(6) = load_sign;

    return 0;
}

int MPF::clear_status() {
    initialize();
    return 0;
}

int MPF::commit_status() {
    current_strain = trial_strain;
    current_stress = trial_stress;
    current_stiffness = trial_stiffness;
    current_history = trial_history;
    return 0;
}

int MPF::reset_status() {
    trial_strain = current_strain;
    trial_stress = current_stress;
    trial_stiffness = current_stiffness;
    trial_history = current_history;
    return 0;
}
