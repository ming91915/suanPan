#ifndef TENSORTOOLBOX_H
#define TENSORTOOLBOX_H

#include <suanPan.h>

mat unitDevTensor4();

double tr(const vec&);
double mean(const vec&);
vec dev(const vec&);

mat shapeStress(const vec&, const unsigned&);

mat shapeStress5(const vec&);
mat shapeStress7(const vec&);
mat shapeStress9(const vec&);

mat shapeStrain(const vec&, const double&, const unsigned&);

mat shapeStrain5(const vec&, const unsigned&);
mat shapeStrain7(const vec&, const unsigned&);
mat shapeStrain9(const vec&, const unsigned&);

template <typename T> T sign(const T& I) { return (I > 0.) - (I < 0.); }

#endif
