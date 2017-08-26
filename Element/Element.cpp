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
    suanpan_debug("Element %u ctor() called.\n", T);
}

Element::~Element() { suanpan_debug("Element %u dtor() called.\n", get_tag()); }

void Element::initialize(const shared_ptr<Domain>& D)
{
    const auto total_dof = num_node * num_dof;

    if(total_dof == 0) return;

    dof_encoding.zeros(total_dof);
    resistance.zeros(total_dof);
    mass.zeros(total_dof, total_dof);
    damping.zeros(total_dof, total_dof);
    stiffness.zeros(total_dof, total_dof);
    initial_stiffness.zeros(total_dof, total_dof);

    // CHECK NODE VALIDITY
    node_ptr.clear();
    for(const auto& tmp_tag : node_encoding) {
        const auto& tmp_node = get_node(D, static_cast<unsigned>(tmp_tag));
        if(tmp_node == nullptr || !tmp_node->is_active()) {
            suanpan_debug("Element %u finds an invalid Node %u, now diable it.\n",
                get_tag(), tmp_tag);
            D->disable_element(get_tag());
            return;
        }
        if(tmp_node->get_dof_number() < num_dof) tmp_node->set_dof_number(num_dof);
        node_ptr.push_back(tmp_node);
    }

    // CHECK MATERIAL PROTOTYPE VALIDITY
    for(const auto& tmp_materail : material_tag)
        if(!D->find_material(unsigned(tmp_materail))) {
            suanpan_debug("Element %u cannot find valid Material %u, now diable it.\n",
                get_tag(), unsigned(tmp_materail));
            D->disable_element(get_tag());
            return;
        }
}

void Element::update_dof_encoding()
{
    auto idx = 0;
    for(const auto& tmp_ptr : node_ptr) {
        auto& node_dof = tmp_ptr.lock()->get_reordered_dof();
        for(unsigned i = 0; i < num_dof; ++i) dof_encoding(idx++) = node_dof(i);
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

vector<vec> Element::record(const OutputList&) { return {}; }
