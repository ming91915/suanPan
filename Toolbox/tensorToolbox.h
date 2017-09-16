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

mat unitDevTensor4();

double tr(const vec&);
double mean(const vec&);
vec dev(const vec&);

mat shapeStress(const double&, const double&, const unsigned&);
mat shapeStress(const vec&, const unsigned&);

mat shapeStress5(const double&, const double&);
mat shapeStress7(const double&, const double&);
mat shapeStress9(const double&, const double&);
mat shapeStress11(const double&, const double&);

mat shapeStress5(const vec&);
mat shapeStress7(const vec&);
mat shapeStress9(const vec&);
mat shapeStress11(const vec&);

mat shapeStrain(const double&, const double&, const double&, const unsigned&);
mat shapeStrain(const vec&, const double&, const unsigned&);

mat shapeStrain5(const double&, const double&, const double&);
mat shapeStrain7(const double&, const double&, const double&);
mat shapeStrain9(const double&, const double&, const double&);
mat shapeStrain11(const double&, const double&, const double&);

mat shapeStrain5(const vec&, const double&);
mat shapeStrain7(const vec&, const double&);
mat shapeStrain9(const vec&, const double&);
mat shapeStrain11(const vec&, const double&);

#endif
