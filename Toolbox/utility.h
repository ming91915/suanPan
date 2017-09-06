#ifndef UTILITY_H
#define UTILITY_H

#include <string>

template <typename T>
T& modifier(const T& I) {
    return const_cast<T&>(I);
}

template <typename T>
T*& modifier(const T* const& I) {
    return const_cast<T*&>(I);
}

template <typename T>
T sign(const T& I) {
    return (I > 0.) - (I < 0.);
}

using std::string;
using std::istringstream;

template <typename T>
bool get_input(istringstream& I, T& O) {
    return !(I >> O).fail();
}

template <typename T>
T get_input(istringstream& I) {
    T O;
    I >> O;
    return O;
}

bool if_equal(const string&, const char*);
bool if_equal(const char*, const char*);

bool if_true(const string&);
bool if_false(const string&);
bool if_true(const char*);
bool if_false(const char*);

#endif