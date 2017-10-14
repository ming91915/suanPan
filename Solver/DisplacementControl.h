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
 * @class DisplacementControl
 * @brief A DisplacementControl class defines a solver using Newton--Raphson iteration.
 * @author T
 * @date 15/10/2017
 * @version 0.1.2
 * @file DisplacementControl.h
 * @addtogroup Solver
 * @{
 */

#ifndef DISPLACEMENTCONTROL_H
#define DISPLACEMENTCONTROL_H

#include <Solver/Solver.h>

class DisplacementControl : public Solver {
public:
    DisplacementControl();

    int analyze() override;
};

#endif // DISPLACEMENTCONTROL_H
