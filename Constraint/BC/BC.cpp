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
    if(if_equal(TP, "XSYMM") || if_equal(TP, "X"))
        dofs = std::initializer_list<uword>({ 1, 5, 6 });
    else if(if_equal(TP, "YSYMM") || if_equal(TP, "Y"))
        dofs = std::initializer_list<uword>({ 2, 4, 6 });
    else if(if_equal(TP, "ZSYMM") || if_equal(TP, "Z"))
        dofs = std::initializer_list<uword>({ 3, 4, 5 });
    else if(if_equal(TP, "ENCASTRE") || if_equal(TP, "E"))
        dofs = std::initializer_list<uword>({ 1, 2, 3, 4, 5, 6 });
    else if(if_equal(TP, "PINNED") || if_equal(TP, "P"))
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
    if(if_equal(TP, "XSYMM") || if_equal(TP, "X"))
        dofs = std::initializer_list<uword>({ 1, 5, 6 });
    else if(if_equal(TP, "YSYMM") || if_equal(TP, "Y"))
        dofs = std::initializer_list<uword>({ 2, 4, 6 });
    else if(if_equal(TP, "ZSYMM") || if_equal(TP, "Z"))
        dofs = std::initializer_list<uword>({ 3, 4, 5 });
    else if(if_equal(TP, "ENCASTRE") || if_equal(TP, "E"))
        dofs = std::initializer_list<uword>({ 1, 2, 3, 4, 5, 6 });
    else if(if_equal(TP, "PINNED") || if_equal(TP, "P"))
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
                            t_matrix(t_idx, t_idx) = t_set.size() == 1 ? t_set_b.size() == 0 ? 1E6 * t_matrix.max() : t_matrix(*t_set_b.cbegin(), *t_set_b.cbegin()) : *t_set.cbegin() == t_idx ? t_matrix(*++t_set.cbegin(), *++t_set.cbegin()) : t_matrix(*t_set.cbegin(), *t_set.cbegin());
                        } else
                            t_matrix(t_idx, t_idx) *= 1E6;
                }
        }
    }

    return 0;
}
