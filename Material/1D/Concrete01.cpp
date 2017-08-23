#include "Concrete01.h"

void Concrete01::initialize()
{
    current_strain.zeros(1);
    current_stress.zeros(1);

    trial_strain.zeros(1);
    trial_stress.zeros(1);

    current_stiffness = initial_stiffness;
    trial_stiffness = initial_stiffness;
}

unique_ptr<Material> Concrete01::get_copy() { return make_unique<Concrete01>(*this); }

int Concrete01::update_trial_status(const vec& t_strain)
{
    trial_strain = t_strain;
    incre_strain = trial_strain - current_strain;
    if(incre_strain(0) == 0.) return 0;

    return 0;
}

int Concrete01::clear_status() { return 0; }

int Concrete01::commit_status() { return 0; }

int Concrete01::reset_status() { return 0; }
