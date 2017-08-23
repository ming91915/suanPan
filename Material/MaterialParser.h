#ifndef MATERIALPARSER_H
#define MATERIALPARSER_H

#include <suanPan.h>

using std::istringstream;

class Material;

void new_elastic1d_(unique_ptr<Material>&, istringstream&);
void new_elastic2d_(unique_ptr<Material>&, istringstream&);
void new_elastic3d_(unique_ptr<Material>&, istringstream&);
void new_bilinear1d_(unique_ptr<Material>&, istringstream&);
void new_bilinear3d_(unique_ptr<Material>&, istringstream&);
void new_concrete01_(unique_ptr<Material>&, istringstream&);
void new_gap01_(unique_ptr<Material>&, istringstream&);
void new_mpf_(unique_ptr<Material>&, istringstream&);
void new_rambergosgood_(unique_ptr<Material>&, istringstream&);

#endif
