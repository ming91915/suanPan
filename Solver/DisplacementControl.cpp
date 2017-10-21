#include "DisplacementControl.h"
#include <Converger/Converger.h>
#include <Domain/DomainBase.h>
#include <Domain/Factory.hpp>
#include <Solver/Integrator/Integrator.h>

DisplacementControl::DisplacementControl() {}

int DisplacementControl::analyze() {
    auto& C = get_converger();
    auto& G = get_integrator();
    auto& W = G->get_domain().lock()->get_factory();

    auto& max_iteration = C->get_max_iteration();

    // ninja anchor
    auto& t_ninja = get_ninja(W);

    auto& load_ref = W->get_reference_load();

    mat disp_a;

    // iteration counter
    unsigned counter = 0;

    while(true) {
        // assemble resistance
        G->assemble_resistance();
        // assemble stiffness
        G->assemble_matrix();
        // process loads
        G->process_load();
        // process constraints
        G->process_constraint();

        // solve ninja
        auto flag = W->get_stiffness()->solve(t_ninja, load_ref * W->get_trial_load_factor() + W->get_trial_load() - W->get_sushi());
        // make sure lapack solver succeeds
        if(flag != 0) return flag;
        // solve reference displacement
        flag = W->get_stiffness()->solve_trs(disp_a, load_ref);
        // make sure lapack solver succeeds
        if(flag != 0) return flag;

        const vec incre_lambda = -solve(disp_a, t_ninja);

        t_ninja += disp_a * incre_lambda;

        // avoid machine error accumulation
        G->erase_machine_error();
        // update trial load factor
        W->update_trial_load_factor(W->get_trial_load_factor() + incre_lambda);
        // update trial displacement
        W->update_trial_displacement(W->get_trial_displacement() + t_ninja);
        // update for nodes and elements
        if(G->update_trial_status() != 0) return -1;

        // exit if converged
        if(C->is_converged()) return 0;
        // exit if maximum iteration is hit
        if(++counter > max_iteration) return -1;
    }
}
