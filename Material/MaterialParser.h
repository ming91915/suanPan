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
