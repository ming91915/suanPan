#include "integrationPlan.h"
#include <cmath>
#include <cstdio>

integrationPlan::integrationPlan(const unsigned& intDimension,
    const unsigned& intOrder,
    const unsigned& intType)
{
    num_row = static_cast<int>(pow(intOrder, intDimension) + .5);
    num_col = intDimension + 1;
    auto PTL = new double[intOrder];
    auto PTW = new double[intOrder];
    // GAUSS INTEGRATION
    if(intType == 1) {
        switch(intOrder) {
        case 1: {
            PTL[0] = 0.; // POINTS LOCATION.
            PTW[0] = 2.; // POINTS WEIGHT.
            break;
        }
        case 2: {
            auto TMPA = 1. / sqrt(3.);
            PTL[0] = -TMPA;
            PTL[1] = TMPA;
            PTW[0] = 1.;
            PTW[1] = 1.;
            break;
        }
        case 3: {
            auto TMPA = sqrt(.6);
            PTL[0] = -TMPA;
            PTL[1] = 0.;
            PTL[2] = TMPA;
            PTW[0] = 5. / 9.;
            PTW[1] = 8. / 9.;
            PTW[2] = 5. / 9.;
            break;
        }
        case 4: {
            auto TMPA = 3. / 7.;
            auto TMPB = 2. / 7. * sqrt(1.2);
            auto TMPC = sqrt(TMPA - TMPB);
            auto TMPD = sqrt(TMPA + TMPB);
            auto TMPE = sqrt(30.) / 36.;
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
            auto TMPA = 2. * sqrt(10. / 7.);
            auto TMPB = sqrt(5. - TMPA);
            auto TMPC = sqrt(5. + TMPA);
            auto TMPD = 13. * sqrt(70.);
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
        };
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
            auto TMPA = sqrt(.2);
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
            auto TMPA = 2. * sqrt(7.);
            auto TMPB = sqrt((7. - TMPA) / 21.);
            auto TMPC = sqrt((7. + TMPA) / 21.);
            auto TMPD = 14. / 30.;
            auto TMPE = sqrt(7.) / 30.;
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
    int_pts = new double*[num_row];
    if(intDimension == 1) {
        for(unsigned i = 0; i < intOrder; ++i) {
            int_pts[IDX] = new double[num_col];
            int_pts[IDX][0] = PTL[i];
            int_pts[IDX++][1] = PTW[i];
        }
    } else if(intDimension == 2) {
        for(unsigned i = 0; i < intOrder; ++i) {
            for(unsigned j = 0; j < intOrder; ++j) {
                int_pts[IDX] = new double[num_col];
                int_pts[IDX][0] = PTL[i];
                int_pts[IDX][1] = PTL[j];
                int_pts[IDX++][2] = PTW[i] * PTW[j];
            }
        }
    } else if(intDimension == 3) {
        for(unsigned i = 0; i < intOrder; ++i) {
            for(unsigned j = 0; j < intOrder; ++j) {
                for(unsigned k = 0; k < intOrder; ++k) {
                    int_pts[IDX] = new double[num_col];
                    int_pts[IDX][0] = PTL[i];
                    int_pts[IDX][1] = PTL[j];
                    int_pts[IDX][2] = PTL[k];
                    int_pts[IDX++][3] = PTW[i] * PTW[j] * PTW[k];
                }
            }
        }
    } else {
        printf("NOT YET IMPLEMENTED.\n");
    }

    delete[] PTL;
    delete[] PTW;
}

integrationPlan::~integrationPlan()
{
    for(unsigned i = 0; i < num_row; ++i) delete[] int_pts[i];
    delete[] int_pts;
}

const unsigned& integrationPlan::n_rows() const { return num_row; }

const unsigned& integrationPlan::n_cols() const { return num_col; }

unsigned integrationPlan::n_elem() const { return num_col * num_row; }

double** integrationPlan::get_integration_scheme() const { return int_pts; }

double integrationPlan::operator()(const unsigned& i, const unsigned& j) const
{
    if(i < num_row && j < num_col && i >= 0 && j >= 0) return int_pts[i][j];
    printf("OUT OF BOUND.\n");
    return 0.;
}

void integrationPlan::print() const
{
    for(unsigned i = 0; i < num_row; ++i) {
        printf("Node %d\t", i + 1);
        for(unsigned j = 0; j < num_col - 1; ++j) printf("%+.6E\t", int_pts[i][j]);
        printf("Weight\t%+.6E\n", int_pts[i][num_col - 1]);
    }
    printf("\n");
}
