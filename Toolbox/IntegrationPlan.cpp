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

#include "IntegrationPlan.h"
#include <cmath>
#include <cstdio>

IntegrationPlan::IntegrationPlan(const unsigned& intDimension, const unsigned& intOrder, const unsigned& intType)
    : n_rows(static_cast<int>(pow(intOrder, intDimension) + .5))
    , n_cols(intDimension + 1)
    , n_elem(n_rows * n_cols) {
    const auto PTL = new double[intOrder];
    const auto PTW = new double[intOrder];

    // GAUSS INTEGRATION
    if(intType == 1) {
        switch(intOrder) {
        case 1: {
            PTL[0] = 0.; // POINTS LOCATION.
            PTW[0] = 2.; // POINTS WEIGHT.
            break;
        }
        case 2: {
            const auto TMPA = 1. / sqrt(3.);
            PTL[0] = -TMPA;
            PTL[1] = TMPA;
            PTW[0] = 1.;
            PTW[1] = 1.;
            break;
        }
        case 3: {
            const auto TMPA = sqrt(.6);
            PTL[0] = -TMPA;
            PTL[1] = 0.;
            PTL[2] = TMPA;
            PTW[0] = 5. / 9.;
            PTW[1] = 8. / 9.;
            PTW[2] = 5. / 9.;
            break;
        }
        case 4: {
            const auto TMPA = 3. / 7.;
            const auto TMPB = 2. / 7. * sqrt(1.2);
            const auto TMPC = sqrt(TMPA - TMPB);
            const auto TMPD = sqrt(TMPA + TMPB);
            const auto TMPE = sqrt(30.) / 36.;
            PTL[0] = -TMPD;
            PTL[1] = -TMPC;
            PTL[2] = TMPC;
            PTL[3] = TMPD;
            PTW[0] = -TMPE + .5;
            PTW[1] = TMPE + .5;
            PTW[2] = TMPE + .5;
            PTW[3] = -TMPE + .5;
            break;
        }
        case 5: {
            const auto TMPA = 2. * sqrt(10. / 7.);
            const auto TMPB = sqrt(5. - TMPA);
            const auto TMPC = sqrt(5. + TMPA);
            const auto TMPD = 13. * sqrt(70.);
            PTL[0] = -TMPC / 3.;
            PTL[1] = -TMPB / 3.;
            PTL[2] = 0.;
            PTL[3] = TMPB / 3.;
            PTL[4] = TMPC / 3.;
            PTW[0] = (322. - TMPD) / 900.;
            PTW[1] = (322. + TMPD) / 900.;
            PTW[2] = 512. / 900.;
            PTW[3] = (322. - TMPD) / 900.;
            PTW[4] = (322. + TMPD) / 900.;
            break;
        }
        default: {
            printf("NOT YET IMPLEMENTED.\n");
            return;
        }
        }
    }
    // LOBATTO INTEGRATION
    else if(intType == 2) {
        switch(intOrder) {
        case 3: {
            PTL[0] = -1.;
            PTL[1] = 0.;
            PTL[2] = 1.;
            PTW[0] = 1. / 3.;
            PTW[1] = 4. / 3.;
            PTW[2] = 1. / 3.;
            break;
        }
        case 4: {
            const auto TMPA = sqrt(.2);
            PTL[0] = -1.;
            PTL[1] = -TMPA;
            PTL[2] = TMPA;
            PTL[3] = 1.;
            PTW[0] = 1. / 6.;
            PTW[1] = 5. / 6.;
            PTW[2] = 5. / 6.;
            PTW[3] = 1. / 6.;
            break;
        }
        case 5: {
            PTL[0] = -1.;
            PTL[1] = -sqrt(3. / 7.);
            PTL[2] = 0.;
            PTL[3] = sqrt(3. / 7.);
            PTL[4] = 1.;
            PTW[0] = .1;
            PTW[1] = 49. / 90.;
            PTW[2] = 64. / 90.;
            PTW[3] = 49. / 90.;
            PTW[4] = .1;
            break;
        }
        case 6: {
            const auto TMPA = 2. * sqrt(7.);
            const auto TMPB = sqrt((7. - TMPA) / 21.);
            const auto TMPC = sqrt((7. + TMPA) / 21.);
            const auto TMPD = 14. / 30.;
            const auto TMPE = sqrt(7.) / 30.;
            PTL[0] = -1.;
            PTL[1] = -TMPC;
            PTL[2] = -TMPB;
            PTL[3] = TMPB;
            PTL[4] = TMPC;
            PTL[5] = 1.;
            PTW[0] = 1. / 15.;
            PTW[1] = TMPD - TMPE;
            PTW[2] = TMPD + TMPE;
            PTW[3] = TMPD + TMPE;
            PTW[4] = TMPD - TMPE;
            PTW[5] = 1. / 15.;
            break;
        }
        default: {
            printf("NOT YET IMPLEMENTED.\n");
            return;
        }
        }
    }
    auto IDX = 0;
    int_pts = new double*[n_rows];
    if(intDimension == 1)
        for(unsigned i = 0; i < intOrder; ++i) {
            int_pts[IDX] = new double[n_cols];
            int_pts[IDX][0] = PTL[i];
            int_pts[IDX++][1] = PTW[i];
        }
    else if(intDimension == 2)
        for(unsigned i = 0; i < intOrder; ++i)
            for(unsigned j = 0; j < intOrder; ++j) {
                int_pts[IDX] = new double[n_cols];
                int_pts[IDX][0] = PTL[i];
                int_pts[IDX][1] = PTL[j];
                int_pts[IDX++][2] = PTW[i] * PTW[j];
            }
    else if(intDimension == 3)
        for(unsigned i = 0; i < intOrder; ++i)
            for(unsigned j = 0; j < intOrder; ++j)
                for(unsigned k = 0; k < intOrder; ++k) {
                    int_pts[IDX] = new double[n_cols];
                    int_pts[IDX][0] = PTL[i];
                    int_pts[IDX][1] = PTL[j];
                    int_pts[IDX][2] = PTL[k];
                    int_pts[IDX++][3] = PTW[i] * PTW[j] * PTW[k];
                }
    else
        printf("NOT YET IMPLEMENTED.\n");

    if(PTL != nullptr) delete[] PTL;
    if(PTW != nullptr) delete[] PTW;
}

IntegrationPlan::~IntegrationPlan() {
    for(unsigned i = 0; i < n_rows; ++i)
        if(int_pts[i] != nullptr) delete[] int_pts[i];
    if(int_pts != nullptr) delete[] int_pts;
}

double** IntegrationPlan::get_integration_scheme() const { return int_pts; }

double IntegrationPlan::operator()(const unsigned& i, const unsigned& j) const {
    if(i < n_rows && j < n_cols && i >= 0 && j >= 0) return int_pts[i][j];
    printf("OUT OF BOUND.\n");
    return 0.;
}

void IntegrationPlan::print() const {
    for(unsigned i = 0; i < n_rows; ++i) {
        printf("Node %d\t", i + 1);
        for(unsigned j = 0; j < n_cols - 1; ++j) printf("%+.6E\t", int_pts[i][j]);
        printf("Weight\t%+.6E\n", int_pts[i][n_cols - 1]);
    }
    printf("\n");
}
