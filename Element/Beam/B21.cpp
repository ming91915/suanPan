#include "B21.h"
#include "Material/Section/Section.h"

const unsigned B21::b_node = 2;
const unsigned B21::b_dof = 3;

B21::B21(const unsigned& T, const uvec& N, const uvec& S, const bool& F)
    : Element(T, ET_B21, b_node, b_dof, N, {}, F) {}

void B21::initialize(const shared_ptr<DomainBase>& D) {}

int B21::update_status() { return 0; }

int B21::commit_status() {
    auto code = 0;
    for(const auto& I : b_section) code += I->commit_status();
    return code;
}

int B21::clear_status() {
    auto code = 0;
    for(const auto& I : b_section) code += I->clear_status();
    return code;
}

int B21::reset_status() {
    auto code = 0;
    for(const auto& I : b_section) code += I->reset_status();
    return code;
}

void B21::print() {}
