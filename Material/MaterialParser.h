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
#ifndef MATERIALPARSER_H
#define MATERIALPARSER_H

#include <suanPan.h>

using std::istringstream;

class Material;

void new_elastic1d(unique_ptr<Material>&, istringstream&);
void new_elastic2d(unique_ptr<Material>&, istringstream&);
void new_elastic3d(unique_ptr<Material>&, istringstream&);
void new_bilinear1d(unique_ptr<Material>&, istringstream&);
void new_bilinear2d(unique_ptr<Material>&, istringstream&);
void new_bilinear3d(unique_ptr<Material>&, istringstream&);
void new_concrete01(unique_ptr<Material>&, istringstream&);
void new_gap01(unique_ptr<Material>&, istringstream&);
void new_mpf(unique_ptr<Material>&, istringstream&);
void new_rambergosgood(unique_ptr<Material>&, istringstream&);

#endif
