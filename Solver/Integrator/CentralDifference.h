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
 * @class CentralDifference
 * @brief A CentralDifference class defines a solver using Newmark
 * algorithm.
 *
 * @author T
 * @date 29/07/2017
 * @version 0.1.0
 * @file CentralDifference.h
 * @addtogroup Integrator
 * @{
 */

#ifndef CENTRALDIFFERENCE_H
#define CENTRALDIFFERENCE_H

#include "Integrator.h"

class CentralDifference : public Integrator {
    double max_dt = 0.;

    double DT = 0.;

    double C0 = 0., C1 = 0., C2 = 0., C3 = 0.;

    void update_parameter();

public:
    explicit CentralDifference(const unsigned& = 0);

    int initialize() override;

    void assemble_resistance() override;
    void assemble_matrix() override;

    void commit_status() const override;
};

#endif

//! @}
