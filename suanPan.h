#ifndef SUANPAN_H
#define SUANPAN_H

#if defined(_DEBUG) || defined(DEBUG)
#define SUANPAN_DEBUG
#else
#define ARMA_NO_DEBUG
#endif

#if defined(__cplusplus)
#endif

#ifdef _MSC_VER
#endif

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) || defined(__WIN32__)
#ifdef SUANPAN_ARCH
#undef SUANPAN_ARCH
#endif
#define SUANPAN_ARCH 32
#ifdef SUANPAN_WIN
#undef SUANPAN_WIN
#endif
#define SUANPAN_WIN
#endif

#if defined(__x86_64) || defined(__x86_64__) || defined(WIN64) || defined(_WIN64) || \
    defined(__WIN64) || defined(__WIN64__)
#ifdef SUANPAN_ARCH
#undef SUANPAN_ARCH
#endif
#define SUANPAN_ARCH 64
#ifdef SUANPAN_WIN
#undef SUANPAN_WIN
#endif
#define SUANPAN_WIN
#endif

#if defined(unix) || defined(__unix__) || defined(__linux__) || defined(linux)
#ifdef SUANPAN_UNIX
#undef SUANPAN_UNIX
#endif
#define SUANPAN_UNIX
#endif

#ifdef __VERSION__
#define SUANPAN_VERSION __VERSION__
#define SUANPAN_COMPILER "GCC"
#endif
#ifdef _MSC_FULL_VER
#define SUANPAN_VERSION _MSC_FULL_VER
#define SUANPAN_COMPILER "MSVC"
#endif

#ifdef _USRDLL
#define DLL_EXPORT extern "C" __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

// auto invalid_code = static_cast<unsigned>(-1);

#include <Toolbox/ClassTag.h>
#include <Toolbox/ElementTag.h>
#include <Toolbox/MaterialTag.h>
#include <Toolbox/debug.h>

#include <armadillo>
#include <memory>

using namespace arma;

using std::cout;

using std::shared_ptr;
using std::unique_ptr;
using std::weak_ptr;

using std::make_shared;
using std::make_unique;

using std::exception;
using std::invalid_argument;
using std::logic_error;
using std::out_of_range;

#endif
