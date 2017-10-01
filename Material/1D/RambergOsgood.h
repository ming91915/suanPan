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
 * @class RambergOsgood
 * @brief A RambergOsgood material class.
 * @author T
 * @date 01/10/2017
 * @version 0.1.2
 * @file RambergOsgood.h
 * @addtogroup Material-1D
 * @ingroup Material
 * @{
 */

#ifndef RAMBERGOSGOOD_H
#define RAMBERGOSGOOD_H

#include <Material/Material.h>

class RambergOsgood final : public Material {
    const double elastic_modulus;
    const double yield_stress;
    const double offset;
    const double n;
    const double nm;

    const double tolerance;

public:
    explicit RambergOsgood(const unsigned& = 0, // tag
        const double& = 2E5,                    // elastic modulus
        const double& = 400.,                   // yield stress
        const double& = 1.,                     // offset
        const double& = 20.,                    // n
        const double& = 0.                      // density
    );

    void initialize() override;

    unique_ptr<Material> get_copy() override;

    int update_incre_status(const vec&) override;
    int update_trial_status(const vec&) override;

    int clear_status() override;
    int commit_status() override;
    int reset_status() override;
};

#endif

//! @}
