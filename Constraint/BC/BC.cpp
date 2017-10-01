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

#include "BC.h"
#include <Domain/DomainBase.h>
#include <Domain/Factory.hpp>
#include <Domain/Node.h>
#include <Toolbox/utility.h>

/**
 * \brief The default constructor.
 * \param T `unique_tag`
 * \param S `step_tag`
 * \param N `nodes`
 * \param D `dofs`
 */
BC::BC(const unsigned& T, const unsigned& S, const uvec& N, const unsigned& D)
    : Constraint(T, CT_BC, S)
    , nodes(N)
    , dofs({ D }) {}

/**
 * \brief No Tag version of the default constructor.
 * \param S `step_tag`
 * \param N `nodes`
 * \param D `dofs`
 */
BC::BC(const unsigned& S, const uvec& N, const unsigned& D)
    : Constraint(0, CT_BC, S)
    , nodes(N)
    , dofs({ D }) {}

/**
 * \brief The constructor uses DoF vector.
 * \param T `unique_tag`
 * \param S `step_tag`
 * \param N `nodes`
 * \param D `dofs`
 */
BC::BC(const unsigned& T, const unsigned& S, const uvec& N, const uvec& D)
    : Constraint(T, CT_BC, S)
    , nodes(N)
    , dofs(D) {}

/**
 * \brief No Tag version.
 * \param S `step_tag`
 * \param N `nodes`
 * \param D `dofs`
 */
BC::BC(const unsigned& S, const uvec& N, const uvec& D)
    : Constraint(0, CT_BC, S)
    , nodes(N)
    , dofs(D) {}

/**
 * \brief The constructor uses predefined TYPE: "XSYMM", "YSYMM",
 * "ZSYMM", "ENCASTRE",
 * "PINNED".
 * \param T `unique_tag`
 * \param S `step_tag`
 * \param N `nodes`
 * \param TP BC TYPE
 */
BC::BC(const unsigned& T, const unsigned& S, const uvec& N, const char* TP)
    : Constraint(T, CT_BC, S)
    , nodes(N) {
    if(is_equal(TP, "XSYMM") || is_equal(TP, "X"))
        dofs = std::initializer_list<uword>({ 1, 5, 6 });
    else if(is_equal(TP, "YSYMM") || is_equal(TP, "Y"))
        dofs = std::initializer_list<uword>({ 2, 4, 6 });
    else if(is_equal(TP, "ZSYMM") || is_equal(TP, "Z"))
        dofs = std::initializer_list<uword>({ 3, 4, 5 });
    else if(is_equal(TP, "ENCASTRE") || is_equal(TP, "E"))
        dofs = std::initializer_list<uword>({ 1, 2, 3, 4, 5, 6 });
    else if(is_equal(TP, "PINNED") || is_equal(TP, "P"))
        dofs = std::initializer_list<uword>({ 1, 2, 3 });
}

/**
 * \brief No Tag version.
 * \param ST `step_tag`
 * \param NT `nodes`
 * \param TP BC TYPE
 */
BC::BC(const unsigned& ST, const uvec& NT, const char* TP)
    : Constraint(0, CT_BC, ST)
    , nodes(NT) {
    if(is_equal(TP, "XSYMM") || is_equal(TP, "X"))
        dofs = std::initializer_list<uword>({ 1, 5, 6 });
    else if(is_equal(TP, "YSYMM") || is_equal(TP, "Y"))
        dofs = std::initializer_list<uword>({ 2, 4, 6 });
    else if(is_equal(TP, "ZSYMM") || is_equal(TP, "Z"))
        dofs = std::initializer_list<uword>({ 3, 4, 5 });
    else if(is_equal(TP, "ENCASTRE") || is_equal(TP, "E"))
        dofs = std::initializer_list<uword>({ 1, 2, 3, 4, 5, 6 });
    else if(is_equal(TP, "PINNED") || is_equal(TP, "P"))
        dofs = std::initializer_list<uword>({ 1, 2, 3 });
}

/**
 * \brief The default destructor.
 */
BC::~BC() {}

/**
 * \brief Method to get Node indices.
 * \return `nodes`
 */
const uvec& BC::get_node() const { return nodes; }

/**
 * \brief Method to get DoF indices.
 * \return `dofs`
 */
const uvec& BC::get_dof() const { return dofs; }

/**
 * \brief Method to apply the BC to the system.
 * \param D `Domain`
 * \return 0
 */
int BC::process(const shared_ptr<DomainBase>& D) {
    auto& t_matrix = get_stiffness(D->get_factory());

    auto& t_set_b = D->get_constrained_dof();

    for(const auto& I : nodes) {
        auto& t_node = D->get_node(static_cast<unsigned>(I));
        if(t_node != nullptr && t_node->is_active()) {
            auto& t_dof = t_node->get_reordered_dof();
            for(const auto& J : dofs)
                if(J <= t_dof.n_elem) {
                    auto& t_idx = t_dof(J - 1);
                    if(D->insert_restrained_dof(static_cast<unsigned>(t_idx)))
                        if(t_matrix(t_idx, t_idx) == 0) {
                            auto& t_set = D->get_restrained_dof();
                            t_matrix(t_idx, t_idx) = t_set.size() == 1 ? t_set_b.size() == 0 ? multiplier * t_matrix.max() : t_matrix(*t_set_b.cbegin(), *t_set_b.cbegin()) : *t_set.cbegin() == t_idx ? t_matrix(*++t_set.cbegin(), *++t_set.cbegin()) : t_matrix(*t_set.cbegin(), *t_set.cbegin());
                        } else
                            t_matrix(t_idx, t_idx) *= multiplier;
                }
        }
    }

    return 0;
}
