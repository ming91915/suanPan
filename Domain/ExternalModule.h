/*******************************************************************************
 * Copyright (C) 2017 Theodore Chang
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/
/**
 * @class ExternalModule
 * @brief A ExternalModule class handles communication between the main program and external library.
 *
 * @author T
 * @date 28/09/2017
 * @version 0.1.1
 * @file ExternalModule.h
 * @addtogroup Utility
 * @{
 */

#ifndef EXTERNALMODULE_H
#define EXTERNALMODULE_H

#include <suanPan.h>

class Element;
class Material;
class Section;

class ExternalModule {
    void* ext_library = nullptr;
    void* ext_creator = nullptr;

public:
    const string library_name;

    explicit ExternalModule(const string&);
    ExternalModule(const ExternalModule&) = delete;
    ExternalModule(ExternalModule&&) = delete;
    ExternalModule& operator=(const ExternalModule&) = delete;
    ExternalModule& operator=(ExternalModule&&) = delete;
    ~ExternalModule();

    bool locate_module(string);

    void new_object(unique_ptr<Element>&, istringstream&) const;
    void new_object(unique_ptr<Material>&, istringstream&) const;
    void new_object(unique_ptr<Section>&, istringstream&) const;
};

#endif
