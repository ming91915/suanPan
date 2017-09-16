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

#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include <suanPan.h>

using std::istringstream;
using std::string;

class Bead;
class Domain;

int process_command(const shared_ptr<Bead>&, istringstream&);

int process_file(const shared_ptr<Bead>&, const char*);
int process_file(const shared_ptr<Bead>&, istringstream&);

int create_new_domain(const shared_ptr<Bead>&, istringstream&);
int create_new_converger(const shared_ptr<Bead>&, istringstream&);
int create_new_step(const shared_ptr<Bead>&, istringstream&);
int create_new_solver(const shared_ptr<Bead>&, istringstream&);
int create_new_bc(const shared_ptr<Bead>&, istringstream&);
int create_new_cload(const shared_ptr<Bead>&, istringstream&);
int create_new_dispload(const shared_ptr<Bead>&, istringstream&);

int set_property(const shared_ptr<Bead>&, istringstream&);

int enable_object(const shared_ptr<Bead>&, istringstream&);
int disable_object(const shared_ptr<Bead>&, istringstream&);
int erase_object(const shared_ptr<Bead>&, istringstream&);

int create_new_node(const shared_ptr<Domain>&, istringstream&);
int create_new_material(const shared_ptr<Domain>&, istringstream&);
int create_new_element(const shared_ptr<Domain>&, istringstream&);
int create_new_recorder(const shared_ptr<Domain>&, istringstream&);

int print_info(const shared_ptr<Domain>&, istringstream&);

void print_command_usage(istringstream&);

#endif
