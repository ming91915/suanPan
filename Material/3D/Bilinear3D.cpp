#include "Bilinear3D.h"
#include <toolbox/tensorToolbox.h>

const vec Bilinear3D::norm_weight = { 1, 1, 1, 2, 2, 2 };
const double Bilinear3D::root_two_third = sqrt(2. / 3.);

Bilinear3D::Bilinear3D(const unsigned& T,
    const double& E,
    const double& V,
    const double& Y,
    const double& H,
    const double& B,
    const double& R)
    : Material(T, MT_BILINEAR3D)
    , elastic_modulus(E)
    , poissons_ratio(V)
    , yield_stress(Y)
    , hardening_ratio(H)
    , beta(B)
    , tolerance(1E-10 * yield_stress)
    , shear_modulus(elastic_modulus / (2. + 2. * poissons_ratio))
    , double_shear(2. * shear_modulus)
    , square_double_shear(double_shear * double_shear)
    , plastic_modulus(elastic_modulus * hardening_ratio / (1. - hardening_ratio))
    , factor(2. / 3. * plastic_modulus)
{
    density = R;
    Bilinear3D::initialize();
}

void Bilinear3D::initialize()
{
    current_strain.zeros(6);
    current_stress.zeros(6);
    trial_strain.zeros(6);
    trial_stress.zeros(6);

    current_back_stress.zeros(6);
    trial_back_stress.zeros(6);

    current_plastic_strain = 0.;
    trial_plastic_strain = 0.;

    auto lambda = shear_modulus * poissons_ratio / (.5 - poissons_ratio);

    initial_stiffness.zeros(6, 6);

    for(auto I = 0; I < 3; ++I)
        for(auto J = 0; J < 3; ++J) initial_stiffness(I, J) = lambda;

    for(auto I = 0; I < 3; ++I) initial_stiffness(I, I) += double_shear;

    for(auto I = 3; I < 6; ++I) initial_stiffness(I, I) = shear_modulus;

    current_stiffness = initial_stiffness;
    trial_stiffness = initial_stiffness;
}

unique_ptr<Material> Bilinear3D::getCopy() { return make_unique<Bilinear3D>(*this); }

int Bilinear3D::updateTrialStatus(const vec& t_strain)
{
    trial_strain = t_strain;
    incre_strain = trial_strain - current_strain;
    trial_stress = current_stress + initial_stiffness * incre_strain;
    trial_back_stress = current_back_stress;
    trial_plastic_strain = current_plastic_strain;
    trial_stiffness = initial_stiffness;

    vec shifted_stress = dev(trial_stress) - current_back_stress;

    auto norm_shifted_stress = sqrt(accu(norm_weight % shifted_stress % shifted_stress));

    auto yield_func = norm_shifted_stress -
        root_two_third *
            (yield_stress + (1. - beta) * plastic_modulus * current_plastic_strain);

    if(yield_func > tolerance) {
        auto tmp_a = double_shear + factor;
        auto gamma = yield_func / tmp_a;
        vec unit_norm = shifted_stress / norm_shifted_stress;
        vec tmp_b = gamma * unit_norm;
        auto tmp_c = square_double_shear / tmp_a;
        auto tmp_d = square_double_shear * gamma / norm_shifted_stress;

        trial_stress -= double_shear * tmp_b;
        trial_back_stress += factor * beta * tmp_b;
        trial_plastic_strain += root_two_third * gamma;

        trial_stiffness -= (tmp_c - tmp_d) * unit_norm * unit_norm.t();
        trial_stiffness -= tmp_d * unitDevTensor4();
    }
    return 0;
}

int Bilinear3D::clearStatus()
{
    current_strain.zeros(6);
    trial_strain.zeros(6);

    current_stress.zeros(6);
    trial_stress.zeros(6);

    current_back_stress.zeros(6);
    trial_back_stress.zeros(6);

    current_plastic_strain = 0.;
    trial_plastic_strain = 0.;

    current_stiffness = initial_stiffness;
    trial_stiffness = initial_stiffness;

    return 0;
}

int Bilinear3D::commitStatus()
{
    current_strain = trial_strain;
    current_stress = trial_stress;
    current_stiffness = trial_stiffness;
    current_back_stress = trial_back_stress;
    current_plastic_strain = trial_plastic_strain;

    return 0;
}

int Bilinear3D::resetStatus()
{
    trial_strain = current_strain;
    trial_stress = current_stress;
    trial_stiffness = current_stiffness;
    trial_back_stress = current_back_stress;
    trial_plastic_strain = current_plastic_strain;

    return 0;
}
