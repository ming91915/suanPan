#ifndef ELEMENTPARSER_H
#define ELEMENTPARSER_H

#include <suanPan.h>

using std::istringstream;

class Element;

#ifdef __cplusplus
extern "C" {
#endif
void new_cp3_(unique_ptr<Element>&, istringstream&);
void new_cp4_(unique_ptr<Element>&, istringstream&);
void new_gq12_(unique_ptr<Element>&, istringstream&);
void new_ps_(unique_ptr<Element>&, istringstream&);
void new_qe2_(unique_ptr<Element>&, istringstream&);
void new_truss2d_(unique_ptr<Element>&, istringstream&);
void new_c3d8_(unique_ptr<Element>&, istringstream&);
#ifdef __cplusplus
}
#endif

#endif
