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

#include "Constraint.h"
#include <Domain/DomainBase.h>

/**
 * \brief The default constructor.
 * \param T `unique_tag`
 * \param CT `class_tag`
 */
Constraint::Constraint(const unsigned& T, const unsigned& CT, const unsigned& ST)
    : Tag(T, CT)
    , step_tag(ST) {}

/**
 * \brief The default destructor.
 */
Constraint::~Constraint() { suanpan_debug("Constraint %u dtor() called.\n", get_tag()); }

/**
 * \brief Method to set `step_tag`.
 * \param ST `step_tag`
 */
void Constraint::set_step_tag(const unsigned& ST) { step_tag = ST; }

/**
 * \brief Method to get `step_tag`.
 * \return `step_tag`
 */
const unsigned& Constraint::get_step_tag() const { return step_tag; }

/**
 * \brief Method to apply the Constraint to the system.
 * \return Number of DoFs Modified
 */
int Constraint::process(const shared_ptr<DomainBase>&) { return -1; }
