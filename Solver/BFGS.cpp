#include "BFGS.h"
#include <Converger/Converger.h>
#include <Domain/Domain.h>
#include <Domain/Factory.hpp>
#include <Solver/Integrator/Integrator.h>

BFGS::BFGS(const unsigned& T, const shared_ptr<Converger>& C, const shared_ptr<Integrator>& G)
    : Solver(T, CT_BFGS, C, G) {}

BFGS::BFGS(const shared_ptr<Converger>& C, const shared_ptr<Integrator>& G)
    : Solver(0, CT_BFGS, C, G) {}

int BFGS::update_status() { return 0; }

int BFGS::analyze(const unsigned& ST) {
    auto& C = get_converger();
    auto& G = get_integrator();
    auto& W = C->get_domain()->get_factory();

    unsigned counter = 0;

    auto& ninja = get_ninja(W);

    hist_ninja.clear();
    hist_residual.clear();
    hist_factor.clear();
    while(true) {
        alpha.clear();
        G->update_trial_status();
        G->update_resistance();
        if(counter == 0) {
            G->update_stiffness();
            G->process(ST);
            hist_residual.emplace_back(W->get_trial_load() - W->get_trial_resistance());
            hist_ninja.emplace_back(W->get_stiffness()->solve(*hist_residual.crbegin()));
            ninja = *hist_ninja.crbegin(); // for updating status
        } else {
            hist_residual.emplace_back(W->get_trial_load() - W->get_trial_resistance());
            ninja = *hist_residual.crbegin();
            const auto S = hist_factor.size() - 1; // intermediate factor
            for(auto I = 0; I <= S; ++I) {
                const auto SI = S - I; // intermediate factor
                alpha.push_back(dot(hist_ninja[SI], ninja) / hist_factor[SI]);
                ninja -= *alpha.crbegin() * hist_residual[SI];
            }
            ninja = W->get_stiffness()->solve_trs(ninja);
            for(auto I = 0; I <= S; ++I) ninja += (alpha[S - I] - dot(hist_residual[I], ninja) / hist_factor[I]) * hist_ninja[I];
            hist_ninja.push_back(ninja);
        }
        hist_factor.push_back(dot(*hist_ninja.crbegin(), *hist_residual.crbegin()));
        if(counter > max_hist) {
            hist_ninja.pop_front();
            hist_residual.pop_front();
            hist_factor.pop_front();
        }
        W->update_trial_displacement(W->get_trial_displacement() + W->get_ninja());
        if(C->if_converged()) return 0;
        if(++counter > C->get_max_iteration()) return -1;
    }
}

void BFGS::print() { suanpan_info("(L-)BFGS.\n"); }
