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

#include "SectionParser.h"
#include <Section/Section>
#include <Toolbox/utility.h>

void new_rectangle(unique_ptr<Section>& return_obj, istringstream& command) {
    unsigned tag;
    if(!get_input(command, tag)) {
        suanpan_error("new_rectangle() requires a valid tag.\n");
        return;
    }

    double width;
    if(!get_input(command, width)) {
        suanpan_error("new_rectangle() requires a valid width.\n");
        return;
    }

    double height;
    if(!get_input(command, height)) {
        suanpan_error("new_rectangle() requires a valid height.\n");
        return;
    }

    unsigned material_id;
    if(!get_input(command, material_id)) {
        suanpan_error("new_rectangle() requires a material tag.\n");
        return;
    }

    unsigned int_pt;
    if(!get_input(command, int_pt)) {
        suanpan_error("new_rectangle() requires a number of integration points.\n");
        return;
    }

    return_obj = make_unique<Rectangle>(tag, width, height, material_id, int_pt);
}
