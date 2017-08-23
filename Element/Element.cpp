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

Element::~Element() { suanpan_debug("Element %u dtor() Called.\n", get_tag()); }

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
        if(!D->get_node(static_cast<unsigned>(I))->is_active()) {
            printf("Element %u finds a disabled Node %u, now disable it.\n", get_tag(),
                static_cast<unsigned>(I));
            D->disable_element(get_tag());
            return;
        }

    // ADJUST DOF NUMBER
    for(const auto& I : node_encoding) {
        auto& tmp_node = D->get_node(static_cast<unsigned>(I));
        if(tmp_node->get_dof_number() < num_dof) tmp_node->set_dof_number(num_dof);
        node_ptr.push_back(tmp_node);
    }
}

void Element::update_dof_encoding()
{
    auto I = 0;
    for(const auto& J : node_ptr) {
        auto K = J.lock();
        auto node_dof = K->get_reordered_dof();
        if(node_dof.is_empty()) node_dof = K->get_original_dof();
        for(unsigned L = 0; L < num_dof; ++L) dof_encoding(I++) = node_dof(L);
    }
}

const unsigned& Element::get_dof_number() const { return num_dof; }

const unsigned& Element::get_node_number() const { return num_node; }

const uvec& Element::get_dof_encoding() const { return dof_encoding; }

const uvec& Element::get_node_encoding() const { return node_encoding; }

const vector<weak_ptr<Node>>& Element::get_node_ptr() const { return node_ptr; }

const vec& Element::get_resistance() const { return resistance; }

const mat& Element::get_mass() const { return mass; }

const mat& Element::get_damping() const { return damping; }

const mat& Element::get_stiffness() const { return stiffness; }

const mat& Element::get_initial_stiffness() const { return initial_stiffness; }

int Element::update_status() { return -1; }

int Element::commit_status() { return -1; }

int Element::clear_status() { return -1; }

int Element::reset_status() { return -1; }
