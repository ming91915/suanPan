#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include <suanPan.h>

using std::istringstream;

class Bead;
class Domain;

int process_command(const shared_ptr<Bead>&, istringstream&);

void process_file(const shared_ptr<Bead>&, const char*);
void process_file(const shared_ptr<Bead>&, istringstream&);

void create_new_domain(const shared_ptr<Bead>&, istringstream&);
void create_new_converger(const shared_ptr<Bead>&, istringstream&);
void create_new_step(const shared_ptr<Bead>&, istringstream&);
void create_new_recorder(const shared_ptr<Bead>&, istringstream&);

void disable_object(const shared_ptr<Bead>&, istringstream&);
void erase_object(const shared_ptr<Bead>&, istringstream&);

void create_new_node(const shared_ptr<Domain>&, istringstream&);
void create_new_material(const shared_ptr<Domain>&, istringstream&);
void create_new_element(const shared_ptr<Domain>&, istringstream&);
void create_new_bc(const shared_ptr<Domain>&, istringstream&);
void create_new_cload(const shared_ptr<Domain>&, istringstream&);

void print_info(const shared_ptr<Domain>&, istringstream&);

#endif
