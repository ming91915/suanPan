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
 * @class CDP
 * @brief The CDP class.
 * @author T
 * @date 08/08/2017
 * @version 0.1.0
 * @file CDP.h
 * @addtogroup Material-3D
 * @{
 */

#ifndef CDP_H
#define CDP_H

#include <Material/Material3D/Material3D.h>

class CDP : public Material3D {
    const double alpha_p; /**< dilatancy */
public:
    explicit CDP(const unsigned = 0, /**< tag */
        const double = 2E5,          /**< elastic modulus */
        const double = .25,          /**< poisson's ratio */
        const double = 400.,         /**< initial yield stress */
        const double = 0.,           /**< hardening ratio */
        const double = 0.,           /**< isotropic/kinematic hardening factor */
        const double = 0.);          /**< density */

    void initialize(const shared_ptr<DomainBase>& = nullptr) override;

    unique_ptr<Material> get_copy() override;

    int update_trial_status(const vec&) override;

    int clear_status() override;
    int commit_status() override;
    int reset_status() override;
};

#endif

//! @}
