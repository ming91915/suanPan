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
 * @class RC01
 * @brief A RC01 material class.
 * @author T
 * @date 03/10/2017
 * @version 0.1.1
 * @file RC01.h
 * @addtogroup Material-2D
 * @{
 */

#ifndef RC01_H
#define RC01_H

#include <Material/Material2D/Material2D.h>

class RC01 : public Material2D {
    const unsigned rebar_tag, concrete_tag;

    unique_ptr<Material> rebar;
    unique_ptr<Material> concrete;

public:
    explicit RC01(const unsigned, // tag
        const unsigned,           // rebar layer tag
        const unsigned            // concrete tag
    );
    RC01(const RC01&);

    void initialize(const shared_ptr<DomainBase>& = nullptr) override;

    unique_ptr<Material> get_copy() override;

    int update_trial_status(const vec&) override;

    int clear_status() override;
    int commit_status() override;
    int reset_status() override;
};

#endif

//! @}
