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
#include "utility.h"
#include <cstring>
#include <suanPan.h>

bool if_equal(const char* A, const char* B) { return _strcmpi(A, B) == 0; }

bool if_equal(const string& A, const char* B) { return if_equal(A.c_str(), B); }

bool if_true(const char* S) { return if_equal(S, "On") || if_equal(S, "True") || if_equal(S, "1") || if_equal(S, "Yes"); }

bool if_false(const char* S) { return if_equal(S, "Off") || if_equal(S, "False") || if_equal(S, "0") || if_equal(S, "No"); }

bool if_true(const string& S) { return if_true(S.c_str()); }

bool if_false(const string& S) { return if_false(S.c_str()); }
