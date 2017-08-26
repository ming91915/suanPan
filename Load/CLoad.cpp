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
    auto& tmp_workroom = D->get_workroom();

    const auto final_load =
        pattern * magnitude->get_amplitude(tmp_workroom->get_trial_time());

    auto tmp_load = tmp_workroom->get_trial_load();

    for(const auto& I : nodes) {
        auto& tmp_node = D->get_node(static_cast<unsigned>(I));
        if(tmp_node->is_active()) {
            auto& tmp_dof = tmp_node->get_reordered_dof();
            for(const auto& J : dofs)
                if(J <= tmp_dof.n_elem) tmp_load(tmp_dof(J - 1)) += final_load;
        }
    }

    tmp_workroom->update_trial_load(tmp_load);

    return 0;
}
