#include "debug.h"
#include <cstdarg>
#include <iostream>
#include <vector>

using std::vector;
using std::cout;

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
