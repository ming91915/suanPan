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
#include "Tag.h"

/**
 * \brief Default Constructor.
 * \param T Unique Tag
 * \param CT Class Tag
 */
Tag::Tag(const unsigned& T, const unsigned& CT)
    : unique_tag(T)
    , class_tag(CT) {}

/**
 * \brief Empty destructor.
 */
Tag::~Tag() {}

/**
 * \brief Method to set `unique_tag`.
 * \param T Unique Tag
 */
void Tag::set_tag(const unsigned& T) { unique_tag = T; }

/**
 * \brief Method to return `unique_tag`.
 * \return `unique_tag`
 */
const unsigned& Tag::get_tag() const { return unique_tag; }

/**
 * \brief Method to set `class_tag`.
 * \param CT Class Tag
 */
void Tag::set_class_tag(const unsigned& CT) { class_tag = CT; }

/**
 * \brief Method to return `class_tag`.
 * \return `class_tag`
 */
const unsigned& Tag::get_class_tag() const { return class_tag; }

/**
 * \brief Method to enable an object.
 */
void Tag::enable() { alive = true; }

/**
 * \brief Method to disable one object.
 */
void Tag::disable() { alive = false; }

/**
 * \brief Method to return status.
 * \return `alive`
 */
const bool& Tag::is_active() const { return alive; }

/**
 * \brief Method to print basic information.
 */
void Tag::print() { suanpan_info("A Tagged Object.\n"); }
