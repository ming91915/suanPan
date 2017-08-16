#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include <suanPan.h>

using std::string;
using std::ifstream;
using std::istringstream;
using std::vector;

class Domain;

void process_file(const shared_ptr<Domain>&, const char*);

void create_new_node(const shared_ptr<Domain>&, istringstream&);
void create_new_element(const shared_ptr<Domain>&, istringstream&);
void create_new_bc(const shared_ptr<Domain>&, istringstream&);
void create_new_cload(const shared_ptr<Domain>&, istringstream&);
void create_new_step(const shared_ptr<Domain>&, istringstream&);

void disable_object(const shared_ptr<Domain>&, istringstream&);

#endif
