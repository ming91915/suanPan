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

#include "Load.h"
#include <Domain/DomainBase.h>
#include <Load/Amplitude/Ramp.h>
#include <Step/Step.h>

const double Load::multiplier = 1E6;

Load::Load(const unsigned& T, const unsigned& CT, const unsigned& ST, const unsigned& AT)
    : Tag(T, CT)
    , step_tag(ST)
    , amplitude_tag(AT) {}

Load::~Load() { suanpan_debug("Load %u dtor() called.\n", get_tag()); }

int Load::initialize(const shared_ptr<DomainBase>& D) {
    if(amplitude_tag != 0) magnitude = D->get_amplitude(amplitude_tag);

    if(amplitude_tag == 0 || magnitude == nullptr) {
        auto t_tag = static_cast<unsigned>(D->get_amplitude()) + 1;
        while(D->find_amplitude(t_tag)) t_tag++;
        magnitude = make_shared<Ramp>(t_tag);
        D->insert(magnitude);
    }

    auto start_time = 0.;
    for(const auto& I : *D->get_step_anchor().lock()) {
        if(I.second->get_tag() >= step_tag) break;
        start_time += I.second->get_time_period();
    }

    magnitude->set_step_tag(step_tag);
    magnitude->set_start_time(start_time);

    return 0;
}

int Load::process(const shared_ptr<DomainBase>&) { return -1; }

void Load::set_step_tag(const unsigned& T) { step_tag = T; }

const unsigned& Load::get_step_tag() const { return step_tag; }
