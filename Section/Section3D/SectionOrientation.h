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
/**
 * @class SectionOrientation
 * @brief A SectionOrientation class.
 * @author T
 * @date 27/10/2017
 * @version 0.1.0
 * @file SectionOrientation.h
 * @addtogroup Section-3D
 * @ingroup Section
 * @{
 */

#ifndef SECTIONORIENTATION_H
#define SECTIONORIENTATION_H

#include <Section/Section.h>

class SectionOrientation : public Tag {
public:
    explicit SectionOrientation(const unsigned T = 0, const double X = 0., const double Y = 0., const double Z = 0.);
};

#endif

//! @}
