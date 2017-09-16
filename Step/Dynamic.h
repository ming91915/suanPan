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
 * @class Dynamic
 * @brief A Dynamic class.
 * @author T
 * @date 03/07/2017
 * @file Dynamic.h
 * @addtogroup Step
 * @{
 */

#ifndef DYNAMIC_H
#define DYNAMIC_H

#include "Step.h"

class Dynamic : public Step {
public:
    explicit Dynamic(const unsigned& T = 0, const double& P = 1.)
        : Step(T, CT_DYNAMIC, P) {}

    virtual ~Dynamic() {}

    int analyze() override;
};

#endif

//! @}
