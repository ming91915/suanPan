////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2017 Theodore Chang
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////

#include "BFGS.h"
#include <Converger/Converger.h>
#include <Domain/DomainBase.h>
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
    auto& D = C->get_domain();
    auto& W = D->get_factory();

    auto& max_iteration = C->get_max_iteration();

    unsigned counter = 0;

    auto& ninja = get_ninja(W);

    hist_ninja.clear();
    hist_residual.clear();
    hist_factor.clear();
    while(true) {
        alpha.clear();
        G->update_trial_status();
        G->update_resistance();
        if(counter == 1) {
            auto& t_load = get_trial_load(W);
            for(const auto& I : D->get_constrained_dof()) t_load(I) = 0.;
        }
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
        if(C->if_converged()) return 0;
        if(++counter > max_iteration) return -1;
        if(counter > max_hist) {
            hist_ninja.pop_front();
            hist_residual.pop_front();
            hist_factor.pop_front();
        }
        W->update_trial_displacement(W->get_trial_displacement() + W->get_ninja());
        G->erase_machine_error();
    }
}

void BFGS::print() { suanpan_info("(L-)BFGS.\n"); }
