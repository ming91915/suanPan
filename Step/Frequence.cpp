#include "Frequence.h"
#include <Domain/Domain.h>
#include <Domain/Workshop.h>

Frequence::Frequence(const unsigned& T, const unsigned& N)
    : Step(T, CT_FREQUENCE, 0.)
    , eigen_number(N) {}

int Frequence::initialize() {
    if(is_updated()) return 0;

    auto& tmp_domain = get_domain();

    if(tmp_domain == nullptr) {
        suanpan_error("initialize() needs a valid Domain.\n");
        return -1;
    }

    if(get_workshop() == nullptr) set_workshop(make_shared<Workshop>());

    auto& tmp_workroom = get_workshop();

    tmp_workroom->set_symm(is_symm());
    tmp_workroom->set_band(is_band());

    tmp_domain->set_workshop(tmp_workroom);
    tmp_domain->initialize();

    tmp_workroom->set_analysis_type(AnalysisType::EIGEN);

    tmp_workroom->initialize();

    return 0;
}

int Frequence::analyze() {
    auto& tmp_domain = get_domain();
    tmp_domain->update_trial_status();
    tmp_domain->update_mass();
    tmp_domain->update_stiffness();
    tmp_domain->process(get_tag());

    return 0;
}

void Frequence::set_eigen_number(const unsigned& N) { eigen_number = N; }

const unsigned& Frequence::get_eigen_number() const { return eigen_number; }
