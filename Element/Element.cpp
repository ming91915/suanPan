#include "Element.h"

Element::Element(const unsigned& T,
    const unsigned& CT,
    const unsigned& NN,
    const unsigned& ND,
    const uvec& NT,
    const uvec& MT,
    const bool& F)
    : Tag(T, CT)
    , num_node(NN)
    , num_dof(ND)
    , node_encoding(NT)
    , material_tag(MT)
    , nlgeom(F)
{
}

Element::~Element()
{
#ifdef SUANPAN_DEBUG
    printf("Element %u dtor() Called.\n", getTag());
#endif
}

void Element::initialize(const shared_ptr<Domain>& D)
{
    auto total_dof = num_node * num_dof;

    if(total_dof != 0) {
        dof_encoding.zeros(total_dof);
        resistance.zeros(total_dof);
        mass.zeros(total_dof, total_dof);
        damping.zeros(total_dof, total_dof);
        stiffness.zeros(total_dof, total_dof);
        initial_stiffness.zeros(total_dof, total_dof);
    }

    node_ptr.clear();
    dof_encoding.zeros();

    // CHECK NODE VALIDITY
    for(const auto& I : node_encoding)
        if(!D->getNode(static_cast<unsigned>(I))->getStatus()) {
            printf("Element %u finds a disabled Node %u, now disable it.\n", getTag(),
                static_cast<unsigned>(I));
            D->disable_element(getTag());
            return;
        }

    // ADJUST DOF NUMBER
    for(const auto& I : node_encoding) {
        auto& tmp_node = D->getNode(static_cast<unsigned>(I));
        if(tmp_node->getNumberDOF() < num_dof) tmp_node->setNumberDOF(num_dof);
        node_ptr.push_back(tmp_node);
    }
}

void Element::updateEncodingDOF()
{
    auto I = 0;
    for(const auto& J : node_ptr) {
        auto K = J.lock();
        auto node_dof = K->getReorderDOF();
        if(node_dof.is_empty()) node_dof = K->getOriginalDOF();
        for(unsigned L = 0; L < num_dof; ++L) dof_encoding(I++) = node_dof(L);
    }
}

const unsigned& Element::getNumberDOF() const { return num_dof; }

const unsigned& Element::getNumberNode() const { return num_node; }

const uvec& Element::getEncodingDOF() const { return dof_encoding; }

const uvec& Element::getEncodingNode() const { return node_encoding; }

const vector<weak_ptr<Node>>& Element::getNodePtr() const { return node_ptr; }

const vec& Element::getResistance() const { return resistance; }

const mat& Element::getMass() const { return mass; }

const mat& Element::getDamping() const { return damping; }

const mat& Element::getStiffness() const { return stiffness; }

const mat& Element::getInitialStiffness() const { return initial_stiffness; }

int Element::updateStatus() { return -1; }

int Element::commitStatus() { return -1; }

int Element::clearStatus() { return -1; }

int Element::resetStatus() { return -1; }
