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

#include "Viscosity1D.h"
#include "Toolbox/utility.h"

Viscosity1D::Viscosity1D(const unsigned T, const double C, const double A)
    : Material1D(T, MT_VISCOSITY1D, 0.)
    , damping(C)
    , alpha(A == 0. ? A : 1. / A) {}

void Viscosity1D::initialize(const shared_ptr<DomainBase>&) {}

unique_ptr<Material> Viscosity1D::get_copy() { return make_unique<Viscosity1D>(*this); }

int Viscosity1D::update_trial_status(const vec&, const vec& t_strain_rate) {
    trial_strain_rate = t_strain_rate;

    trial_stress = -suanpan::sign(trial_strain_rate(0)) * damping * pow(fabs(trial_strain_rate(0)), alpha);

    return 0;
}

int Viscosity1D::clear_status() {
    current_strain_rate.zeros();
    current_stress.zeros();
    trial_strain_rate.zeros();
    trial_stress.zeros();
    return 0;
}

int Viscosity1D::commit_status() {
    current_strain_rate = trial_strain_rate;
    current_stress = trial_stress;
    return 0;
}

int Viscosity1D::reset_status() {
    trial_strain_rate = current_strain_rate;
    trial_stress = current_stress;
    return 0;
}

void Viscosity1D::print() { suanpan_info("1-D Vicosity Material %u.\n", get_tag()); }
