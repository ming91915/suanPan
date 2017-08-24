#include "MaterialExample.h"
#include <Toolbox/utility.h>

using std::vector;

SUANPAN_EXPORT void new_materialexample_(unique_ptr<Material>& return_obj,
    std::istringstream& command)
{
    unsigned tag;
    if((command >> tag).fail()) {
        suanpan_error("new_materialexample_() needs a tag.\n");
        return;
    }

    double elastic_modulus;
    if((command >> elastic_modulus).fail()) {
        suanpan_error("new_materialexample_() needs elastic modulus.\n");
        return;
    }

    double yield_stress;
    if((command >> yield_stress).fail()) {
        suanpan_error("new_materialexample_() needs yield stress.\n");
        return;
    }

    double hardening_ratio;
    if((command >> hardening_ratio).fail()) {
        suanpan_error("new_materialexample_() needs hardening ratio.\n");
        return;
    }

    double beta = 0.;
    if(command.eof())
        suanpan_info("new_materialexample_() assumes zero beta.\n");
    else if((command >> beta).fail()) {
        suanpan_error("ElementExample needs a valid beta.\n");
        return;
    }

    double density = 0.;
    if(command.eof())
        suanpan_info("new_materialexample_() assumes zero density.\n");
    else if((command >> density).fail()) {
        suanpan_error("ElementExample needs a valid density.\n");
        return;
    }

    return_obj = make_unique<MaterialExample>(
        tag, elastic_modulus, yield_stress, hardening_ratio, beta, density);
}

#ifndef MT_MATERIALEXAMPLE
#define MT_MATERIALEXAMPLE 0
#endif

MaterialExample::MaterialExample(const unsigned& T,
    const double& E,
    const double& Y,
    const double& H,
    const double& B,
    const double& R)
    : Material(T, MT_MATERIALEXAMPLE)
    , elastic_modulus(E)
    , yield_stress(Y)
    , hardening_ratio(H)
    , beta(B)
    , plastic_modulus(elastic_modulus * hardening_ratio / (1. - hardening_ratio))
    , tolerance(1E-10 * yield_stress)
{
    density = R;
    MaterialExample::initialize();
}

void MaterialExample::initialize()
{
    current_strain.zeros(1);
    current_stress.zeros(1);

    trial_strain.zeros(1);
    trial_stress.zeros(1);

    current_back_stress = 0.;
    current_plastic_strain = 0.;

    trial_back_stress = 0.;
    trial_plastic_strain = 0.;

    initial_stiffness = { elastic_modulus };
    current_stiffness = initial_stiffness;
    trial_stiffness = initial_stiffness;
}

unique_ptr<Material> MaterialExample::get_copy()
{
    return make_unique<MaterialExample>(*this);
}

int MaterialExample::update_trial_status(const vec& t_strain)
{
    trial_strain = t_strain;
    incre_strain = trial_strain - current_strain;

    if(incre_strain(0) == 0.) return 0;

    trial_back_stress = current_back_stress;
    trial_plastic_strain = current_plastic_strain;
    trial_stiffness = initial_stiffness;

    trial_stress = current_stress + elastic_modulus * incre_strain;

    auto shifted_stress = trial_stress(0) - current_back_stress;

    auto yield_func = abs(shifted_stress) - yield_stress -
        (1. - beta) * plastic_modulus * current_plastic_strain;

    if(yield_func > tolerance) {
        auto incre_plastic_strain = yield_func / (elastic_modulus + plastic_modulus);
        trial_stress -= sign(shifted_stress) * elastic_modulus * incre_plastic_strain;
        trial_stiffness *= hardening_ratio;
        trial_back_stress +=
            sign(shifted_stress) * beta * plastic_modulus * incre_plastic_strain;
        trial_plastic_strain += incre_plastic_strain;
    }

    return 0;
}

int MaterialExample::clear_status()
{
    initialize();
    return 0;
}

int MaterialExample::commit_status()
{
    current_strain = trial_strain;
    current_stress = trial_stress;
    current_stiffness = trial_stiffness;
    current_back_stress = trial_back_stress;
    current_plastic_strain = trial_plastic_strain;
    return 0;
}

int MaterialExample::reset_status()
{
    trial_strain = current_strain;
    trial_stress = current_stress;
    trial_stiffness = current_stiffness;
    trial_back_stress = current_back_stress;
    trial_plastic_strain = current_plastic_strain;
    return 0;
}
