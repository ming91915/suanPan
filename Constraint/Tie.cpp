#include "Tie.h"
#include <Domain/Domain.h>
#include <Domain/Factory.hpp>
#include <Domain/Node.h>

Tie::Tie(const unsigned& T, const unsigned& S, const unsigned& NA, const unsigned& DA, const unsigned& NB, const unsigned& DB)
    : MPC(T, CT_TIE, S)
    , node_i(NA)
    , dof_i(DA)
    , node_j(NB)
    , dof_j(DB) {}

Tie::Tie(const unsigned& S, const unsigned& NA, const unsigned& DA, const unsigned& NB, const unsigned& DB)
    : MPC(0, CT_TIE, S)
    , node_i(NA)
    , dof_i(DA)
    , node_j(NB)
    , dof_j(DB) {}

int Tie::process(const shared_ptr<Domain>& D) {
    const auto tmp_max = D->get_factory()->get_stiffness()->max();

    auto& tmp_dof_i = D->get_node(node_i)->get_reordered_dof().at(dof_i - 1);
    auto& tmp_dof_j = D->get_node(node_j)->get_reordered_dof().at(dof_j - 1);

    auto& tmp_matrix = *get_stiffness(D->get_factory());

    tmp_matrix(tmp_dof_i, tmp_dof_i) += tmp_max;
    tmp_matrix(tmp_dof_j, tmp_dof_j) += tmp_max;
    tmp_matrix(tmp_dof_i, tmp_dof_j) -= tmp_max;
    tmp_matrix(tmp_dof_j, tmp_dof_i) -= tmp_max;

    return 0;
}
