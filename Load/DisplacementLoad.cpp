#include "DisplacementLoad.h"
#include <Domain/DomainBase.h>
#include <Domain/Factory.hpp>
#include <Domain/Node.h>
#include <Load/Amplitude/Amplitude.h>

DisplacementLoad::DisplacementLoad(const unsigned& T, const unsigned& S, const double& L, const uvec& N, const unsigned& D, const shared_ptr<Amplitude>& A)
    : Load(T, CT_DISPLACEMENTLOAD)
    , pattern(L)
    , nodes(N)
    , dofs({ D }) {}

DisplacementLoad::DisplacementLoad(const unsigned& T, const unsigned& S, const double& L, const uvec& N, const uvec& D, const shared_ptr<Amplitude>& A)
    : Load(T, CT_DISPLACEMENTLOAD)
    , pattern(L)
    , nodes(N)
    , dofs(D) {}

int DisplacementLoad::process(const shared_ptr<DomainBase>& D) {
    auto& W = D->get_factory();

    const auto final_load = pattern * magnitude->get_amplitude(W->get_trial_time());

    auto& t_stiff = get_stiffness(W);
    auto& t_load = get_trial_load(W);

    for(const auto& I : nodes) {
        auto& t_node = D->get_node(static_cast<unsigned>(I));
        if(t_node != nullptr && t_node->is_active()) {
            auto& t_dof = t_node->get_reordered_dof();
            for(const auto& J : dofs)
                if(J <= t_dof.n_elem) {
                    auto& t_idx = t_dof(J - 1);
                    if(D->insert_constrained_dof(unsigned(t_idx))) {
                        if(t_stiff(t_idx, t_idx) == 0) {
                            auto& t_set = D->get_constrained_dof();
                            t_stiff(t_idx, t_idx) = t_set.size() == 1 ? multiplier * t_stiff.max() : *t_set.cbegin() == t_idx ? t_stiff(*++t_set.cbegin(), *++t_set.cbegin()) : t_stiff(*t_set.cbegin(), *t_set.cbegin());
                        } else
                            t_stiff(t_idx, t_idx) *= multiplier;
                    }
                    t_load(t_idx) = t_stiff(t_idx, t_idx) * final_load;
                }
        }
    }

    W->set_incre_load(t_load - W->get_current_load());

    return 0;
}
