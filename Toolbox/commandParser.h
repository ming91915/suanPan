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
