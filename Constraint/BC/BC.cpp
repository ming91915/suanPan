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
const uvec& BC::getNode() const { return nodes; }

/**
 * \brief Method to get DoF indices.
 * \return `dofs`
 */
const uvec& BC::getDOF() const { return dofs; }

/**
 * \brief Method to apply the BC to the system.
 * \param D `Domain`
 * \return 0
 */
int BC::process(const shared_ptr<Domain>& D)
{
    auto& W = D->getWorkroom();
    auto& tmp_matrix = getStiffness(W);

    if(!W->is_band()) {
        for(const auto& I : nodes) {
            auto& tmp_node = D->getNode(static_cast<unsigned>(I));
            if(tmp_node->getStatus()) {
                auto& tmp_dof = tmp_node->getReorderDOF();
                for(const auto& J : dofs)
                    if(J <= tmp_dof.n_elem) {
                        auto& tmp_idx = tmp_dof(J - 1);
                        if(D->insertRestrainedDOF(static_cast<unsigned>(tmp_idx))) {
                            if(tmp_matrix(tmp_idx, tmp_idx) == 0) {
                                auto& tmp_set = D->getRestrainedDOF();
                                if(tmp_set.size() == 1)
                                    tmp_matrix(tmp_idx, tmp_idx) = 1E6 * tmp_matrix.max();
                                else if(*tmp_set.cbegin() == tmp_idx)
                                    tmp_matrix(tmp_idx, tmp_idx) = tmp_matrix(
                                        *++tmp_set.cbegin(), *++tmp_set.cbegin());
                                else
                                    tmp_matrix(tmp_idx, tmp_idx) =
                                        tmp_matrix(*tmp_set.cbegin(), *tmp_set.cbegin());
                            } else
                                tmp_matrix(tmp_idx, tmp_idx) *= 1E6;
                        }
                    }
            }
        }
    } else {
        unsigned tmp_i = 0;
        if(!W->is_symm()) {
            unsigned L, U;
            W->getBandwidth(L, U);
            tmp_i = L + U;
        }
        for(const auto& I : nodes) {
            auto& tmp_node = D->getNode(static_cast<unsigned>(I));
            if(tmp_node->getStatus()) {
                auto& tmp_dof = tmp_node->getReorderDOF();
                for(const auto& J : dofs)
                    if(J <= tmp_dof.n_elem) {
                        auto& tmp_idx = tmp_dof(J - 1);
                        if(D->insertRestrainedDOF(static_cast<unsigned>(tmp_idx))) {
                            if(tmp_matrix(tmp_i, tmp_idx) == 0) {
                                auto& tmp_set = D->getRestrainedDOF();
                                if(tmp_set.size() == 1)
                                    tmp_matrix(tmp_i, tmp_idx) = 1E6 * tmp_matrix.max();
                                else if(*tmp_set.cbegin() == tmp_idx)
                                    tmp_matrix(tmp_i, tmp_idx) =
                                        tmp_matrix(tmp_i, *++tmp_set.cbegin());
                                else
                                    tmp_matrix(tmp_i, tmp_idx) =
                                        tmp_matrix(tmp_i, *tmp_set.cbegin());
                            } else
                                tmp_matrix(tmp_i, tmp_idx) *= 1E6;
                        }
                    }
            }
        }
    }

    return 0;
}
