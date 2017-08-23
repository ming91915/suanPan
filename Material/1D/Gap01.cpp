#include "Gap01.h"
#include <Toolbox/utility.h>

Gap01::Gap01(const unsigned& T,
    const double& E,
    const double& Y,
    const double& G,
    const double& R)
    : Material(T, MT_GAP01)
    , elastic_modulus(E)
    , yield_stress(Y)
    , gap_strain(G)
{
    density = R;
    Gap01::initialize();
}

void Gap01::initialize()
{
    current_strain.zeros(1);
    trial_strain.zeros(1);

    current_stress.zeros(1);
    trial_stress.zeros(1);

    initial_stiffness = { elastic_modulus };
    current_stiffness = initial_stiffness;
    trial_stiffness = initial_stiffness;

    current_load_flag = 0.;
    trial_load_flag = 0.;
    current_reverse_strain = 0.;
    trial_reverse_strain = 0.;
}

unique_ptr<Material> Gap01::get_copy() { return make_unique<Gap01>(*this); }

int Gap01::update_incre_status(const vec& i_strain)
{
    return update_trial_status(current_strain + i_strain);
}

int Gap01::update_trial_status(const vec& t_strain)
{
    trial_strain = t_strain;
    incre_strain = trial_strain - current_strain;

    if(incre_strain(0) == 0.) return 0;

    trial_reverse_strain = current_reverse_strain;

    trial_load_flag = sign(incre_strain(0));

    if(current_load_flag > 0. && current_load_flag == trial_load_flag &&
        current_stress(0) == 0) {
        incre_strain = trial_strain - gap_strain - trial_reverse_strain;
        if(incre_strain(0) < 0) incre_strain(0) = 0.;
    }

    if(current_load_flag < 0. && current_load_flag != trial_load_flag) {
        trial_reverse_strain = current_strain(0);
        if(current_stress(0) == 0) {
            incre_strain = trial_strain - gap_strain - trial_reverse_strain;
            if(incre_strain(0) < 0) incre_strain(0) = 0.;
        }
    }

    trial_stress = current_stress + elastic_modulus * incre_strain;
    trial_stiffness = initial_stiffness;
    if(trial_stress(0) < 0.) {
        trial_stress(0) = 0.;
        trial_stiffness(0) = 0.;
    } else if(trial_stress(0) > yield_stress) {
        trial_stress(0) = yield_stress;
        trial_stiffness(0) = 0.;
    }

    return 0;
}

int Gap01::clear_status()
{
    initialize();
    return 0;
}

int Gap01::commit_status()
{
    current_strain = trial_strain;
    current_stress = trial_stress;
    current_stiffness = trial_stiffness;
    current_load_flag = trial_load_flag;
    current_reverse_strain = trial_reverse_strain;
    return 0;
}

int Gap01::reset_status()
{
    trial_strain = current_strain;
    trial_stress = current_stress;
    trial_stiffness = current_stiffness;
    trial_load_flag = current_load_flag;
    trial_reverse_strain = current_reverse_strain;
    return 0;
}
