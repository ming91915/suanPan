////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2017 Theodore Chang
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////

#include "Material.h"

Material::Material(const unsigned& T, const unsigned& CT, const MaterialType& MT, const double& D)
    : Tag(T, CT)
    , density(D)
    , material_type(MT) {
    suanpan_debug("Material %u ctor() called.\n", T);
}

Material::~Material() { suanpan_debug("Material %u dtor() called.\n", get_tag()); }

void Material::initialize(const shared_ptr<DomainBase>&) {
    unsigned size = 0;
    switch(material_type) {
    case MaterialType::D0:
        break;
    case MaterialType::D1:
        size = 1;
        break;
    case MaterialType::D2:
        size = 3;
        break;
    case MaterialType::D3:
        size = 6;
        break;
    }

    if(size == 0) return;

    current_strain.zeros(size);
    current_stress.zeros(size);

    trial_strain.zeros(size);
    trial_stress.zeros(size);
}

double Material::get_parameter(const ParameterType& T) const {
    if(T == ParameterType::DENSITY) return density;

    return 0.;
}

const vec& Material::get_strain() const { return trial_strain; }

const vec& Material::get_strain_rate() const { return trial_strain_rate; }

const vec& Material::get_stress() const { return trial_stress; }

const mat& Material::get_stiffness() const { return trial_stiffness; }

const mat& Material::get_initial_stiffness() const { return initial_stiffness; }

unique_ptr<Material> Material::get_copy() { throw invalid_argument("hidden method called.\n"); }

int Material::update_incre_status(const double i_strain) {
    const vec i_vec_strain{ i_strain };
    return update_incre_status(i_vec_strain);
}

int Material::update_incre_status(const double i_strain, const double i_strain_rate) {
    const vec i_vec_strain{ i_strain };
    const vec i_vec_strain_rate{ i_strain_rate };
    return update_incre_status(i_vec_strain, i_vec_strain_rate);
}

int Material::update_trial_status(const double t_strain) {
    const vec t_vec_strain{ t_strain };
    return update_trial_status(t_vec_strain);
}

int Material::update_trial_status(const double t_strain, const double t_strain_rate) {
    const vec t_vec_strain{ t_strain };
    const vec t_vec_strain_rate{ t_strain_rate };
    return update_trial_status(t_vec_strain, t_vec_strain_rate);
}

int Material::update_incre_status(const vec& i_strain) { return update_trial_status(current_strain + i_strain); }

int Material::update_incre_status(const vec& i_strain, const vec& i_strain_rate) { return update_trial_status(current_strain + i_strain, current_strain_rate + i_strain_rate); }

int Material::update_trial_status(const vec&) { throw invalid_argument("hidden method called.\n"); }

int Material::update_trial_status(const vec& t_strain, const vec&) { return update_trial_status(t_strain); }

int Material::clear_status() {
    if(!current_strain.is_empty()) current_strain.zeros();
    if(!current_strain_rate.is_empty()) current_strain_rate.zeros();
    if(!current_stress.is_empty()) current_stress.zeros();
    if(!current_history.is_empty()) current_history.zeros();

    if(!trial_strain.is_empty()) trial_strain.zeros();
    if(!trial_strain_rate.is_empty()) trial_strain_rate.zeros();
    if(!trial_stress.is_empty()) trial_stress.zeros();
    if(!trial_history.is_empty()) trial_history.zeros();

    current_stiffness = initial_stiffness;
    trial_stiffness = initial_stiffness;

    return 0;
}

int Material::commit_status() {
    current_stiffness = trial_stiffness;

    if(!trial_strain.is_empty()) current_strain = trial_strain;
    if(!trial_strain_rate.is_empty()) current_strain_rate = trial_strain_rate;
    if(!trial_stress.is_empty()) current_stress = trial_stress;
    if(!trial_history.is_empty()) current_history = trial_history;

    return 0;
}

int Material::reset_status() {
    trial_stiffness = current_stiffness;

    if(!trial_strain.is_empty()) trial_strain = current_strain;
    if(!trial_strain_rate.is_empty()) trial_strain_rate = current_strain_rate;
    if(!trial_stress.is_empty()) trial_stress = current_stress;
    if(!trial_history.is_empty()) trial_history = current_history;

    return 0;
}

vector<vec> Material::record(const OutputType&) { return {}; }

unique_ptr<Material> suanpan::make_copy(const shared_ptr<Material>& P) { return P->get_copy(); }

unique_ptr<Material> suanpan::make_copy(const unique_ptr<Material>& P) { return P->get_copy(); }
