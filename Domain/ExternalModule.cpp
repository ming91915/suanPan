////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2017 Theodore Chang
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////

#include "ExternalModule.h"
#include <algorithm>
#if defined(SUANPAN_WIN)
#include <windows.h>
#elif defined(SUANPAN_UNIX)
#include <dlfcn.h>
#endif

using element_creator = void (*)(unique_ptr<Element>&, istringstream&);
using material_creator = void (*)(unique_ptr<Material>&, istringstream&);

ExternalModule::ExternalModule(const string& L)
    : library_name(L) {
#ifdef SUANPAN_WIN
    auto file_name = library_name + ".dll";
    auto gnu_name = "lib" + file_name;

    ext_library = LoadLibraryA(file_name.c_str());
    if(ext_library == nullptr) {
        transform(file_name.begin(), file_name.end(), file_name.begin(), tolower);
        ext_library = LoadLibraryA(file_name.c_str());
    }
    if(ext_library == nullptr) {
        transform(file_name.begin(), file_name.end(), file_name.begin(), toupper);
        ext_library = LoadLibraryA(file_name.c_str());
    }
    if(ext_library == nullptr) { ext_library = LoadLibraryA(gnu_name.c_str()); }
    if(ext_library == nullptr) {
        transform(gnu_name.begin(), gnu_name.end(), gnu_name.begin(), tolower);
        ext_library = LoadLibraryA(gnu_name.c_str());
    }
    if(ext_library == nullptr) {
        transform(gnu_name.begin(), gnu_name.end(), gnu_name.begin(), toupper);
        ext_library = LoadLibraryA(gnu_name.c_str());
    }
    if(ext_library == nullptr) suanpan_error("locate_module() cannot find the library with the given name %s.\n", file_name.c_str());
#elif defined(SUANPAN_UNIX)
    auto file_name = "./lib" + library_name + ".so";
    ext_library = dlopen(file_name.c_str(), RTLD_NOW);
    if(ext_library == nullptr) {
        file_name = "./" + library_name + ".so";
        ext_library = dlopen(file_name.c_str(), RTLD_NOW);
    }
    if(ext_library == nullptr) suanpan_error("locate_module() cannot find the library with the given name %s.\n", file_name.c_str());
#endif
}

ExternalModule::~ExternalModule() {
#ifdef SUANPAN_WIN
    if(ext_library != nullptr) FreeLibrary(HINSTANCE(ext_library));
#elif defined(SUANPAN_UNIX)
    if(ext_library != nullptr) dlclose(ext_library);
#endif
}

bool ExternalModule::locate_module(string module_name) {
    if(ext_library == nullptr) return false;

    transform(module_name.begin(), module_name.end(), module_name.begin(), tolower);
    module_name = "new_" + module_name;

#ifdef SUANPAN_WIN
    ext_creator = reinterpret_cast<void*>(GetProcAddress(HINSTANCE(ext_library), LPCSTR(module_name.c_str())));
#elif defined(SUANPAN_UNIX)
    ext_creator = dlsym(ext_library, module_name.c_str());
#endif

    if(ext_creator == nullptr) return false;

    return true;
}

void ExternalModule::new_object(unique_ptr<Element>& return_obj, istringstream& command) const { (element_creator(ext_creator))(return_obj, command); }

void ExternalModule::new_object(unique_ptr<Material>& return_obj, istringstream& command) const { (material_creator(ext_creator))(return_obj, command); }
