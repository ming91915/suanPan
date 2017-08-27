#include "BC.h"
#include <Domain/Domain.h>
#include <Domain/Node.h>
#include <Domain/Workroom.h>

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
    , dofs({ D })
{
}

/**
 * \brief No Tag version of the default constructor.
 * \param S `step_tag`
 * \param N `nodes`
 * \param D `dofs`
 */
BC::BC(const unsigned& S, const uvec& N, const unsigned& D)
    : Constraint(0, CT_BC, S)
    , nodes(N)
    , dofs({ D })
{
}

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
    , dofs(D)
{
}

/**
 * \brief No Tag version.
 * \param S `step_tag`
 * \param N `nodes`
 * \param D `dofs`
 */
BC::BC(const unsigned& S, const uvec& N, const uvec& D)
    : Constraint(0, CT_BC, S)
    , nodes(N)
    , dofs(D)
{
}

/**
 * \brief The constructor uses predefined TYPE: "XSYMM", "YSYMM", "ZSYMM", "ENCASTRE",
 * "PINNED".
 * \param T `unique_tag`
 * \param S `step_tag`
 * \param N `nodes`
 * \param TP BC TYPE
 */
BC::BC(const unsigned& T, const unsigned& S, const uvec& N, const char* TP)
    : Constraint(T, CT_BC, S)
    , nodes(N)
{
    if(_strcmpi(TP, "XSYMM") == 0 || _strcmpi(TP, "X") == 0)
        dofs = { 1, 5, 6 };
    else if(_strcmpi(TP, "YSYMM") == 0 || _strcmpi(TP, "Y") == 0)
        dofs = { 2, 4, 6 };
    else if(_strcmpi(TP, "ZSYMM") == 0 || _strcmpi(TP, "Z") == 0)
        dofs = { 3, 4, 5 };
    else if(_strcmpi(TP, "ENCASTRE") == 0 || _strcmpi(TP, "E") == 0)
        dofs = { 1, 2, 3, 4, 5, 6 };
    else if(_strcmpi(TP, "PINNED") == 0 || _strcmpi(TP, "P") == 0)
        dofs = { 1, 2, 3 };
}

/**
 * \brief No Tag version.
 * \param ST `step_tag`
 * \param NT `nodes`
 * \param TP BC TYPE
 */
BC::BC(const unsigned& ST, const uvec& NT, const char* TP)
    : Constraint(0, CT_BC, ST)
    , nodes(NT)
{
    if(_strcmpi(TP, "XSYMM") == 0 || _strcmpi(TP, "X") == 0)
        dofs = { 1, 5, 6 };
    else if(_strcmpi(TP, "YSYMM") == 0 || _strcmpi(TP, "Y") == 0)
        dofs = { 2, 4, 6 };
    else if(_strcmpi(TP, "ZSYMM") == 0 || _strcmpi(TP, "Z") == 0)
        dofs = { 3, 4, 5 };
    else if(_strcmpi(TP, "ENCASTRE") == 0 || _strcmpi(TP, "E") == 0)
        dofs = { 1, 2, 3, 4, 5, 6 };
    else if(_strcmpi(TP, "PINNED") == 0 || _strcmpi(TP, "P") == 0)
        dofs = { 1, 2, 3 };
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
int BC::process(const shared_ptr<Domain>& D)
{
    auto& W = D->get_workroom();
    auto& t_matrix = get_stiffness(W);

    if(!W->is_band()) {
        for(const auto& I : nodes) {
            auto& t_node = D->get_node(static_cast<unsigned>(I));
            if(t_node->is_active()) {
                auto& t_dof = t_node->get_reordered_dof();
                for(const auto& J : dofs)
                    if(J <= t_dof.n_elem) {
                        auto& t_idx = t_dof(J - 1);
                        if(D->insert_restrained_dof(static_cast<unsigned>(t_idx))) {
                            if(t_matrix(t_idx, t_idx) == 0) {
                                auto& t_set = D->get_restrained_dof();
                                if(t_set.size() == 1)
                                    t_matrix(t_idx, t_idx) = 1E8 * t_matrix.max();
                                else if(*t_set.cbegin() == t_idx)
                                    t_matrix(t_idx, t_idx) =
                                        t_matrix(*++t_set.cbegin(), *++t_set.cbegin());
                                else
                                    t_matrix(t_idx, t_idx) =
                                        t_matrix(*t_set.cbegin(), *t_set.cbegin());
                            } else
                                t_matrix(t_idx, t_idx) *= 1E8;
                        }
                    }
            }
        }
    } else {
        unsigned t_zero = 0;
        if(!W->is_symm()) {
            unsigned L, U;
            W->get_bandwidth(L, U);
            t_zero = L + U;
        }
        for(const auto& I : nodes) {
            auto& t_node = D->get_node(static_cast<unsigned>(I));
            if(t_node->is_active()) {
                auto& t_dof = t_node->get_reordered_dof();
                for(const auto& J : dofs)
                    if(J <= t_dof.n_elem) {
                        auto& t_idx = t_dof(J - 1);
                        if(D->insert_restrained_dof(static_cast<unsigned>(t_idx))) {
                            if(t_matrix(t_zero, t_idx) == 0) {
                                auto& t_set = D->get_restrained_dof();
                                if(t_set.size() == 1)
                                    t_matrix(t_zero, t_idx) = 1E8 * t_matrix.max();
                                else if(*t_set.cbegin() == t_idx)
                                    t_matrix(t_zero, t_idx) =
                                        t_matrix(t_zero, *++t_set.cbegin());
                                else
                                    t_matrix(t_zero, t_idx) =
                                        t_matrix(t_zero, *t_set.cbegin());
                            } else
                                t_matrix(t_zero, t_idx) *= 1E8;
                        }
                    }
            }
        }
    }

    return 0;
}
