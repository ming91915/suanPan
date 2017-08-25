#pragma once

#include <Toolbox/debug.h>
#include <sstream>

template <typename T> T sign(const T& I) { return (I > 0.) - (I < 0.); }

using std::string;
using std::istringstream;

template <typename T> bool get_input(istringstream& I, T& O) { return !(I >> O).fail(); }

template <typename T> T get_input(istringstream& I)
{
    T O;
    I >> O;
    return O;
}
