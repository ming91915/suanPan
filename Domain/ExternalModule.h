/**
* @class ExternalModule
* @brief A ExternalModule class handles communication between the main program and external library.
*
* @author T
* @date 14/08/2017
* @version 0.1.0
* @file ExternalModule.h
* @addtogroup Utility
* @{
*/

#ifndef EXTERNALMODULE_H
#define EXTERNALMODULE_H

#include <suanPan.h>
#if defined(SUANPAN_WIN)
#include <windows.h>
#elif defined(SUANPAN_UNIX)
#include <dlfcn.h>
#endif
#include "Element/Element.h"

using std::string;
using std::istringstream;

class ExternalModule {
    string library_name;
    string module_name;

    void* ext_library = nullptr;
    void* ext_creator = nullptr;

public:
    explicit ExternalModule(const string&);
    explicit ExternalModule(const string&, const string&);
    ExternalModule(const ExternalModule&) = delete;
    ExternalModule(ExternalModule&&) = delete;
    ExternalModule& operator=(const ExternalModule&) = delete;
    ExternalModule& operator=(ExternalModule&&) = delete;
    ~ExternalModule();

    bool locate_module();

    void new_object(unique_ptr<Element>&, istringstream&) const;
    void new_object(unique_ptr<Material>&, istringstream&) const;
};

#endif
