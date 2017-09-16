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
 * @class MVLEM
 * @brief The MVLEM class.
 * @author T
 * @date 14/09/2017
 * @version 0.1.1
 * @file MVLEM.h
 * @addtogroup Beam
 * @ingroup Element
 * @{
 */

#ifndef MVLEM_H
#define MVLEM_H

#include <Element/Element.h>

class MVLEM final : public Element {
    static const unsigned b_node, b_dof;
};

#endif

//! @}
