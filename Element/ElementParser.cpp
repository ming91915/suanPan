#include "ElementParser.h"
#include <Element/Element>

using std::vector;

void new_cp3_(unique_ptr<Element>& return_obj, istringstream& command)
{
    unsigned tag;
    if((command >> tag).fail()) {
        suanpan_debug("new_cp3_() needs a tag.\n");
        return;
    }

    unsigned node;
    vector<uword> node_tag;
    for(auto I = 0; I < 3; ++I) {
        if((command >> node).fail()) {
            suanpan_debug("new_cp3_() needs three valid nodes.\n");
            return;
        }
        node_tag.push_back(node);
    }

    unsigned material_tag;
    if((command >> material_tag).fail()) {
        suanpan_debug("new_cp3_() needs a valid material tag.\n");
        return;
    }

    auto thickness = 1.;
    if(!command.eof()) {
        if((command >> thickness).fail())
            suanpan_debug("new_cp3_() needs a valid thickness.\n");
    } else
        suanpan_debug("new_cp3_() assumes thickness to be unit.\n");

    return_obj = make_unique<CP3>(tag, uvec(node_tag), material_tag, thickness);
}

void new_cp4_(unique_ptr<Element>& return_obj, istringstream& command)
{
    unsigned tag;
    if((command >> tag).fail()) {
        suanpan_debug("new_cp4_() needs a tag.\n");
        return;
    }

    unsigned node;
    vector<uword> node_tag;
    for(auto I = 0; I < 4; ++I) {
        if((command >> node).fail()) {
            suanpan_debug("new_cp4_() needs four valid nodes.\n");
            return;
        }
        node_tag.push_back(node);
    }

    unsigned material_tag;
    if((command >> material_tag).fail()) {
        suanpan_debug("new_cp4_() needs a valid material tag.\n");
        return;
    }

    auto thickness = 1.;
    if(!command.eof()) {
        if((command >> thickness).fail())
            suanpan_debug("new_cp4_() needs a valid thickness.\n");
    } else
        suanpan_debug("new_cp4_() assumes thickness to be unit.\n");

    unsigned reduced_scheme = 0;
    if(!command.eof()) {
        if((command >> reduced_scheme).fail())
            suanpan_debug("new_cp4_() needs a valid reduced integration switch (0,1).\n");
    } else
        suanpan_debug("new_cp4_() assumes standard integration scheme (2*2).\n");

    unsigned nonlinear = 0;
    if(!command.eof()) {
        if((command >> nonlinear).fail())
            suanpan_debug("new_cp4_() needs a valid nonlinear geometry switch (0,1).\n");
    } else
        suanpan_debug("new_cp4_() assumes linear geometry.\n");

    return_obj = make_unique<CP4>(
        tag, uvec(node_tag), material_tag, thickness, !!reduced_scheme, !!nonlinear);
}

void new_gq12_(unique_ptr<Element>& return_obj, istringstream& command)
{
    unsigned tag;
    if((command >> tag).fail()) {
        suanpan_debug("new_gq12_() needs a valid tag.\n");
        return;
    }

    unsigned node;
    vector<uword> node_tag;
    for(auto I = 0; I < 4; ++I) {
        if((command >> node).fail()) {
            suanpan_debug("new_gq12_() needs four valid nodes.\n");
            return;
        }
        node_tag.push_back(node);
    }

    unsigned material_tag;
    if((command >> material_tag).fail()) {
        suanpan_debug("new_gq12_() needs a valid material tag.\n");
        return;
    }

    auto thickness = 1.;
    if(!command.eof()) {
        if((command >> thickness).fail())
            suanpan_debug("new_gq12_() needs a valid thickness.\n");
    } else
        suanpan_debug("new_gq12_() assumes thickness to be unit.\n");

    return_obj = make_unique<GQ12>(tag, uvec(node_tag), material_tag, thickness);
}

void new_ps_(unique_ptr<Element>& return_obj, istringstream& command)
{
    unsigned tag;
    if((command >> tag).fail()) {
        suanpan_debug("new_ps_() needs a valid tag.\n");
        return;
    }

    unsigned node;
    vector<uword> node_tag;
    for(auto I = 0; I < 4; ++I) {
        if((command >> node).fail()) {
            suanpan_debug("new_ps_() needs four valid nodes.\n");
            return;
        }
        node_tag.push_back(node);
    }

    unsigned material_tag;
    if((command >> material_tag).fail()) {
        suanpan_debug("new_ps_() needs a valid material tag.\n");
        return;
    }

    auto thickness = 1.;
    if(!command.eof()) {
        if((command >> thickness).fail())
            suanpan_debug("new_ps_() needs a valid thickness.\n");
    } else
        suanpan_debug("new_ps_() assumes thickness to be unit.\n");

    return_obj = make_unique<PS>(tag, uvec(node_tag), material_tag, thickness);
}

void new_qe2_(unique_ptr<Element>& return_obj, istringstream& command)
{
    unsigned tag;
    if((command >> tag).fail()) {
        suanpan_debug("new_qe2_() needs a valid tag.\n");
        return;
    }

    unsigned node;
    vector<uword> node_tag;
    for(auto I = 0; I < 4; ++I) {
        if((command >> node).fail()) {
            suanpan_debug("new_qe2_() needs four valid nodes.\n");
            return;
        }
        node_tag.push_back(node);
    }

    unsigned material_tag;
    if((command >> material_tag).fail()) {
        suanpan_debug("new_qe2_() needs a valid material tag.\n");
        return;
    }

    auto thickness = 1.;
    if(!command.eof()) {
        if((command >> thickness).fail())
            suanpan_debug("new_qe2_() needs a valid thickness.\n");
    } else
        suanpan_debug("new_qe2_() assumes thickness to be unit.\n");

    return_obj = make_unique<QE2>(tag, uvec(node_tag), material_tag, thickness);
}

void new_truss2d_(unique_ptr<Element>& return_obj, istringstream& command)
{
    unsigned tag;
    if((command >> tag).fail()) {
        suanpan_debug("new_truss2d_() needs a valid tag.\n");
        return;
    }

    unsigned node;
    vector<uword> node_tag;
    for(auto I = 0; I < 2; ++I) {
        if((command >> node).fail()) {
            suanpan_debug("new_truss2d_() needs two valid nodes.\n");
            return;
        }
        node_tag.push_back(node);
    }

    unsigned material_tag;
    if((command >> material_tag).fail()) {
        suanpan_debug("new_truss2d_() needs a valid material tag.\n");
        return;
    }

    double area;
    if((command >> area).fail()) {
        suanpan_debug("new_truss2d_() needs a valid area.\n");
        return;
    }

    unsigned nonlinear = 0, update_area = 0, log_strain = 0;

    if(!command.eof()) {
        if((command >> nonlinear).fail())
            suanpan_debug("new_qe2_() needs a valid nonlinear geometry switch (0,1).\n");
    } else
        suanpan_debug("new_qe2_() assumes linear geometry.\n");

    if(!command.eof()) {
        if((command >> update_area).fail())
            suanpan_debug(
                "new_qe2_() needs a valid switch (0,1) to indicate if update area.\n");
    } else
        suanpan_debug("new_qe2_() assumes constant area.\n");

    if(!command.eof()) {
        if((command >> log_strain).fail())
            suanpan_debug("new_qe2_() needs a valid switch (0,1) to indicate if use "
                          "engineering strain.\n");
    } else
        suanpan_debug("new_qe2_() assumes engineering strain.\n");

    return_obj = make_unique<Truss2D>(
        tag, uvec(node_tag), material_tag, !!nonlinear, !!update_area, !!log_strain);
}

void new_c3d8_(unique_ptr<Element>& return_obj, istringstream& command)
{
    unsigned tag;
    if((command >> tag).fail()) {
        suanpan_debug("new_c3d8_() needs a valid tag.\n");
        return;
    }

    unsigned node;
    vector<uword> node_tag;
    for(auto I = 0; I < 8; ++I) {
        if((command >> node).fail()) {
            suanpan_debug("new_c3d8_() needs two valid nodes.\n");
            return;
        }
        node_tag.push_back(node);
    }

    unsigned material_tag;
    if((command >> material_tag).fail()) {
        suanpan_debug("new_c3d8_() needs a valid material tag.\n");
        return;
    }

    unsigned reduced_scheme = 0;
    if(!command.eof()) {
        if((command >> reduced_scheme).fail())
            suanpan_debug(
                "new_c3d8_() needs a valid reduced integration switch (0,1).\n");
    } else
        suanpan_debug("new_c3d8_() assumes standard integration scheme (2*2).\n");

    unsigned nonlinear = 0;
    if(!command.eof()) {
        if((command >> nonlinear).fail())
            suanpan_debug("new_c3d8_() needs a valid nonlinear geomtery switch (0,1).\n");
    } else
        suanpan_debug("new_c3d8_() assumes linear geometry.\n");

    return_obj = make_unique<C3D8>(
        tag, uvec(node_tag), material_tag, !!reduced_scheme, !!nonlinear);
}
