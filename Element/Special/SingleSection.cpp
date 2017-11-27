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

#include "SingleSection.h"
#include <Domain/DomainBase.h>
#include <Domain/Node.h>
#include <Section/Section.h>

const unsigned SingleSection::s_node = 1;
const unsigned SingleSection::s_dof = 2;

SingleSection::SingleSection(const unsigned T, const unsigned NT, const unsigned ST)
    : SectionElement(T, ET_SINGLESECTION, s_node, s_dof, uvec{ NT }, uvec{ ST }, false) {}

void SingleSection::initialize(const shared_ptr<DomainBase>& D) {
    s_section = suanpan::make_copy(D->get_section(unsigned(section_tag(0))));

    initial_stiffness = s_section->get_initial_stiffness();
}

int SingleSection::update_status() {
    const auto& s_node = node_ptr[0].lock();

    s_section->update_trial_status(s_node->get_trial_displacement());

    trial_stiffness = s_section->get_stiffness();

    trial_resistance = s_section->get_resistance();

    return 0;
}

int SingleSection::commit_status() { return s_section->commit_status(); }

int SingleSection::clear_status() { return s_section->clear_status(); }

int SingleSection::reset_status() { return s_section->reset_status(); }
