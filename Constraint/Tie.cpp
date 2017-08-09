#include "Tie.h"
#include <Domain/Domain>

Tie::Tie(const unsigned& T,
    const unsigned& S,
    const unsigned& NA,
    const unsigned& DA,
    const unsigned& NB,
    const unsigned& DB)
    : MPC(T, CT_TIE, S)
    , node_i(NA)
    , dof_i(DA)
    , node_j(NB)
    , dof_j(DB)
{
}

Tie::Tie(const unsigned& S,
    const unsigned& NA,
    const unsigned& DA,
    const unsigned& NB,
    const unsigned& DB)
    : MPC(0, CT_TIE, S)
    , node_i(NA)
    , dof_i(DA)
    , node_j(NB)
    , dof_j(DB)
{
}

int Tie::process(const shared_ptr<Domain>& D)
{
    auto tmp_max = D->getWorkroom()->getStiffness().max();

    auto tmp_dof_i = D->getNode(node_i)->getReorderDOF().at(dof_i - 1);
    auto tmp_dof_j = D->getNode(node_j)->getReorderDOF().at(dof_j - 1);

    auto& tmp_matrix = getStiffness(D->getWorkroom());

    tmp_matrix(tmp_dof_i, tmp_dof_i) += tmp_max;
    tmp_matrix(tmp_dof_j, tmp_dof_j) += tmp_max;
    tmp_matrix(tmp_dof_i, tmp_dof_j) -= tmp_max;
    tmp_matrix(tmp_dof_j, tmp_dof_i) -= tmp_max;

    return 0;
}
