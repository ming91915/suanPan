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

#ifndef TENSORTOOLBOX_H
#define TENSORTOOLBOX_H

#include <suanPan.h>

namespace tensor {
vec unitTensor2();
mat unitDevTensor4();
mat unitSymTensor4();

double tr(const vec&);
double mean(const vec&);
vec dev(const vec&);

/**
 * \brief takes principal tensor in column vector form (6) only
 * \return I1 invariant
 */
double I1(const vec&);
/**
 * \brief takes principal tensor in column vector form (6) only
 * \return I2 invariant
 */
double I2(const vec&);
/**
 * \brief takes principal tensore in column vector form (6) only
 * \return I3 invariant
 */
double I3(const vec&);
/**
 * \brief takes principal tensor in column vector form (6) only
 * \return J1 invariant
 */
double J1(const vec&);
/**
 * \brief takes principal tensor in column vector form (6) only
 * \return J2 invariant
 */
double J2(const vec&);
/**
 * \brief takes principal tensor in column vector form (6) only
 * \return J3 invariant
 */
double J3(const vec&);
}

namespace transform {
double atan2(const vec&);
namespace strain {
    double angle(const vec&);
    mat trans(const double);
    vec principal(const vec&);
    vec rotate(const vec&, const double);
    mat tensor(const vec&);
    vec voigt(const mat&);
}
namespace stress {
    double angle(const vec&);
    mat trans(const double);
    vec principal(const vec&);
    vec rotate(const vec&, const double);
    mat tensor(const vec&);
    vec voigt(const mat&);
}
namespace beam {
    mat global_to_local(const double, const double, const double, const double);
    mat global_to_local(const double, const double, const double);
    mat global_to_local(const vec& direction_cosine, const double length);
}
}

#endif
