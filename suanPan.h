#ifndef SUANPAN_H
#define SUANPAN_H

// SUANPAN_DEBUG
// _DEBUG --> MSVC
// DEBUG --> GCC
#if defined(_DEBUG) || defined(DEBUG)
#define SUANPAN_DEBUG
#else
#define ARMA_NO_DEBUG
#endif

#if defined(__cplusplus)
#endif

// SUANPAN_ARCH SUANPAN_WIN
// WIN32 _WIN32 __WIN32 __WIN32__ --> MSVC GCC
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) || defined(__WIN32__)
#ifdef SUANPAN_ARCH
#undef SUANPAN_ARCH
#endif
#define SUANPAN_ARCH 32
#ifndef SUANPAN_WIN
#define SUANPAN_WIN
#endif
#endif

// SUANPAN_ARCH SUANPAN_WIN
#if defined(WIN64) || defined(_WIN64) || defined(__WIN64) || defined(__WIN64__)
#ifdef SUANPAN_ARCH
#undef SUANPAN_ARCH
#endif
#define SUANPAN_ARCH 64
#ifndef SUANPAN_WIN
#define SUANPAN_WIN
#endif
#endif

#ifdef SUANPAN_WIN
#ifndef NOMINMAX
#define NOMINMAX
#endif
#endif

// SUANPAN_UNIX
#if defined(unix) || defined(__unix__) || defined(__linux__) || defined(linux)
#ifndef SUANPAN_UNIX
#define SUANPAN_UNIX
#endif
#if __x86_64 == 1
#define SUANPAN_ARCH 64
#else
#define SUANPAN_ARCH 32
#endif
#endif

// SUANPAN_VERSION SUANPAN_COMPILER
// __VERSION__ --> GCC
#ifdef __VERSION__
#define SUANPAN_VERSION __VERSION__
#define SUANPAN_COMPILER "GCC"
#endif
// _MSC_FULL_VER --> MSVC
#ifdef _MSC_FULL_VER
#define SUANPAN_VERSION _MSC_FULL_VER
#define SUANPAN_COMPILER "MSVC"
#endif

// SUANPAN_DLL
#ifdef _USRDLL
#ifndef SUANPAN_DLL
#define SUANPAN_DLL
#endif
#endif

#ifdef SUANPAN_DLL
#ifdef SUANPAN_WIN
#define SUANPAN_EXPORT extern "C" __declspec(dllexport) // WIN MSVC GCC EXPORT
#elif defined(SUANPAN_UNIX)
#define SUANPAN_EXPORT extern "C" // UNIX GCC EXPORT
#else
#define SUANPAN_EXPORT // EMPTY
#endif
#else
#define SUANPAN_EXPORT // EMPTY
#endif

#ifndef SUANPAN_EXIT
#define SUANPAN_EXIT 1
#endif

// #define SUANPAN_OPENMP

// TWO IMPLEMENTATIONS
#if defined(SUANPAN_UNIX)
#define _strcmpi strcasecmp
#endif

#include <Element/ElementTag.h>
#include <Material/MaterialTag.h>
#include <Toolbox/ClassTag.h>
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
