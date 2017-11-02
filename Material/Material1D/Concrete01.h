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
 * @class Concrete01
 * @brief A Concrete01 material class.
 * @author T
 * @date 01/10/2017
 * @version 0.1.1
 * @file Concrete01.h
 * @addtogroup Material-1D
 * @{
 */

#ifndef CONCRETE01_H
#define CONCRETE01_H

#include <Material/Material1D/Material1D.h>

enum class BackboneType { THORENFELDT, POPOVICS, TSAI, KPSC, KPSU };

class Concrete01 : public Material1D {
    static const double crack_strain;

    const double peak_strain, peak_stress;

    double crack_stress = 0., d_factor = 0.;

    const BackboneType backbone_type;

    bool center_oriented;

    double M = 0., N = 0.;

    bool on_compression_backbone = true;
    bool on_tension_backbone = true;

    void compute_compression_backbone();
    void compute_tension_backbone();

public:
    Concrete01(const unsigned&, // tag
        const double&,          // peak strain in negative
        const double&,          // peak stress in negative
        const BackboneType&,    // backbone type
        const bool& = false,    // center oriented or using unloading criterion
        const double& = 0.);

    void initialize(const shared_ptr<DomainBase>&) override;

    unique_ptr<Material> get_copy() override;

    int update_trial_status(const vec&) override;

    int clear_status() override;
    int commit_status() override;
    int reset_status() override;
};

#endif

//! @}
