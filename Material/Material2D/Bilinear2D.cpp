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
#include <array>

const array<unsigned, 3> Bilinear2D::F = { 0, 1, 3 };

Bilinear2D::Bilinear2D(const unsigned& T, const double& E, const double& V, const double& Y, const double& H, const double& B, const PlaneType& M, const double& D)
    : Material2D(T, MT_BILINEAR2D, M, D)
    , base(0, E, V, Y, H, B, D) {
    const auto EE = plane_type == PlaneType::S ? E : E / (1. - V * V);
    const auto VV = plane_type == PlaneType::S ? V : V / (1. - V);

    const auto t_factor = EE / (1. - VV * VV);

    initial_stiffness.zeros(3, 3);
    initial_stiffness(0, 1) = initial_stiffness(1, 0) = VV * (initial_stiffness(0, 0) = initial_stiffness(1, 1) = t_factor);
    initial_stiffness(2, 2) = .5 * t_factor * (1. - VV);
}

void Bilinear2D::initialize(const shared_ptr<DomainBase>&) {
    base.Material::initialize();
    base.initialize();

    trial_full_strain.zeros(6);

    for(auto I = 0; I < 3; ++I)
        for(auto J = 0; J < 3; ++J) initial_stiffness(I, J) = base.get_initial_stiffness().at(F[I], F[J]);

    trial_stiffness = current_stiffness = initial_stiffness;
}

double Bilinear2D::get_parameter(const ParameterType& T) const { return base.get_parameter(T); }

unique_ptr<Material> Bilinear2D::get_copy() { return make_unique<Bilinear2D>(*this); }

int Bilinear2D::update_trial_status(const vec& t_strain) {
    trial_strain = t_strain;

    for(auto I = 0; I < 3; ++I) trial_full_strain(F[I]) = trial_strain(I);

    base.update_trial_status(trial_full_strain);

    // PLANE STRESS
    if(plane_type == PlaneType::S) {
        auto& stress_anchor = base.get_stress().at(2);
        auto counter = 0;
        while(true) {
            if(fabs(stress_anchor) < 1E-10) break;
            if(++counter > 20) {
                suanpan_warning("cannot converge in 20 iterations.\n");
                break;
            }
            trial_full_strain(2) -= stress_anchor / base.get_stiffness().at(2, 2);
            base.update_trial_status(trial_full_strain);
        }
    }

    for(auto I = 0; I < 3; ++I) trial_stress(I) = base.get_stress().at(F[I]);

    for(auto I = 0; I < 3; ++I)
        for(auto J = 0; J < 3; ++J) trial_stiffness(I, J) = base.get_stiffness().at(F[I], F[J]);

    if(plane_type == PlaneType::S) {
        auto& t_stiffness = base.get_stiffness();
        if(t_stiffness(2, 2) != 0.) {
            for(auto I = 0; I < 3; ++I)
                for(auto J = 0; J < 3; ++J) trial_stiffness(I, J) -= t_stiffness(F[I], 2) * t_stiffness(2, F[J]) / t_stiffness(2, 2);
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
