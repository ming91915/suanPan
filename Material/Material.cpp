#include "Material.h"

Material::Material(const unsigned& T, const unsigned& CT)
    : Tag(T, CT)
{
}

Material::~Material() { suanpan_debug("Material %u dtor() Called.\n", getTag()); }

void Material::initialize() {}

double Material::getParameter(const unsigned&) const { return density; }

const vec& Material::getStrain() const { return trial_strain; }

const vec& Material::getStrainRate() const { return trial_strain_rate; }

const vec& Material::getStress() const { return trial_stress; }

const mat& Material::getStiffness() const { return trial_stiffness; }

const mat& Material::getInitialStiffness() const { return initial_stiffness; }

unique_ptr<Material> Material::getCopy() { return nullptr; }

int Material::updateIncreStatus(const vec&) { return -1; }

int Material::updateIncreStatus(const vec&, const vec&) { return -1; }

int Material::updateTrialStatus(const vec&) { return -1; }

int Material::updateTrialStatus(const vec&, const vec&) { return -1; }

int Material::clearStatus()
{
    if(!current_strain.is_empty()) current_strain.zeros();
    if(!current_strain_rate.is_empty()) current_strain_rate.zeros();
    if(!current_stress.is_empty()) current_stress.zeros();
    if(!current_history.is_empty()) current_history.zeros();

    if(!trial_strain.is_empty()) trial_strain.zeros();
    if(!trial_strain_rate.is_empty()) trial_strain_rate.zeros();
    if(!trial_stress.is_empty()) trial_stress.zeros();
    if(!trial_history.is_empty()) trial_history.zeros();

    if(!incre_strain.is_empty()) incre_strain.zeros();
    if(!incre_stress.is_empty()) incre_stress.zeros();
    if(!incre_strain_rate.is_empty()) incre_strain_rate.zeros();

    current_stiffness = initial_stiffness;
    trial_stiffness = initial_stiffness;

    return 0;
}

int Material::commitStatus()
{
    current_stiffness = trial_stiffness;

    if(!trial_strain.is_empty()) current_strain = trial_strain;
    if(!trial_strain_rate.is_empty()) current_strain_rate = trial_strain_rate;
    if(!trial_stress.is_empty()) current_stress = trial_stress;
    if(!trial_history.is_empty()) current_history = trial_history;

    return 0;
}

int Material::resetStatus()
{
    trial_stiffness = current_stiffness;

    if(!trial_strain.is_empty()) {
        trial_strain = current_strain;
        incre_strain.zeros();
    }

    if(!trial_strain_rate.is_empty()) {
        trial_strain_rate = current_strain_rate;
        incre_strain_rate.zeros();
    }

    if(!trial_stress.is_empty()) {
        trial_stress = current_stress;
        incre_stress.zeros();
    }

    if(!trial_history.is_empty()) trial_history = current_history;

    return 0;
}
