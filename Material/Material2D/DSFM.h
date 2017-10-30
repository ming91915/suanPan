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
 * @class DSFM
 * @brief A DSFM material class.
 * @author T
 * @date 03/10/2017
 * @version 0.1.1
 * @file DSFM.h
 * @addtogroup Material-2D
 * @{
 */

#ifndef DSFM_H
#define DSFM_H

#include <Material/Material2D/Material2D.h>

class DSFM : public Material2D {
public:
    explicit DSFM(const unsigned // tag
    );

    void initialize(const shared_ptr<DomainBase>&) override;
    unique_ptr<Material> get_copy() override;
    int clear_status() override;
    int commit_status() override;
    int reset_status() override;
};

#endif

//! @}
