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

void Section::initialize(const shared_ptr<DomainBase>& D) {
    if(!D->find_material(material_tag)) {
        D->disable_section(get_tag());
        suanpan_error("initialize() cannot find material %u, now disable it.\n", material_tag);
    }
}

const vec& Section::get_resistance() const { return resistance; }

const mat& Section::get_stiffness() const { return stiffness; }

const mat& Section::get_initial_stiffness() const { return initial_stiffness; }

unique_ptr<Section> Section::get_copy() { throw; }

int Section::update_status(const vec&) { throw; }

int Section::update_status(const vec&, const vec&) { throw; }

int Section::clear_status() { throw; }

int Section::commit_status() { throw; }

int Section::reset_status() { throw; }
