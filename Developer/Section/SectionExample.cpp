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

#include "SectionExample.h"
#include <Toolbox/utility.h>

SUANPAN_EXPORT void new_sectionexample(unique_ptr<Section>& return_obj, istringstream& command) {
    unsigned tag;
    if(!get_input(command, tag)) {
        suanpan_error("new_sectionexample() requires a valid tag.\n");
        return;
    }

    double edge;
    if(!get_input(command, edge)) {
        suanpan_error("new_sectionexample() requires a valid width.\n");
        return;
    }

    double elastic_modulus;
    if(!get_input(command, elastic_modulus)) {
        suanpan_error("new_sectionexample() requires a valid modulus.\n");
        return;
    }

    return_obj = make_unique<SectionExample>(tag, edge, elastic_modulus);
}

#ifndef ST_SECTIONEXAMPLE
#define ST_SECTIONEXAMPLE 0
#endif

SectionExample::SectionExample(const unsigned& T, const double& S, const double& E)
    : Section(T, ST_SECTIONEXAMPLE, 0)
    , edge_length(S)
    , area(S * S)
    , moment_inertia(S * S * S * S / 12.)
    , elastic_modulus(E) {}

void SectionExample::initialize(const shared_ptr<DomainBase>&) {
    stiffness.zeros(2, 2);
    stiffness(0, 0) = elastic_modulus * area;
    stiffness(1, 1) = elastic_modulus * moment_inertia;
}

unique_ptr<Section> SectionExample::get_copy() { return make_unique<SectionExample>(*this); }

int SectionExample::update_status(const vec& t_strain) {
    resistance = stiffness * t_strain;
    return 0;
}

int SectionExample::clear_status() { return 0; }

int SectionExample::commit_status() { return 0; }

int SectionExample::reset_status() { return 0; }

void SectionExample::print() { suanpan_info("This is an example section object represents the square elastic section.\n"); }
