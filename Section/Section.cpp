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

#include "Section.h"
#include <Domain/DomainBase.h>

Section::Section(const unsigned T, const unsigned CT, const SectionType& ST, const unsigned MT)
    : Tag(T, CT)
    , material_tag(MT)
    , section_type(ST) {}

Section::~Section() {}

void Section::initialize(const shared_ptr<DomainBase>& D) {
    if(!D->find_material(material_tag)) {
        D->disable_section(get_tag());
        suanpan_error("initialize() cannot find material %u, now disable it.\n", material_tag);
        return;
    }

    const auto size = static_cast<unsigned>(section_type);

    eccentricity.zeros(size - 1);

    current_deformation.zeros(size);
    trial_deformation.zeros(size);

    // current_deformation_rate.zeros(size);
    // trial_deformation_rate.zeros(size);

    current_resistance.zeros(size);
    trial_resistance.zeros(size);

    initial_stiffness.zeros(size, size);
    trial_stiffness.zeros(size, size);
    current_stiffness.zeros(size, size);
}

const vec& Section::get_deformation() const { return trial_deformation; }

const vec& Section::get_deformation_rate() const { return trial_deformation_rate; }

const vec& Section::get_resistance() const { return trial_resistance; }

const mat& Section::get_stiffness() const { return trial_stiffness; }

const mat& Section::get_initial_stiffness() const { return initial_stiffness; }

unique_ptr<Section> Section::get_copy() { throw invalid_argument("hidden method called.\n"); }

double Section::get_parameter(const ParameterType&) { return 0.; }

int Section::update_incre_status(const vec& i_deformation) { return update_trial_status(current_deformation + i_deformation); }

int Section::update_incre_status(const vec& i_deformation, const vec& i_deformation_rate) { return update_trial_status(current_deformation + i_deformation, current_deformation_rate + i_deformation_rate); }

int Section::update_trial_status(const vec&) { throw invalid_argument("hidden method called.\n"); }

int Section::update_trial_status(const vec& t_deformation, const vec&) { return update_trial_status(t_deformation); }

int Section::clear_status() { throw invalid_argument("hidden method called.\n"); }

int Section::commit_status() { throw invalid_argument("hidden method called.\n"); }

int Section::reset_status() { throw invalid_argument("hidden method called.\n"); }

unique_ptr<Section> suanpan::make_copy(const shared_ptr<Section>& S) { return S->get_copy(); }

unique_ptr<Section> suanpan::make_copy(const unique_ptr<Section>& S) { return S->get_copy(); }
