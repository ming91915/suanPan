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

#include "ElementParser.h"
#include <Element/Element>
#include <Toolbox/utility.h>

using std::vector;

void new_cp3(unique_ptr<Element>& return_obj, istringstream& command) {
    unsigned tag;
    if((command >> tag).fail()) {
        suanpan_debug("new_cp3() needs a tag.\n");
        return;
    }

    unsigned node;
    vector<uword> node_tag;
    for(auto I = 0; I < 3; ++I) {
        if((command >> node).fail()) {
            suanpan_debug("new_cp3() needs three valid nodes.\n");
            return;
        }
        node_tag.push_back(node);
    }

    unsigned material_tag;
    if((command >> material_tag).fail()) {
        suanpan_debug("new_cp3() needs a valid material tag.\n");
        return;
    }

    auto thickness = 1.;
    if(!command.eof()) {
        if((command >> thickness).fail()) suanpan_debug("new_cp3() needs a valid thickness.\n");
    } else
        suanpan_debug("new_cp3() assumes thickness to be unit.\n");

    return_obj = make_unique<CP3>(tag, uvec(node_tag), material_tag, thickness);
}

void new_cp4(unique_ptr<Element>& return_obj, istringstream& command) {
    unsigned tag;
    if((command >> tag).fail()) {
        suanpan_debug("new_cp4() needs a tag.\n");
        return;
    }

    unsigned node;
    vector<uword> node_tag;
    for(auto I = 0; I < 4; ++I) {
        if((command >> node).fail()) {
            suanpan_debug("new_cp4() needs four valid nodes.\n");
            return;
        }
        node_tag.push_back(node);
    }

    unsigned material_tag;
    if((command >> material_tag).fail()) {
        suanpan_debug("new_cp4() needs a valid material tag.\n");
        return;
    }

    auto thickness = 1.;
    if(!command.eof()) {
        if((command >> thickness).fail()) suanpan_debug("new_cp4() needs a valid thickness.\n");
    } else
        suanpan_debug("new_cp4() assumes thickness to be unit.\n");

    unsigned reduced_scheme = 0;
    if(!command.eof()) {
        if((command >> reduced_scheme).fail()) suanpan_debug("new_cp4() needs a valid reduced integration switch (0,1).\n");
    } else
        suanpan_debug("new_cp4() assumes standard integration scheme (2*2).\n");

    unsigned nonlinear = 0;
    if(!command.eof()) {
        if((command >> nonlinear).fail()) suanpan_debug("new_cp4() needs a valid nonlinear geometry switch (0,1).\n");
    } else
        suanpan_debug("new_cp4() assumes linear geometry.\n");

    return_obj = make_unique<CP4>(tag, uvec(node_tag), material_tag, thickness, !!reduced_scheme, !!nonlinear);
}

void new_cp8(unique_ptr<Element>& return_obj, istringstream& command) {
    unsigned tag;
    if((command >> tag).fail()) {
        suanpan_debug("new_cp8() needs a tag.\n");
        return;
    }

    uword node;
    vector<uword> node_tag;
    for(auto I = 0; I < 8; ++I) {
        if((command >> node).fail()) {
            suanpan_debug("new_cp8() needs eight valid nodes.\n");
            return;
        }
        node_tag.push_back(node);
    }

    unsigned material_tag;
    if((command >> material_tag).fail()) {
        suanpan_debug("new_cp8() needs a valid material tag.\n");
        return;
    }

    auto thickness = 1.;
    if(!command.eof() && (command >> thickness).fail()) {
        suanpan_debug("new_cp8() needs a valid thickness.\n");
        return;
    }

    unsigned reduced_scheme = 0;
    if(!command.eof() && (command >> reduced_scheme).fail()) {
        suanpan_debug("new_cp8() needs a valid reduced integration switch (0,1).\n");
        return;
    }

    unsigned nonlinear = 0;
    if(!command.eof() && (command >> nonlinear).fail()) {
        suanpan_debug("new_cp8() needs a valid nonlinear geometry switch (0,1).\n");
        return;
    }

    return_obj = make_unique<CP8>(tag, uvec(node_tag), material_tag, thickness, !!reduced_scheme, !!nonlinear);
}

void new_gq12(unique_ptr<Element>& return_obj, istringstream& command) {
    unsigned tag;
    if((command >> tag).fail()) {
        suanpan_debug("new_gq12() needs a valid tag.\n");
        return;
    }

    unsigned node;
    vector<uword> node_tag;
    for(auto I = 0; I < 4; ++I) {
        if((command >> node).fail()) {
            suanpan_debug("new_gq12() needs four valid nodes.\n");
            return;
        }
        node_tag.push_back(node);
    }

    unsigned material_tag;
    if((command >> material_tag).fail()) {
        suanpan_debug("new_gq12() needs a valid material tag.\n");
        return;
    }

    auto thickness = 1.;
    if(!command.eof()) {
        if((command >> thickness).fail()) suanpan_debug("new_gq12() needs a valid thickness.\n");
    } else
        suanpan_debug("new_gq12() assumes thickness to be unit.\n");

    return_obj = make_unique<GQ12>(tag, uvec(node_tag), material_tag, thickness);
}

void new_ps(unique_ptr<Element>& return_obj, istringstream& command) {
    unsigned tag;
    if((command >> tag).fail()) {
        suanpan_debug("new_ps() needs a valid tag.\n");
        return;
    }

    unsigned node;
    vector<uword> node_tag;
    for(auto I = 0; I < 4; ++I) {
        if((command >> node).fail()) {
            suanpan_debug("new_ps() needs four valid nodes.\n");
            return;
        }
        node_tag.push_back(node);
    }

    unsigned material_tag;
    if((command >> material_tag).fail()) {
        suanpan_debug("new_ps() needs a valid material tag.\n");
        return;
    }

    auto thickness = 1.;
    if(!command.eof()) {
        if((command >> thickness).fail()) suanpan_debug("new_ps() needs a valid thickness.\n");
    } else
        suanpan_debug("new_ps() assumes thickness to be unit.\n");

    return_obj = make_unique<PS>(tag, uvec(node_tag), material_tag, thickness);
}

void new_qe2(unique_ptr<Element>& return_obj, istringstream& command) {
    unsigned tag;
    if((command >> tag).fail()) {
        suanpan_debug("new_qe2() needs a valid tag.\n");
        return;
    }

    unsigned node;
    vector<uword> node_tag;
    for(auto I = 0; I < 4; ++I) {
        if((command >> node).fail()) {
            suanpan_debug("new_qe2() needs four valid nodes.\n");
            return;
        }
        node_tag.push_back(node);
    }

    unsigned material_tag;
    if((command >> material_tag).fail()) {
        suanpan_debug("new_qe2() needs a valid material tag.\n");
        return;
    }

    auto thickness = 1.;
    if(!command.eof()) {
        if((command >> thickness).fail()) suanpan_debug("new_qe2() needs a valid thickness.\n");
    } else
        suanpan_debug("new_qe2() assumes thickness to be unit.\n");

    return_obj = make_unique<QE2>(tag, uvec(node_tag), material_tag, thickness);
}

void new_truss2d(unique_ptr<Element>& return_obj, istringstream& command) {
    unsigned tag;
    if((command >> tag).fail()) {
        suanpan_debug("new_truss2d() needs a valid tag.\n");
        return;
    }

    unsigned node;
    vector<uword> node_tag;
    for(auto I = 0; I < 2; ++I) {
        if((command >> node).fail()) {
            suanpan_debug("new_truss2d() needs two valid nodes.\n");
            return;
        }
        node_tag.push_back(node);
    }

    unsigned material_tag;
    if((command >> material_tag).fail()) {
        suanpan_debug("new_truss2d() needs a valid material tag.\n");
        return;
    }

    double area;
    if((command >> area).fail()) {
        suanpan_debug("new_truss2d() needs a valid area.\n");
        return;
    }

    unsigned nonlinear = 0, update_area = 0, log_strain = 0;

    if(!command.eof()) {
        if((command >> nonlinear).fail()) suanpan_debug("new_truss2d() needs a valid nonlinear geometry switch (0,1).\n");
    } else
        suanpan_debug("new_truss2d() assumes linear geometry.\n");

    if(!command.eof()) {
        if((command >> update_area).fail()) suanpan_debug("new_truss2d() needs a valid switch (0,1) to indicate if update area.\n");
    } else
        suanpan_debug("new_truss2d() assumes constant area.\n");

    if(!command.eof()) {
        if((command >> log_strain).fail()) suanpan_debug("new_truss2d() needs a valid switch (0,1) to indicate if to use engineering strain.\n");
    } else
        suanpan_debug("new_truss2d() assumes engineering strain.\n");

    return_obj = make_unique<Truss2D>(tag, uvec(node_tag), material_tag, area, !!nonlinear, !!update_area, !!log_strain);
}

void new_c3d8(unique_ptr<Element>& return_obj, istringstream& command) {
    unsigned tag;
    if((command >> tag).fail()) {
        suanpan_debug("new_c3d8() needs a valid tag.\n");
        return;
    }

    unsigned node;
    vector<uword> node_tag;
    for(auto I = 0; I < 8; ++I) {
        if((command >> node).fail()) {
            suanpan_debug("new_c3d8() needs two valid nodes.\n");
            return;
        }
        node_tag.push_back(node);
    }

    unsigned material_tag;
    if((command >> material_tag).fail()) {
        suanpan_debug("new_c3d8() needs a valid material tag.\n");
        return;
    }

    unsigned reduced_scheme = 0;
    if(!command.eof()) {
        if((command >> reduced_scheme).fail()) suanpan_debug("new_c3d8() needs a valid reduced integration switch (0,1).\n");
    } else
        suanpan_debug("new_c3d8() assumes standard integration scheme (2*2).\n");

    unsigned nonlinear = 0;
    if(!command.eof()) {
        if((command >> nonlinear).fail()) suanpan_debug("new_c3d8() needs a valid nonlinear geomtery switch (0,1).\n");
    } else
        suanpan_debug("new_c3d8() assumes linear geometry.\n");

    return_obj = make_unique<C3D8>(tag, uvec(node_tag), material_tag, !!reduced_scheme, !!nonlinear);
}

void new_c3d20(unique_ptr<Element>& return_obj, istringstream& command) {
    unsigned tag;
    if((command >> tag).fail()) {
        suanpan_debug("new_c3d20() needs a valid tag.\n");
        return;
    }

    unsigned node;
    vector<uword> node_tag;
    for(auto I = 0; I < 20; ++I) {
        if((command >> node).fail()) {
            suanpan_debug("new_c3d20() needs twenty valid nodes.\n");
            return;
        }
        node_tag.push_back(node);
    }

    unsigned material_tag;
    if((command >> material_tag).fail()) {
        suanpan_debug("new_c3d20() needs a valid material tag.\n");
        return;
    }

    unsigned reduced_scheme = 0;
    if(!command.eof()) {
        if((command >> reduced_scheme).fail()) suanpan_debug("new_c3d20() needs a valid reduced integration switch (0,1).\n");
    } else
        suanpan_debug("new_c3d20() assumes standard integration scheme (3*3).\n");

    unsigned nonlinear = 0;
    if(!command.eof()) {
        if((command >> nonlinear).fail()) suanpan_debug("new_c3d20() needs a valid nonlinear geomtery switch (0,1).\n");
    } else
        suanpan_debug("new_c3d20() assumes linear geometry.\n");

    return_obj = make_unique<C3D20>(tag, uvec(node_tag), material_tag, !!reduced_scheme, !!nonlinear);
}

void new_elasticb21(unique_ptr<Element>& return_obj, istringstream& command) {
    unsigned tag;
    if((command >> tag).fail()) {
        suanpan_debug("new_elasticb21() needs a valid tag.\n");
        return;
    }

    unsigned node;
    vector<uword> node_tag;
    for(auto I = 0; I < 2; ++I) {
        if((command >> node).fail()) {
            suanpan_debug("new_elasticb21() needs two valid nodes.\n");
            return;
        }
        node_tag.push_back(node);
    }

    double area;
    if((command >> area).fail()) {
        suanpan_debug("new_elasticb21() needs a valid area.\n");
        return;
    }

    double moment_inertia;
    if((command >> moment_inertia).fail()) {
        suanpan_debug("new_elasticb21() needs a valid moment of inertia.\n");
        return;
    }

    unsigned material_tag;
    if((command >> material_tag).fail()) {
        suanpan_debug("new_elasticb21() needs a valid material tag.\n");
        return;
    }

    unsigned nonlinear = 0;
    if(!command.eof()) {
        if((command >> nonlinear).fail()) suanpan_debug("new_elasticb21() needs a valid nonlinear geomtery switch (0,1).\n");
    } else
        suanpan_debug("new_elasticb21() assumes linear geometry.\n");

    return_obj = make_unique<ElasticB21>(tag, uvec(node_tag), area, moment_inertia, material_tag, !!nonlinear);
}

void new_b21(unique_ptr<Element>& return_obj, istringstream& command) {
    unsigned tag;
    if(!get_input(command, tag)) {
        suanpan_debug("new_b21() needs a valid tag.\n");
        return;
    }

    unsigned node;
    vector<uword> node_tag;
    for(auto I = 0; I < 2; ++I) {
        if(!get_input(command, node)) {
            suanpan_debug("new_b21() needs two valid nodes.\n");
            return;
        }
        node_tag.push_back(node);
    }

    unsigned section_id;
    if(!get_input(command, section_id)) {
        suanpan_debug("new_b21() needs a valid section tag.\n");
        return;
    }

    unsigned int_pt;
    if(!get_input(command, int_pt)) {
        suanpan_debug("new_b21() needs a valid number of integration points.\n");
        return;
    }

    unsigned nonlinear = 0;
    if(!command.eof()) {
        if(!get_input(command, nonlinear)) suanpan_debug("new_b21() needs a valid nonlinear geomtery switch (0,1).\n");
    } else
        suanpan_debug("new_b21() assumes linear geometry.\n");

    return_obj = make_unique<B21>(tag, uvec(node_tag), section_id, int_pt, !!nonlinear);
}

void new_f21(unique_ptr<Element>& return_obj, istringstream& command) {
    unsigned tag;
    if(!get_input(command, tag)) {
        suanpan_debug("new_f21() needs a valid tag.\n");
        return;
    }

    unsigned node;
    vector<uword> node_tag;
    for(auto I = 0; I < 2; ++I) {
        if(!get_input(command, node)) {
            suanpan_debug("new_f21() needs two valid nodes.\n");
            return;
        }
        node_tag.push_back(node);
    }

    unsigned section_id;
    if(!get_input(command, section_id)) {
        suanpan_debug("new_f21() needs a valid section tag.\n");
        return;
    }

    unsigned int_pt = 6;
    if(!get_input(command, int_pt)) {
        suanpan_debug("new_f21() needs a valid number of integration points.\n");
        return;
    }

    unsigned nonlinear = 0;
    if(!command.eof()) {
        if(!get_input(command, nonlinear)) suanpan_debug("new_f21() needs a valid nonlinear geomtery switch (0,1).\n");
    } else
        suanpan_debug("new_b21() assumes linear geometry.\n");

    return_obj = make_unique<F21>(tag, uvec(node_tag), section_id, int_pt, !!nonlinear);
}

void new_mass(unique_ptr<Element>& return_obj, istringstream& command) {
    unsigned tag;
    if(!get_input(command, tag)) {
        suanpan_debug("new_mass() needs a valid tag.\n");
        return;
    }

    unsigned node;
    if(!get_input(command, node)) {
        suanpan_debug("new_mass() needs one valid node.\n");
        return;
    }

    double magnitude;
    if(!get_input(command, magnitude)) {
        suanpan_debug("new_mass() needs a valid magnitude.\n");
        return;
    }

    unsigned dof;
    vector<uword> dof_tag;
    while(get_input(command, dof)) dof_tag.push_back(dof);

    return_obj = make_unique<Mass>(tag, node, magnitude, uvec(dof_tag));
}

void new_proto01(unique_ptr<Element>& return_obj, istringstream& command) {
    unsigned tag;
    if(!get_input(command, tag)) {
        suanpan_debug("new_proto01() needs a valid tag.\n");
        return;
    }

    unsigned node;
    vector<uword> node_tag;
    for(auto I = 0; I < 4; ++I) {
        if(!get_input(command, node)) {
            suanpan_debug("new_proto01() needs four valid nodes.\n");
            return;
        }
        node_tag.push_back(node);
    }

    unsigned material_tag;
    if(!get_input(command, material_tag)) {
        suanpan_debug("new_proto01() needs a valid material tag.\n");
        return;
    }

    auto thickness = 1.;
    if(!command.eof()) {
        if((command >> thickness).fail()) suanpan_debug("new_proto01() needs a valid thickness.\n");
    } else
        suanpan_debug("new_proto01() assumes thickness to be unit.\n");

    return_obj = make_unique<Proto01>(tag, uvec(node_tag), material_tag, thickness);
}
