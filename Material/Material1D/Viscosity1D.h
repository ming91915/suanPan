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
 * @class Viscosity1D
 * @brief A 1-D Elastic class.
 * @author T
 * @date 27/07/2017
 * @file Viscosity1D.h
 * @addtogroup Material-1D
 * @{
 */

#ifndef VISCOSITY1D_H
#define VISCOSITY1D_H

#include <Material/Material1D/Material1D.h>

class Viscosity1D : public Material1D {
    const double damping, alpha;

public:
    explicit Viscosity1D(const unsigned = 0, const double = 2E5, const double = 1.);

    void initialize(const shared_ptr<DomainBase>&) override;

    unique_ptr<Material> get_copy() override;

    int update_trial_status(const vec&, const vec&) override;

    int clear_status() override;
    int commit_status() override;
    int reset_status() override;

    void print() override;
};

#endif

//! @}
