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

#include "Concrete2D.h"
#include <Toolbox/tensorToolbox.h>

Concrete2D::Concrete2D(const unsigned& T, const unsigned& XT, const unsigned& YT, const double& R)
    : Material(T, MT_CONCRETE2D, MaterialType::D2, R)
    , tag_major(XT)
    , tag_minor(YT) {}

Concrete2D::Concrete2D(const Concrete2D& P)
    : Material(P.get_tag(), MT_CONCRETE2D, MaterialType::D2, P.density)
    , tag_major(P.tag_major)
    , tag_minor(P.tag_minor) {
    concrete_major = P.concrete_major->get_copy();
    concrete_minor = P.concrete_minor->get_copy();
}

void Concrete2D::initialize(const shared_ptr<DomainBase>& D) {
    if(D->find_material(tag_major)) {
        concrete_major = D->get_material(tag_major)->get_copy();
        if(!concrete_major->initialized) {
            concrete_major->Material::initialize(D);
            concrete_major->initialize(D);
        }
    } else {
        suanpan_error("initialize() cannot find a proper defined material with tag %u.\n", tag_major);
        return;
    }
    if(D->find_material(tag_minor)) {
        concrete_minor = D->get_material(tag_minor)->get_copy();
        if(!concrete_minor->initialized) {
            concrete_minor->Material::initialize(D);
            concrete_minor->initialize(D);
        }
    } else {
        suanpan_error("initialize() cannot find a proper defined material with tag %u.\n", tag_minor);
        return;
    }

    initial_stiffness.zeros(3, 3);
    const auto& E11 = concrete_major->get_initial_stiffness().at(0);
    const auto& E22 = concrete_minor->get_initial_stiffness().at(0);

    initial_stiffness(0, 0) = E11;
    initial_stiffness(1, 1) = E22;
    initial_stiffness(2, 2) = E11 * E22 / (E11 + E22);

    trial_stiffness = initial_stiffness;
    current_stiffness = initial_stiffness;
}

unique_ptr<Material> Concrete2D::get_copy() { return make_unique<Concrete2D>(*this); }

int Concrete2D::update_incre_status(const vec& i_strain) { return update_trial_status(current_strain + i_strain); }

int Concrete2D::update_trial_status(const vec& t_strain) {
    trial_strain = t_strain;

    // store a copy as will be modified
    auto p_strain = trial_strain;

    // transform to principal direction
    const auto theta = principal_strain(p_strain);

    // update status
    concrete_major->update_trial_status(vec{ p_strain(0) });
    concrete_minor->update_trial_status(vec{ p_strain(1) });

    vec p_stress(3, fill::zeros);

    // collect principal stress components
    p_stress(0) = concrete_major->get_stress().at(0);
    p_stress(1) = concrete_minor->get_stress().at(0);

    // transform back to nominal direction
    trial_stress = nominal_stress(p_stress, theta);

    const auto& E11 = concrete_major->get_stiffness().at(0);
    const auto& E22 = concrete_minor->get_stiffness().at(0);

    // collect stiffness components
    trial_stiffness.zeros(3, 3);
    trial_stiffness(0, 0) = E11;
    trial_stiffness(1, 1) = E22;
    trial_stiffness(2, 2) = E11 * E22 / (E11 + E22);

    const auto sin_theta = sin(theta);
    const auto cos_theta = cos(theta);
    const auto sin_square = sin_theta * sin_theta;
    const auto cos_square = cos_theta * cos_theta;
    const auto sin_cos = cos_theta * sin_theta;

    mat trans(3, 3);
    trans(0, 0) = cos_square;
    trans(0, 1) = sin_square;
    trans(0, 2) = sin_cos;
    trans(1, 0) = sin_square;
    trans(1, 1) = cos_square;
    trans(1, 2) = -sin_cos;
    trans(2, 0) = -2. * sin_cos;
    trans(2, 1) = sin_cos;
    trans(2, 2) = cos_square - sin_square;

    trial_stiffness = trans.t() * trial_stiffness * trans;

    return 0;
}

int Concrete2D::clear_status() {
    concrete_major->clear_status();
    concrete_minor->clear_status();
    return 0;
}

int Concrete2D::commit_status() {
    concrete_major->commit_status();
    concrete_minor->commit_status();
    return 0;
}

int Concrete2D::reset_status() {
    concrete_major->reset_status();
    concrete_minor->reset_status();
    return 0;
}
