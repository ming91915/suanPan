////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2017 Theodore Chang
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////

#include "CDP.h"
#include <Toolbox/tensorToolbox.h>
#include <suanPan>

CDP::CDP(const unsigned T, const double E, const double V, const double AP, const double H, const double B, const double R)
    : Material3D(T, MT_CDP, R)
    , elastic_modulus(E)
    , poissons_ratio(V)
    , shear_modulus(.5 * elastic_modulus / (1. + poissons_ratio))
    , bulk_modulus(elastic_modulus / (3. - 6. * poissons_ratio))
    , alpha_p(AP) {}

void CDP::initialize(const shared_ptr<DomainBase>&) {}

unique_ptr<Material> CDP::get_copy() { return make_unique<CDP>(*this); }

int CDP::update_trial_status(const vec& t_strain) {
    // convert voigt form to tensor form 3-by-3 matrix
    const auto stress_tensor = tensor::stress::to_tensor(trial_stress);
    // get deviatoric stress
    const auto dev_stress_tensor = tensor::dev(stress_tensor);

    // solve for principal stress tensor
    vec principal_stress;
    mat trans_mat;
    eig_sym(principal_stress, trans_mat, stress_tensor);

    // get deviatoric principal stress
    const vec dev_principal_stress = principal_stress - mean(principal_stress);

    // set anchors pay attention to the order
    const auto& p_stress_1 = dev_principal_stress(2);
    const auto& p_stress_2 = dev_principal_stress(1);
    const auto& p_stress_3 = dev_principal_stress(0);

    const auto I1 = tensor::invariant1(principal_stress);
    const auto J2 = tensor::invariant2(dev_principal_stress);

    return 0;
}

int CDP::clear_status() {
    current_strain.zeros();
    current_stress.zeros();
    current_stiffness = initial_stiffness;

    return reset_status();
}

int CDP::commit_status() {
    current_strain = trial_strain;
    current_stress = trial_stress;
    current_stiffness = trial_stiffness;

    return 0;
}

int CDP::reset_status() {
    trial_strain = current_strain;
    trial_stress = current_stress;
    trial_stiffness = current_stiffness;

    return 0;
}
