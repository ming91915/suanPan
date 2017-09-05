#include "C3D8.h"

const unsigned C3D8::c_node = 8;
const unsigned C3D8::c_dof = 3;

C3D8::C3D8(const unsigned& T, const uvec& N, const unsigned& M, const bool& R, const bool& F)
    : Element(T, ET_C3D8, c_node, c_dof, N, uvec{ M }, F)
    , reduced_scheme(R) {}

void C3D8::initialize(const shared_ptr<Domain>&) {}

int C3D8::update_status() { return 0; }

int C3D8::commit_status() {
    auto code = 0;
    for(const auto& I : int_pt) code += I->c_material->commit_status();
    return code;
}

int C3D8::clear_status() {
    auto code = 0;
    for(const auto& I : int_pt) code += I->c_material->clear_status();
    return code;
}

int C3D8::reset_status() {
    auto code = 0;
    for(const auto& I : int_pt) code += I->c_material->reset_status();
    return code;
}
