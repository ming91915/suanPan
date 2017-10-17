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

#include "MVLEM.h"
#include "Domain/DomainBase.h"
#include "Material/Material.h"

const unsigned MVLEM::b_node = 2;
const unsigned MVLEM::b_dof = 3;

MVLEM::Fibre::Fibre(const double B, const double H, const double R)
    : width(B)
    , thickness(H)
    , c_area(B * H * (1. - R))
    , s_area(B * H * R) {}

MVLEM::MVLEM(const unsigned T, const uvec& NT, const vec& B, const vec& H, const vec& R, const uvec& CRT, const uvec& STT, const unsigned SST)
    : Element(T, ET_MVLEM, b_node, b_dof, NT, join_cols(CRT, STT), false)
    , shear_spring_tag(SST) {
    axial_spring.clear(), axial_spring.reserve(B.n_elem);
    auto total_width = 0.;
    for(auto I = 0; I < B.n_elem; ++I) {
        axial_spring.emplace_back(B(I), H(I), R(I));
        total_width += B(I);
        total_area += B(I) * H(I);
    }
    total_width *= -.5;
    for(auto I = 0; I < B.n_elem; ++I) axial_spring[I].eccentricity = total_width += .5 * axial_spring[I].width;
}

void MVLEM::initialize(const shared_ptr<DomainBase>& D) {
    const auto& total_fibre_num = axial_spring.size();
    for(auto I = 0; I < total_fibre_num; ++I) {
        axial_spring[I].c_material = make_copy(D->get_material(unsigned(material_tag(I))));
        axial_spring[I].s_material = make_copy(D->get_material(unsigned(material_tag(I + total_fibre_num))));
    }

    shear_spring = make_copy(D->get_material(shear_spring_tag));
}

int MVLEM::update_status() { return 0; }

int MVLEM::commit_status() {
    auto code = 0;

    code += shear_spring->commit_status();

    for(const auto& I : axial_spring) {
        code += I.c_material->commit_status();
        code += I.s_material->commit_status();
    }

    return code;
}

int MVLEM::clear_status() {
    auto code = 0;

    code += shear_spring->clear_status();

    for(const auto& I : axial_spring) {
        code += I.c_material->clear_status();
        code += I.s_material->clear_status();
    }

    return code;
}

int MVLEM::reset_status() {
    auto code = 0;

    code += shear_spring->reset_status();

    for(const auto& I : axial_spring) {
        code += I.c_material->reset_status();
        code += I.s_material->reset_status();
    }

    return code;
}
