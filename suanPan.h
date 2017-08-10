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

#if defined(__x86_64__) || defined(_WIN64)
#define __ARCH__ 64
#else
#define __ARCH__ 32
#endif

#ifdef __GNUC__
#endif

// auto invalid_code = static_cast<unsigned>(-1);

#include <Toolbox/ClassTag.h>
#include <Toolbox/ElementTag.h>
#include <Toolbox/MaterialTag.h>

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
