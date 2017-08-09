#include "Elastic3D.h"

Elastic3D::Elastic3D(const unsigned& T, const double& E, const double& P, const double& R)
    : Material(T, MT_ELASTIC3D)
    , elastic_modulus(E)
    , poissons_ratio(P)
{
    density = R;
    Elastic3D::initialize();
}

Elastic3D::Elastic3D(const double& E, const double& P, const double& R)
    : Material(0, MT_ELASTIC3D)
    , elastic_modulus(E)
    , poissons_ratio(P)
{
    density = R;
    Elastic3D::initialize();
}

Elastic3D::~Elastic3D() {}

void Elastic3D::initialize()
{
    current_strain.zeros(6);
    current_stress.zeros(6);
    trial_strain.zeros(6);
    trial_stress.zeros(6);
    // incre_strain.zeros(6);
    // incre_stress.zeros(6);

    auto shear_modulus = elastic_modulus / (2. + 2. * poissons_ratio);
    auto lambda = shear_modulus * poissons_ratio / (.5 - poissons_ratio);

    initial_stiffness.zeros(6, 6);

    for(auto I = 0; I < 3; ++I)
        for(auto J = 0; J < 3; ++J) initial_stiffness(I, J) = lambda;

    for(auto I = 0; I < 3; ++I) initial_stiffness(I, I) += 2. * shear_modulus;

    for(auto I = 3; I < 6; ++I) initial_stiffness(I, I) = shear_modulus;

    current_stiffness = initial_stiffness;
    trial_stiffness = initial_stiffness;
}

unique_ptr<Material> Elastic3D::getCopy() { return make_unique<Elastic3D>(*this); }

int Elastic3D::updateTrialStatus(const vec& t_strain)
{
    trial_strain = t_strain;
    trial_stress = trial_stiffness * trial_strain;
    // incre_strain = trial_strain - current_strain;
    // incre_stress = trial_stress - current_stress;
    return 0;
}

int Elastic3D::clearStatus()
{
    current_strain.zeros(6);
    current_stress.zeros(6);
    trial_strain.zeros(6);
    trial_stress.zeros(6);
    current_stiffness = initial_stiffness;
    trial_stiffness = initial_stiffness;
    return 0;
}

int Elastic3D::commitStatus()
{
    current_strain = trial_strain;
    current_stress = trial_stress;
    // current_stiffness = trial_stiffness;
    return 0;
}

int Elastic3D::resetStatus()
{
    trial_strain = current_strain;
    trial_stress = current_stress;
    // trial_stiffness = current_stiffness;
    return 0;
}

void Elastic3D::print() { printf("Isotropic Elastic Material.\n"); }
