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
#ifndef ELEMENTPARSER_H
#define ELEMENTPARSER_H

#include <suanPan.h>

using std::istringstream;

class Element;

void new_cp3(unique_ptr<Element>&, istringstream&);
void new_cp4(unique_ptr<Element>&, istringstream&);
void new_cp8(unique_ptr<Element>&, istringstream&);
void new_gq12(unique_ptr<Element>&, istringstream&);
void new_ps(unique_ptr<Element>&, istringstream&);
void new_qe2(unique_ptr<Element>&, istringstream&);
void new_truss2d(unique_ptr<Element>&, istringstream&);
void new_c3d8(unique_ptr<Element>&, istringstream&);
void new_c3d20(unique_ptr<Element>&, istringstream&);

void new_proto01(unique_ptr<Element>&, istringstream&);

#endif
