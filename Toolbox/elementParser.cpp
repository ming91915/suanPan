#include "elementParser.h"
#include <Element/Element>

using std::vector;

void new_cp3_(unique_ptr<Element>& return_obj, istringstream& command)
{
    unsigned tag;
    command >> tag;
    unsigned node;
    vector<uword> node_tag;
    for(auto I = 0; I < 3; ++I) {
        command >> node;
        node_tag.push_back(node);
    }
    unsigned material_tag;
    command >> material_tag;
    auto thickness = 1.;
    if(command.good())
        command >> thickness;
    else
        suanpan_debug("new_cp3_() assumes thickness to be unit.\n");

    return_obj = make_unique<CP3>(tag, uvec(node_tag), material_tag, thickness);
}

void new_cp4_(unique_ptr<Element>& return_obj, istringstream& command)
{
    unsigned tag;
    command >> tag;
    unsigned node;
    vector<uword> node_tag;
    for(auto I = 0; I < 4; ++I) {
        command >> node;
        node_tag.push_back(node);
    }
    unsigned material_tag;
    command >> material_tag;
    auto thickness = 1.;
    if(command.good()) command >> thickness;
    unsigned reduced_scheme = 0;
    if(command.good()) command >> reduced_scheme;
    unsigned nonlinear = 0;
    if(command.good()) command >> nonlinear;

    return_obj = make_unique<CP4>(tag, uvec(node_tag), material_tag, thickness,
        static_cast<bool>(reduced_scheme), static_cast<bool>(nonlinear));
}

void new_gq12_(unique_ptr<Element>& return_obj, istringstream& command)
{
    unsigned tag;
    command >> tag;
    unsigned node;
    vector<uword> node_tag;
    for(auto I = 0; I < 4; ++I) {
        command >> node;
        node_tag.push_back(node);
    }
    unsigned material_tag;
    command >> material_tag;
    auto thickness = 1.;
    if(command.good())
        command >> thickness;
    else
        suanpan_debug("new_gq12_() assumes thickness to be unit.\n");

    return_obj = make_unique<GQ12>(tag, uvec(node_tag), material_tag, thickness);
}

void new_ps_(unique_ptr<Element>& return_obj, istringstream& command)
{
    unsigned tag;
    command >> tag;
    unsigned node;
    vector<uword> node_tag;
    for(auto I = 0; I < 4; ++I) {
        command >> node;
        node_tag.push_back(node);
    }
    unsigned material_tag;
    command >> material_tag;
    auto thickness = 1.;
    if(command.good())
        command >> thickness;
    else
        suanpan_debug("new_ps_() assumes thickness to be unit.\n");

    return_obj = make_unique<PS>(tag, uvec(node_tag), material_tag, thickness);
}

void new_qe2_(unique_ptr<Element>& return_obj, istringstream& command)
{
    unsigned tag;
    command >> tag;

    unsigned node;
    vector<uword> node_tag;

    for(auto I = 0; I < 4; ++I) {
        command >> node;
        node_tag.push_back(node);
    }

    unsigned material_tag;
    command >> material_tag;

    auto thickness = 1.;
    if(command.good())
        command >> thickness;
    else
        suanpan_debug("new_qe2_() assumes thickness to be unit.\n");

    return_obj = make_unique<QE2>(tag, uvec(node_tag), material_tag, thickness);
}

void new_truss2d_(unique_ptr<Element>& return_obj, istringstream& command)
{
    unsigned tag;
    command >> tag;

    unsigned node;
    vector<uword> node_tag;
    for(auto I = 0; I < 2; ++I) {
        command >> node;
        node_tag.push_back(node);
    }

    unsigned material_tag;
    command >> material_tag;

    double area;
    command >> area;

    unsigned nonlinear = 0, update_area = 0, log_strain = 0;

    if(command.good()) {
        command >> nonlinear;
        if(command.good()) {
            command >> update_area;
            if(command.good())
                command >> log_strain;
            else
                suanpan_debug("new_truss2d_ assumes engineering strain.\n");
        } else
            suanpan_debug("new_truss2d_ assumes constant area.\n");
    } else
        suanpan_debug("new_truss2d_ uses linear geometry.\n");

    return_obj = make_unique<Truss2D>(tag, uvec(node_tag), material_tag,
        static_cast<bool>(nonlinear), static_cast<bool>(update_area),
        static_cast<bool>(log_strain));
}

void new_c3d8_(unique_ptr<Element>& return_obj, istringstream& command)
{
    unsigned tag;
    command >> tag;

    unsigned node;
    vector<uword> node_tag;
    for(auto I = 0; I < 8; ++I) {
        command >> node;
        node_tag.push_back(node);
    }

    unsigned material_tag;
    command >> material_tag;

    unsigned reduced_scheme = 0;
    if(command.good()) command >> reduced_scheme;

    unsigned nonlinear = 0;
    if(command.good()) command >> nonlinear;

    return_obj = make_unique<C3D8>(tag, uvec(node_tag), material_tag,
        static_cast<bool>(reduced_scheme), static_cast<bool>(nonlinear));
}
