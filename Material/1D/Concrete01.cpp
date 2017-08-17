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

unique_ptr<Material> Concrete01::getCopy() { return make_unique<Concrete01>(*this); }

int Concrete01::updateTrialStatus(const vec& t_strain)
{
    trial_strain = t_strain;
    incre_strain = trial_strain - current_strain;
    if(incre_strain(0) == 0.) return 0;

    return 0;
}

int Concrete01::clearStatus() { return 0; }

int Concrete01::commitStatus() { return 0; }

int Concrete01::resetStatus() { return 0; }
