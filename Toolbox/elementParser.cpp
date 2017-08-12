#include "elementParser.h"
#include <Element/Membrane/CP4.h>

using std::vector;

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