/*******************************************************************************
 * Copyright (C) 2017 Theodore Chang
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

#ifndef TENSORTOOLBOX_H
#define TENSORTOOLBOX_H

#include <suanPan.h>

namespace tensor {
mat unitDevTensor4();

double tr(const vec&);
double mean(const vec&);
vec dev(const vec&);
}

namespace transform {
double atan2(const vec&);
mat form_trans(const double);
vec nominal_to_principal_strain(const vec&);
vec nominal_to_principal_stress(const vec&);
mat nominal_to_principal_strain(vec&, double&);
mat nominal_to_principal_stress(vec&, double&);
vec rotate_strain(const vec&, const mat&);
vec rotate_stress(const vec&, const mat&);
vec rotate_strain(const vec&, const double);
vec rotate_stress(const vec&, const double);
namespace beam {
    mat global_to_local(const double, const double, const double, const double);
    mat global_to_local(const double, const double, const double);
    mat global_to_local(const vec& direction_cosine, const double length);
}
}

#endif
