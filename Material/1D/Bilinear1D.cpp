#include "Bilinear1D.h"
#include <Toolbox/utility.h>

Bilinear1D::Bilinear1D(const unsigned& T, const double& E, const double& Y, const double& H, const double& B, const double& R)
    : Material(T, MT_BILINEAR1D)
    , elastic_modulus(E)
    , yield_stress(Y)
    , hardening_ratio(H)
    , beta(B)
    , plastic_modulus(elastic_modulus * hardening_ratio / (1. - hardening_ratio))
    , tolerance(1E-10 * yield_stress) {
    density = R;
    Bilinear1D::initialize();
}

Bilinear1D::Bilinear1D(const double& E, const double& Y, const double& H, const double& B, const double& R)
    : Material(0, MT_BILINEAR1D)
    , elastic_modulus(E)
    , yield_stress(Y)
    , hardening_ratio(H)
    , beta(B)
    , plastic_modulus(elastic_modulus * hardening_ratio / (1. - hardening_ratio))
    , tolerance(1E-10 * yield_stress) {
    density = R;
    Bilinear1D::initialize();
}

void Bilinear1D::initialize() {
    current_strain.zeros(1);
    current_stress.zeros(1);

    trial_strain.zeros(1);
    trial_stress.zeros(1);

    current_back_stress = 0.;
    current_plastic_strain = 0.;

    trial_back_stress = 0.;
    trial_plastic_strain = 0.;

    initial_stiffness = elastic_modulus;
    current_stiffness = initial_stiffness;
    trial_stiffness = initial_stiffness;
}

unique_ptr<Material> Bilinear1D::get_copy() { return make_unique<Bilinear1D>(*this); }

int Bilinear1D::update_trial_status(const vec& t_strain) {
    trial_strain = t_strain;
    incre_strain = trial_strain - current_strain;

    if(incre_strain(0) == 0.) return 0;

    trial_back_stress = current_back_stress;
    trial_plastic_strain = current_plastic_strain;
    trial_stiffness = initial_stiffness;

    trial_stress = current_stress + elastic_modulus * incre_strain;

    const auto shifted_stress = trial_stress(0) - current_back_stress;

    const auto yield_func = abs(shifted_stress) - yield_stress - (1. - beta) * plastic_modulus * current_plastic_strain;

    if(yield_func > tolerance) {
        const auto incre_plastic_strain = yield_func / (elastic_modulus + plastic_modulus);
        trial_stress -= sign(shifted_stress) * elastic_modulus * incre_plastic_strain;
        trial_stiffness *= hardening_ratio;
        trial_back_stress += sign(shifted_stress) * beta * plastic_modulus * incre_plastic_strain;
        trial_plastic_strain += incre_plastic_strain;
    }

    return 0;
}

int Bilinear1D::clear_status() {
    initialize();
    return 0;
}

int Bilinear1D::commit_status() {
    current_strain = trial_strain;
    current_stress = trial_stress;
    current_stiffness = trial_stiffness;
    current_back_stress = trial_back_stress;
    current_plastic_strain = trial_plastic_strain;
    return 0;
}

int Bilinear1D::reset_status() {
    trial_strain = current_strain;
    trial_stress = current_stress;
    trial_stiffness = current_stiffness;
    trial_back_stress = current_back_stress;
    trial_plastic_strain = current_plastic_strain;
    return 0;
}
