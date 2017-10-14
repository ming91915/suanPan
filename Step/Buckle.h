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
 * @class Buckle
 * @brief A Buckle class.
 * @author T
 * @date 11/10/2017
 * @version 0.1.0
 * @file Buckle.h
 * @addtogroup Step
 * @{
 */

#ifndef BUCKLE_H
#define BUCKLE_H

#include <Step/Step.h>

class Buckle : public Step {
public:
    explicit Buckle(const unsigned& = 0, const unsigned& = 4);

    int initialize() override;

    int analyze() override;
};

#endif

//! @}
