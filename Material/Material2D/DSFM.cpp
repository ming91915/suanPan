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

#include "DSFM.h"

DSFM::DSFM(const unsigned) {}

void DSFM::initialize(const shared_ptr<DomainBase>&) {}

unique_ptr<Material> DSFM::get_copy() { throw; }

int DSFM::clear_status() { throw; }

int DSFM::commit_status() { throw; }

int DSFM::reset_status() { throw; }