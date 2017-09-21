/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF5. The full HDF5 copyright notice, including      *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file, which can be found at the root of the source code       *
 * distribution tree, or in https://support.hdfgroup.org/ftp/HDF5/releases.  *
 * If you do not have access to either file, you may request a copy from     *
 * help@hdfgroup.org.                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
 * Programmer:  Raymond Lu <songyulu@hdfgroup.org>
 *              13 February 2013
 */
#ifndef _H5PLextern_H
#define _H5PLextern_H

/* Include HDF5 header */
#include "hdf5.h"

/* plugins always export */
#if defined(_MSC_VER) /* MSVC Compiler Case */
#define H5PLUGIN_DLL __declspec(dllexport)
#elif(__GNUC__ >= 4) /* GCC 4.x has support for visibility options */
#define H5PLUGIN_DLL __attribute__((visibility("default")))
#else
#define H5PLUGIN_DLL
#endif

#ifdef __cplusplus
extern "C" {
#endif

H5PLUGIN_DLL H5PL_type_t H5PLget_plugin_type(void);
H5PLUGIN_DLL const void* H5PLget_plugin_info(void);

#ifdef __cplusplus
}
#endif

#endif /* _H5PLextern_H */
