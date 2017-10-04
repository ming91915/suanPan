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

#include "Bilinear2D.h"

Bilinear2D::Bilinear2D(const unsigned& T, const double& E, const double& V, const double& Y, const double& H, const double& B, const PlaneType& M, const double& D)
    : Material(T, MT_BILINEAR2D, MaterialType::D2, D)
    , plane_type(M)
    , base(0, E, V, Y, H, B, D) {
    const auto EE = plane_type == PlaneType::S ? E : E / (1 - V * V);
    const auto VV = plane_type == PlaneType::S ? V : V / (1 - V);

    initial_stiffness.zeros(3, 3);
    initial_stiffness(0, 0) = 1;
    initial_stiffness(1, 1) = 1;
    initial_stiffness(2, 2) = (1. - VV) / 2.;
    initial_stiffness(0, 1) = VV;
    initial_stiffness(1, 0) = VV;
    initial_stiffness *= EE / (1. - VV * VV);
}

void Bilinear2D::initialize(const shared_ptr<DomainBase>&) {
    trial_full_strain.zeros(6);

    current_stiffness = initial_stiffness;
    trial_stiffness = initial_stiffness;
}

double Bilinear2D::get_parameter(const unsigned& T) const { return base.get_parameter(T); }

unique_ptr<Material> Bilinear2D::get_copy() { return make_unique<Bilinear2D>(*this); }

int Bilinear2D::update_incre_status(const vec& i_strain) { return update_trial_status(current_strain + i_strain); }

int Bilinear2D::update_trial_status(const vec& t_strain) {
    trial_full_strain(0) = t_strain(0);
    trial_full_strain(1) = t_strain(1);
    trial_full_strain(3) = t_strain(2);

    base.update_trial_status(trial_full_strain);

    // PLANE STRESS
    if(plane_type == PlaneType::S)
        while(fabs(base.get_stress().at(2)) > 1E-10) {
            trial_full_strain(2) -= base.get_stress().at(2) / base.get_stiffness().at(2, 2);
            base.update_trial_status(trial_full_strain);
        }

    auto& tmp_strain = base.get_strain();
    trial_strain(0) = tmp_strain.at(0);
    trial_strain(1) = tmp_strain.at(1);
    trial_strain(2) = tmp_strain.at(3);

    auto& tmp_stress = base.get_stress();
    trial_stress(0) = tmp_stress.at(0);
    trial_stress(1) = tmp_stress.at(1);
    trial_stress(2) = tmp_stress.at(3);

    auto& tmp_stiffness = base.get_stiffness();
    trial_stiffness(0, 0) = tmp_stiffness.at(0, 0);
    trial_stiffness(0, 1) = tmp_stiffness.at(0, 1);
    trial_stiffness(0, 2) = tmp_stiffness.at(0, 3);
    trial_stiffness(1, 0) = tmp_stiffness.at(1, 0);
    trial_stiffness(1, 1) = tmp_stiffness.at(1, 1);
    trial_stiffness(1, 2) = tmp_stiffness.at(1, 3);
    trial_stiffness(2, 0) = tmp_stiffness.at(3, 0);
    trial_stiffness(2, 1) = tmp_stiffness.at(3, 1);
    trial_stiffness(2, 2) = tmp_stiffness.at(3, 3);

    if(plane_type == PlaneType::S) {
        if(tmp_stiffness(2, 2) != 0.) {
            const auto tmp_a = tmp_stiffness(2, 0) / tmp_stiffness(2, 2);
            const auto tmp_b = tmp_stiffness(2, 1) / tmp_stiffness(2, 2);
            const auto tmp_c = tmp_stiffness(2, 3) / tmp_stiffness(2, 2);
            trial_stiffness(0, 0) -= tmp_stiffness(0, 2) * tmp_a;
            trial_stiffness(0, 1) -= tmp_stiffness(0, 2) * tmp_b;
            trial_stiffness(0, 2) -= tmp_stiffness(0, 2) * tmp_c;
            trial_stiffness(1, 0) -= tmp_stiffness(1, 2) * tmp_a;
            trial_stiffness(1, 1) -= tmp_stiffness(1, 2) * tmp_b;
            trial_stiffness(1, 2) -= tmp_stiffness(1, 2) * tmp_c;
            trial_stiffness(2, 0) -= tmp_stiffness(3, 2) * tmp_a;
            trial_stiffness(2, 1) -= tmp_stiffness(3, 2) * tmp_b;
            trial_stiffness(2, 2) -= tmp_stiffness(3, 2) * tmp_c;
        } else
            suanpan_error("K(2,2)=0.\n");
    }

    return 0;
}

int Bilinear2D::clear_status() {
    trial_full_strain.zeros();

    current_strain.zeros();
    current_stress.zeros();
    current_stiffness = initial_stiffness;

    trial_strain.zeros();
    trial_stress.zeros();
    trial_stiffness = initial_stiffness;

    return base.clear_status();
}

int Bilinear2D::commit_status() {
    current_strain = trial_strain;
    current_stress = trial_stress;
    current_stiffness = trial_stiffness;

    return base.commit_status();
}

int Bilinear2D::reset_status() {
    trial_strain = current_strain;
    trial_stress = current_stress;
    trial_stiffness = current_stiffness;

    return base.reset_status();
}
