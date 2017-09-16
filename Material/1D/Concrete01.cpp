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
﻿#include "Concrete01.h"

void Concrete01::initialize() {
    current_strain.zeros(1);
    current_stress.zeros(1);

    trial_strain.zeros(1);
    trial_stress.zeros(1);

    current_stiffness = initial_stiffness;
    trial_stiffness = initial_stiffness;
}

unique_ptr<Material> Concrete01::get_copy() { return make_unique<Concrete01>(*this); }

int Concrete01::update_trial_status(const vec& t_strain) {
    trial_strain = t_strain;
    incre_strain = trial_strain - current_strain;
    if(incre_strain(0) == 0.) return 0;

    return 0;
}

int Concrete01::clear_status() { return 0; }

int Concrete01::commit_status() { return 0; }

int Concrete01::reset_status() { return 0; }
