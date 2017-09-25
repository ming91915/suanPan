#include "ArcLength.h"
#include <Domain/Domain.h>
#include <Domain/Factory.hpp>
#include <Domain/Node.h>
#include <Solver/Integrator/Integrator.h>
#include <Solver/Solver.h>

ArcLength::ArcLength(const unsigned& T, const unsigned& NT, const unsigned& DT, const double& MA)
    : Step(T, CT_ARCLENGTH, 0.)
    , node(NT)
    , dof(DT)
    , maginitude(MA) {}

int ArcLength::initialize() {
    const auto code = Step::initialize();

    if(code != 0) return code;

    auto& W = get_factory();

    W->set_reference_size(1);
    W->initialize_load_factor();

    auto& t_load_ref = get_reference_load(W);

    auto& dof_anchor = get_domain()->get_node(node)->get_reordered_dof().at(dof - 1);

    t_load_ref(dof_anchor) = maginitude;

    return 0;
}

int ArcLength::analyze() {
    auto& S = get_solver();
    auto& G = get_integrator();

    G->update_trial_status();

    unsigned num_iteration = 0;

    while(true) {
        if(num_iteration++ > get_max_substep()) {
            suanpan_warning("analyze() reaches maximum substep number %u.\n", get_max_substep());
            return -1;
        }
        if(G->process_criterion() < 0) return 0;
        const auto code = S->analyze();
        if(code == 0) {
            G->commit_status();
            G->record();
        } else if(code == -1) {
            G->reset_status();
        } else
            return -1;
    }
}
