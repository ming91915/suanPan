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

#include "ISection2D.h"
#include <Material/Material1D/Material1D.h>

ISection2D::IntegrationPoint::IntegrationPoint(const double C, const double W, unique_ptr<Material>&& M)
    : coor(C)
    , weight(W)
    , s_material(move(M)) {}

ISection2D::IntegrationPoint::IntegrationPoint(const IntegrationPoint& old_obj)
    : coor(old_obj.coor)
    , weight(old_obj.weight)
    , s_material(old_obj.s_material->get_copy()) {}

ISection2D::ISection2D(const unsigned T, const double TFW, const double TFT, const double BFW, const double BFT, const double WH, const double WT, const unsigned MT, const unsigned IP)
    : Section2D(T, ST_HSECTION2D, MT)
    , top_flange_width(TFW)
    , top_flange_thickness(TFT)
    , bottom_flange_width(BFW)
    , bottom_flange_thickness(BFT)
    , web_height(WH)
    , web_thickness(WT)
    , int_pt_num(IP) {}

void ISection2D::initialize(const shared_ptr<DomainBase>&) {}

unique_ptr<Section> ISection2D::get_copy() { return make_unique<ISection2D>(*this); }

double ISection2D::get_parameter(const ParameterType& P) {
    if(P == ParameterType::AREA) return area;

    return 0.;
}

int ISection2D::update_trial_status(const vec&) { return 0; }

int ISection2D::clear_status() { return 0; }

int ISection2D::commit_status() { return 0; }

int ISection2D::reset_status() { return 0; }

void ISection2D::print() {}
