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
#ifndef OUTPUTLIST_H
#define OUTPUTLIST_H

enum class OutputList {
    S,
    S11,
    S22,
    S33,
    S12,
    S23,
    S13,
    E,
    E11,
    E22,
    E33,
    E12,
    E23,
    E13,
    SP,
    SP1,
    SP2,
    SP3,
    EP,
    EP1,
    EP2,
    EP3,
    SINV,
    MISES,
    TRESC,
    PE,
    PE11,
    PE22,
    PE33,
    PE12,
    PE23,
    PE13,
    PEEQ,

    U,
    UT,
    UR,
    U1,
    U2,
    U3,
    UR1,
    UR2,
    UR3,
    V,
    VT,
    VR,
    V1,
    V2,
    V3,
    VR1,
    VR2,
    VR3,
    A,
    AT,
    AR,
    A1,
    A2,
    A3,
    AR1,
    AR2,
    AR3,

    RF,
    RF1,
    RF2,
    RF3,
    RM,
    RM1,
    RM2,
    RM3,
    RT,

    NL
};

const char* to_char(const OutputList&);
OutputList to_list(const char*);

#endif
