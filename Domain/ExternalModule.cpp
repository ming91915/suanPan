#include "ExternalModule.h"
#include <algorithm>

using element_creator = void (*)(unique_ptr<Element>&, istringstream&);
using material_creator = void (*)(unique_ptr<Material>&, istringstream&);

ExternalModule::ExternalModule(const string& N)
    : library_name(N)
    , module_name(N)
{
}

ExternalModule::ExternalModule(const string& L, const string& M)
    : library_name(L)
    , module_name(M)
{
}

bool ExternalModule::locate_module()
{
    library_name += ".dll";
    ext_library = LoadLibraryA(library_name.c_str());
    if(!ext_library) {
        transform(
            library_name.begin(), library_name.end(), library_name.begin(), tolower);
        ext_library = LoadLibraryA(library_name.c_str());
    }
    if(!ext_library) {
        transform(
            library_name.begin(), library_name.end(), library_name.begin(), toupper);
        ext_library = LoadLibraryA(library_name.c_str());
    }
    if(!ext_library) {
        suanpan_error("locate_module() cannot find the library with the given name.\n");
        return false;
    }

    transform(module_name.begin(), module_name.end(), module_name.begin(), tolower);
    module_name = "new_" + module_name + "_";

    ext_creator = GetProcAddress(ext_library, LPCSTR(module_name.c_str()));

    if(!ext_creator) {
        suanpan_error("locate_module() cannot find the function with the given name.\n");
        return false;
    }

    return true;
}

void ExternalModule::new_object(unique_ptr<Element>& return_obj,
    istringstream& command) const
{
    static_cast<element_creator>(ext_creator)(return_obj, command);
}

void ExternalModule::new_object(unique_ptr<Material>& return_obj,
    istringstream& command) const
{
    static_cast<material_creator>(ext_creator)(return_obj, command);
}
