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

#include "HSection2D.h"
#include <Material/Material1D/Material1D.h>

HSection2D::IntegrationPoint::IntegrationPoint(const double C, const double W, unique_ptr<Material>&& M)
    : coor(C)
    , weight(W)
    , s_material(move(M)) {}

HSection2D::IntegrationPoint::IntegrationPoint(const IntegrationPoint& old_obj)
    : coor(old_obj.coor)
    , weight(old_obj.weight)
    , s_material(old_obj.s_material->get_copy()) {}

HSection2D::HSection2D(const unsigned T, const double TFW, const double TFT, const double BFW, const double BFT, const double WH, const double WT, const unsigned MT, const unsigned IP)
    : Section2D(T, ST_HSECTION2D, MT)
    , left_flange_height(TFW)
    , left_flange_thickness(TFT)
    , right_flange_height(BFW)
    , right_flange_thickness(BFT)
    , web_width(WH)
    , web_thickness(WT)
    , int_pt_num(IP) {}

void HSection2D::initialize(const shared_ptr<DomainBase>&) { area = left_flange_height * left_flange_thickness + right_flange_height * right_flange_thickness + web_width * web_thickness; }

unique_ptr<Section> HSection2D::get_copy() { return make_unique<HSection2D>(*this); }

double HSection2D::get_parameter(const ParameterType&) { return 0.; }

int HSection2D::update_trial_status(const vec&) { return 0; }

int HSection2D::clear_status() { return 0; }

int HSection2D::commit_status() { return 0; }

int HSection2D::reset_status() { return 0; }

void HSection2D::print() {}
