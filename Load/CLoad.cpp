#include "CLoad.h"
#include "Amplitude/Amplitude.h"
#include <Domain/Domain.h>
#include <Domain/Node.h>
#include <Domain/Workroom.h>

CLoad::CLoad(const unsigned& T,
    const unsigned& S,
    const double& L,
    const uvec& N,
    const unsigned& D,
    const shared_ptr<Amplitude>& A)
    : Load(T, CT_CLOAD, S, A)
    , pattern(L)
    , nodes(N)
    , dofs({ D })
{
}

CLoad::CLoad(const unsigned& S,
    const double& L,
    const uvec& N,
    const unsigned& D,
    const shared_ptr<Amplitude>& A)
    : Load(0, CT_CLOAD, S, A)
    , pattern(L)
    , nodes(N)
    , dofs({ D })
{
}

CLoad::CLoad(const unsigned& T,
    const unsigned& S,
    const double& L,
    const uvec& N,
    const uvec& D,
    const shared_ptr<Amplitude>& A)
    : Load(T, CT_CLOAD, S, A)
    , pattern(L)
    , nodes(N)
    , dofs(D)
{
}

CLoad::CLoad(const unsigned& S,
    const double& L,
    const uvec& N,
    const uvec& D,
    const shared_ptr<Amplitude>& A)
    : Load(0, CT_CLOAD, S, A)
    , pattern(L)
    , nodes(N)
    , dofs(D)
{
}

CLoad::~CLoad() {}

int CLoad::process(const shared_ptr<Domain>& D)
{
    auto& tmp_workroom = D->getWorkroom();

    auto final_load = pattern * magnitude->getAmplitude(tmp_workroom->getTrialTime());

    auto tmp_load = tmp_workroom->getTrialLoad();

    for(const auto& I : nodes) {
        auto& tmp_node = D->getNode(static_cast<unsigned>(I));
        if(tmp_node->getStatus()) {
            auto& tmp_dof = tmp_node->getReorderDOF();
            for(const auto& J : dofs)
                if(J <= tmp_dof.n_elem) tmp_load(tmp_dof(J - 1)) += final_load;
        }
    }

    tmp_workroom->updateTrialLoad(tmp_load);

    return 0;
}
