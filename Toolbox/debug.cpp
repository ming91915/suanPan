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

#include "debug.h"
#include <cstdarg>
#include <iostream>
#include <suanPan.h>
#include <vector>

using std::cout;
using std::vector;

void suanpan_info(const char* M, ...) {
    va_list arguments_a, arguments_b;
    va_start(arguments_a, M);
    va_copy(arguments_b, arguments_a);
    vector<char> buffer(1 + vsnprintf(nullptr, 0, M, arguments_a));
    va_end(arguments_a);
    vsnprintf(&buffer[0], buffer.size(), M, arguments_b);
    va_end(arguments_b);
    cout << buffer.data();
}

void suanpan_debug(const char* M, ...) {
#ifdef SUANPAN_DEBUG
    cout << "debug: ";
    va_list arguments_a, arguments_b;
    va_start(arguments_a, M);
    va_copy(arguments_b, arguments_a);
    vector<char> buffer(1 + vsnprintf(nullptr, 0, M, arguments_a));
    va_end(arguments_a);
    vsnprintf(&buffer[0], buffer.size(), M, arguments_b);
    va_end(arguments_b);
    cout << buffer.data();
#endif
}

void suanpan_warning(const char* M, ...) {
    cout << "warning: ";
    va_list arguments_a, arguments_b;
    va_start(arguments_a, M);
    va_copy(arguments_b, arguments_a);
    vector<char> buffer(1 + vsnprintf(nullptr, 0, M, arguments_a));
    va_end(arguments_a);
    vsnprintf(&buffer[0], buffer.size(), M, arguments_b);
    va_end(arguments_b);
    cout << buffer.data();
}

void suanpan_error(const char* M, ...) {
    cout << "error: ";
    va_list arguments_a, arguments_b;
    va_start(arguments_a, M);
    va_copy(arguments_b, arguments_a);
    vector<char> buffer(1 + vsnprintf(nullptr, 0, M, arguments_a));
    va_end(arguments_a);
    vsnprintf(&buffer[0], buffer.size(), M, arguments_b);
    va_end(arguments_b);
    cout << buffer.data();
}

void suanpan_fatal(const char* M, ...) {
    cout << "fatal: ";
    va_list arguments_a, arguments_b;
    va_start(arguments_a, M);
    va_copy(arguments_b, arguments_a);
    vector<char> buffer(1 + vsnprintf(nullptr, 0, M, arguments_a));
    va_end(arguments_a);
    vsnprintf(&buffer[0], buffer.size(), M, arguments_b);
    va_end(arguments_b);
    cout << buffer.data();
}
