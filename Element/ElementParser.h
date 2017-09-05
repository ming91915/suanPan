#ifndef ELEMENTPARSER_H
#define ELEMENTPARSER_H

#include <suanPan.h>

using std::istringstream;

class Element;

void new_cp3(unique_ptr<Element>&, istringstream&);
void new_cp4(unique_ptr<Element>&, istringstream&);
void new_gq12(unique_ptr<Element>&, istringstream&);
void new_ps(unique_ptr<Element>&, istringstream&);
void new_qe2(unique_ptr<Element>&, istringstream&);
void new_truss2d(unique_ptr<Element>&, istringstream&);
void new_c3d8(unique_ptr<Element>&, istringstream&);

void new_proto01(unique_ptr<Element>&, istringstream&);

#endif
