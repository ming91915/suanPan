#include "RambergOsgood.h"
#include <Toolbox/tensorToolbox.h>

RambergOsgood::RambergOsgood(const unsigned& T,
    const double& E,
    const double& Y,
    const double& O,
    const double& N,
    const double& R)
    : Material(T, MT_RAMBERGOSGOOD)
    , elastic_modulus(E)
    , yield_stress(Y)
    , offset(O)
    , n(N)
    , nm(N - 1.)
    , tolerance(1E-10 * yield_stress)
{
    density = R;
    RambergOsgood::initialize();
}

void RambergOsgood::initialize()
{
    current_strain.zeros(1);
    trial_strain.zeros(1);

    current_stress.zeros(1);
    trial_stress.zeros(1);

    current_history.zeros(3);
    trial_history.zeros(3);

    initial_stiffness = { elastic_modulus };
    current_stiffness = initial_stiffness;
    trial_stiffness = initial_stiffness;
}

unique_ptr<Material> RambergOsgood::getCopy()
{
    return make_unique<RambergOsgood>(*this);
}

int RambergOsgood::updateIncreStatus(const vec& i_strain)
{
    return updateTrialStatus(current_strain + i_strain);
}

int RambergOsgood::updateTrialStatus(const vec& t_strain)
{
    trial_strain = t_strain;
    incre_strain = trial_strain - current_strain;

    auto reverse_strain = current_history(0);
    auto reverse_stress = current_history(1);
    auto load_sign = current_history(2);

    auto trial_load_sign = sign(incre_strain(0));
    if(trial_load_sign != load_sign) {
        reverse_strain = current_strain(0);
        reverse_stress = current_stress(0);
        load_sign = trial_load_sign;
    }

    auto norm_stress = abs(current_stress(0) - reverse_stress);

    auto elastic_predictor = elastic_modulus * abs(trial_strain(0) - reverse_strain);

    auto incre_norm_stress = yield_stress;
    while(abs(incre_norm_stress) > tolerance) {
        auto tmp_a = offset * pow(norm_stress / yield_stress, nm);
        trial_stiffness(0) = 1. + tmp_a * n;
        incre_norm_stress =
            (elastic_predictor - norm_stress * (1. + tmp_a)) / trial_stiffness(0);
        norm_stress += incre_norm_stress;
    }

    trial_stress = load_sign * norm_stress + reverse_stress;

    trial_history(0) = reverse_strain;
    trial_history(1) = reverse_stress;
    trial_history(2) = load_sign;

    return 0;
}

int RambergOsgood::clearStatus()
{
    initialize();
    return 0;
}

int RambergOsgood::commitStatus()
{
    current_strain = trial_strain;
    current_stress = trial_stress;
    current_stiffness = trial_stiffness;
    current_history = trial_history;
    return 0;
}

int RambergOsgood::resetStatus()
{
    trial_strain = current_strain;
    trial_stress = current_stress;
    trial_stiffness = current_stiffness;
    trial_history = current_history;
    return 0;
}
