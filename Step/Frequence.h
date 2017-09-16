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
 * @class Frequence
 * @brief A Frequence class.
 * @author T
 * @date 03/07/2017
 * @version 0.1.0
 * @file Frequence.h
 * @addtogroup Step
 * @{
 */

#ifndef FREQUENCE_H
#define FREQUENCE_H

#include "Step.h"

class Frequence : public Step {
    unsigned eigen_number;

public:
    explicit Frequence(const unsigned& = 0, const unsigned& = 4);

    int initialize() override;

    int analyze() override;

    void set_eigen_number(const unsigned&);
    const unsigned& get_eigen_number() const;
};

#endif

//! @}
