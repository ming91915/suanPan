#include "BFGS.h"
#include <Converger/Converger.h>
#include <Domain/Domain.h>
#include <Domain/Workshop.h>
#include <Solver/Integrator/Integrator.h>

BFGS::BFGS(const unsigned& T, const shared_ptr<Converger>& C, const shared_ptr<Integrator>& G)
    : Solver(T, CT_BFGS, C, G) {}

BFGS::BFGS(const shared_ptr<Converger>& C, const shared_ptr<Integrator>& G)
    : Solver(0, CT_BFGS, C, G) {}

int BFGS::initialize() {
    if(Solver::initialize() != 0) return -1;

    auto& W = get_converger()->get_domain()->get_workshop();

    if(W->is_band() || W->is_symm()) {
        suanpan_fatal("analyze() currently does not suppoort band matrix.\n");
        return -1;
    }

    return 0;
}

int BFGS::update_status() { return 0; }

int BFGS::analyze(const unsigned& ST) {
    auto& C = get_converger();
    auto& G = get_integrator();
    auto& W = C->get_domain()->get_workshop();

    unsigned counter = 0;

    auto& tmp_ninja = get_ninja(W);

    vec tmp_residual;

    mat inv_stiffness;

    while(true) {
        G->update_resistance();
        if(counter == 0) {
            G->update_stiffness();
            G->process(ST);
            if(!inv_sympd(inv_stiffness, W->get_stiffness())) return 1;
        } else {
            const auto factor = dot(tmp_residual, tmp_ninja);
            mat tmp_a = tmp_ninja * tmp_residual.t() / factor;
            for(auto I = 0; I < tmp_a.n_rows; ++I) tmp_a(I, I) -= 1.;
            inv_stiffness = tmp_a * inv_stiffness * tmp_a.t() + tmp_ninja * tmp_ninja.t() / factor;
        }
        tmp_residual = W->get_trial_load() - W->get_trial_resistance();
        tmp_ninja = inv_stiffness * tmp_residual;
        W->update_trial_displacement(W->get_trial_displacement() + W->get_ninja());
        G->update_trial_status();
        if(C->if_converged()) return 0;
        if(++counter > C->get_max_iteration()) return -1;
    }
}

void BFGS::print() {}
