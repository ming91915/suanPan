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

#include "Frequence.h"
#include <Domain/Domain.h>
#include <Domain/Factory.hpp>

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

    if(get_factory() == nullptr) set_factory(make_shared<Factory<double>>());

    auto& tmp_workroom = get_factory();

    if(is_symm() && is_band())
        tmp_workroom->set_storage_scheme(StorageScheme::BANDSYMM);
    else if(!is_symm() && is_band())
        tmp_workroom->set_storage_scheme(StorageScheme::BAND);
    else if(is_symm() && !is_band())
        tmp_workroom->set_storage_scheme(StorageScheme::SYMMPACK);
    else if(!is_symm() && !is_band())
        tmp_workroom->set_storage_scheme(StorageScheme::FULL);

    tmp_domain->set_factory(tmp_workroom);
    tmp_domain->initialize();

    tmp_workroom->set_analysis_type(AnalysisType::EIGEN);

    tmp_workroom->initialize();

    return 0;
}

int Frequence::analyze() {
    auto& tmp_domain = get_domain();
    tmp_domain->update_trial_status();
    tmp_domain->assemble_mass();
    tmp_domain->assemble_stiffness();
    tmp_domain->process_load();
    tmp_domain->process_constraint();

    return 0;
}

void Frequence::set_eigen_number(const unsigned& N) { eigen_number = N; }

const unsigned& Frequence::get_eigen_number() const { return eigen_number; }
