/*******************************************************************************
 * Copyright (C) 2017 Theodore Chang
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

#ifndef UTILITY_H
#define UTILITY_H

#include <sstream>
#include <string>

namespace suanpan {
template <typename T> T& modifier(const T& I) { return const_cast<T&>(I); }

template <typename T> T*& modifier(const T* const& I) { return const_cast<T*&>(I); }

template <typename T> T sign(const T& I) { return (I > 0.) - (I < 0.); }
}

using std::istringstream;
using std::string;

template <typename T> bool get_input(istringstream& I, T& O) { return !(I >> O).fail(); }

template <typename T> T get_input(istringstream& I) {
    T O;
    I >> O;
    return O;
}

template <typename T> bool get_optional_input(istringstream& I, T& O) { return !I.eof() && !(I >> O).fail(); }

bool is_equal(const char*, const char*);
bool is_equal(const string&, const char*);
bool is_equal(const string&, const string&);

bool is_true(const char*);
bool is_false(const char*);
bool is_true(const string&);
bool is_false(const string&);

#endif
