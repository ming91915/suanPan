/**
* @class ExternalModule
* @brief A ExternalModule class handles communication between the main program and
* external library.
*
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

#include "Element/Element.h"
#include <windows.h>

using std::string;
using std::istringstream;

class ExternalModule
{
    string library_name;
    string module_name;

    HINSTANCE ext_library = nullptr;
    void* ext_creator = nullptr;

public:
    explicit ExternalModule(const string&);
    explicit ExternalModule(const string&, const string&);

    bool locate_module();

    void new_object(unique_ptr<Element>&, istringstream&) const;
    void new_object(unique_ptr<Material>&, istringstream&) const;
};

#endif
