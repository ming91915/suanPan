#ifndef ELEMENTPARSER_H
#define ELEMENTPARSER_H

#include <suanPan.h>

using std::istringstream;

class Element;

#ifdef __cplusplus
extern "C" {
#endif

void new_cp4_(unique_ptr<Element>&, istringstream&);

#ifdef __cplusplus
}
#endif

#endif
