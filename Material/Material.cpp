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

Material::Material(const unsigned& T, const unsigned& CT)
    : Tag(T, CT) {
    suanpan_debug("Material %u ctor() called.\n", T);
}

Material::~Material() { suanpan_debug("Material %u dtor() called.\n", get_tag()); }

void Material::initialize() {}

double Material::get_parameter(const unsigned&) const { return density; }

const vec& Material::get_strain() const { return trial_strain; }

const vec& Material::get_strain_rate() const { return trial_strain_rate; }

const vec& Material::get_stress() const { return trial_stress; }

const mat& Material::get_stiffness() const { return trial_stiffness; }

const mat& Material::get_initial_stiffness() const { return initial_stiffness; }

unique_ptr<Material> Material::get_copy() { return nullptr; }

int Material::update_incre_status(const vec&) { return -1; }

int Material::update_incre_status(const vec&, const vec&) { return -1; }

int Material::update_trial_status(const vec&) { return -1; }

int Material::update_trial_status(const vec&, const vec&) { return -1; }

int Material::clear_status() {
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
