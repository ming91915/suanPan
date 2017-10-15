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

IntegrationPlan::IntegrationPlan(const unsigned& intDimension, const unsigned& intOrder, const IntegrationType& intType)
    : n_rows(static_cast<int>(pow(intOrder, intDimension) + .5))
    , n_cols(intDimension + 1)
    , n_elem(n_rows * n_cols) {
    const auto PTL = new double[intOrder];
    const auto PTW = new double[intOrder];

    // GAUSS INTEGRATION
    if(intType == IntegrationType::GAUSS) {
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
        case 6: {
            PTL[0] = -.9324695142;
            PTL[1] = -.6612093865;
            PTL[2] = -.2386191861;
            PTL[3] = .2386191861;
            PTL[4] = .6612093865;
            PTL[5] = .9324695142;
            PTW[0] = .1713244924;
            PTW[1] = .3607615730;
            PTW[2] = .4679139346;
            PTW[3] = .4679139346;
            PTW[4] = .3607615730;
            PTW[5] = .1713244924;
            break;
        }
        case 7: {
            PTL[0] = -.9491079123;
            PTL[1] = -.7415311856;
            PTL[2] = -.4058451514;
            PTL[3] = 0;
            PTL[4] = .4058451514;
            PTL[5] = .7415311856;
            PTL[6] = .9491079123;
            PTW[0] = .1294849662;
            PTW[1] = .2797053915;
            PTW[2] = .3818300505;
            PTW[3] = .4179591837;
            PTW[4] = .3818300505;
            PTW[5] = .2797053915;
            PTW[6] = .1294849662;
            break;
        }
        case 8: {
            PTL[0] = -.9602898565;
            PTL[1] = -.7966664774;
            PTL[2] = -.5255324099;
            PTL[3] = -.1834346425;
            PTL[4] = .1834346425;
            PTL[5] = .5255324099;
            PTL[6] = .7966664774;
            PTL[7] = .9602898565;
            PTW[0] = .1012285363;
            PTW[1] = .2223910345;
            PTW[2] = .3137066459;
            PTW[3] = .3626837834;
            PTW[4] = .3626837834;
            PTW[5] = .3137066459;
            PTW[6] = .2223910345;
            PTW[7] = .1012285363;
            break;
        }
        case 9: {
            PTL[0] = -.9681602395;
            PTL[1] = -.8360311073;
            PTL[2] = -.6133714327;
            PTL[3] = -.3242534234;
            PTL[4] = 0;
            PTL[5] = .3242534234;
            PTL[6] = .6133714327;
            PTL[7] = .8360311073;
            PTL[8] = .9681602395;
            PTW[0] = .0812743883;
            PTW[1] = .1806481607;
            PTW[2] = .2606106964;
            PTW[3] = .3123470770;
            PTW[4] = .3302393550;
            PTW[5] = .3123470770;
            PTW[6] = .2606106964;
            PTW[7] = .1806481607;
            PTW[8] = .0812743883;
            break;
        }
        case 10: {
            PTL[0] = -.9739065285;
            PTL[1] = -.8650633667;
            PTL[2] = -.6794095683;
            PTL[3] = -.4333953941;
            PTL[4] = -.1488743390;
            PTL[5] = .1488743390;
            PTL[6] = .4333953941;
            PTL[7] = .6794095683;
            PTL[8] = .8650633667;
            PTL[9] = .9739065285;
            PTW[0] = .0666713443;
            PTW[1] = .1494513492;
            PTW[2] = .2190863625;
            PTW[3] = .2692667193;
            PTW[4] = .2955242247;
            PTW[5] = .2955242247;
            PTW[6] = .2692667193;
            PTW[7] = .2190863625;
            PTW[8] = .1494513492;
            PTW[9] = .0666713443;
            break;
        }
        default: {
            printf("NOT YET IMPLEMENTED.\n");
            return;
        }
        }
    }
    // LOBATTO INTEGRATION
    else if(intType == IntegrationType::LOBATTO) {
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
        case 7: {
            PTL[0] = -1.;
            PTL[1] = -.8302238962;
            PTL[2] = -.4688487934;
            PTL[3] = 0.;
            PTL[4] = -.4688487934;
            PTL[5] = -.8302238962;
            PTL[6] = 1.;

            PTW[0] = .0476190476;
            PTW[1] = .2768260473;
            PTW[2] = .4317453812;
            PTW[3] = .4876190476;
            PTW[4] = .4317453812;
            PTW[5] = .2768260473;
            PTW[6] = .0476190476;
            break;
        }
        case 8: {
            PTL[0] = -1.;
            PTL[1] = -.8717401485;
            PTL[2] = -.5917001814;
            PTL[3] = -.2092992179;
            PTL[4] = .2092992179;
            PTL[5] = .5917001814;
            PTL[6] = .8717401485;
            PTL[7] = 1.;

            PTW[0] = .0357142857;
            PTW[1] = .2107042271;
            PTW[2] = .3411226924;
            PTW[3] = .4124587946;
            PTW[4] = .4124587946;
            PTW[5] = .3411226924;
            PTW[6] = .2107042271;
            PTW[7] = .0357142857;
            break;
        }
        case 9: {
            PTL[0] = -1.;
            PTL[1] = -.8997579954;
            PTL[2] = -.6771862795;
            PTL[3] = -.3631174638;
            PTL[4] = 0.;
            PTL[5] = .3631174638;
            PTL[6] = .6771862795;
            PTL[7] = .8997579954;
            PTL[8] = 1.;

            PTW[0] = .0277777777;
            PTW[1] = .1654953615;
            PTW[2] = .2745387125;
            PTW[3] = .3464285109;
            PTW[4] = .3715192743;
            PTW[5] = .3464285109;
            PTW[6] = .2745387125;
            PTW[7] = .1654953615;
            PTW[8] = .0277777777;
            break;
        }
        case 10: {
            PTL[0] = -1.;
            PTL[1] = -.9195339081;
            PTL[2] = -.7387738651;
            PTL[3] = -.4779249498;
            PTL[4] = -.1652789576;
            PTL[5] = .1652789576;
            PTL[6] = .4779249498;
            PTL[7] = .7387738651;
            PTL[8] = .9195339081;
            PTL[9] = 1.;

            PTW[0] = .0222222222;
            PTW[1] = .1333059908;
            PTW[2] = .2248893420;
            PTW[3] = .2920426836;
            PTW[4] = .3275397611;
            PTW[5] = .3275397611;
            PTW[6] = .2920426836;
            PTW[7] = .2248893420;
            PTW[8] = .1333059908;
            PTW[9] = .0222222222;
            break;
        }
        case 11: {
            PTL[0] = -1.;
            PTL[1] = -.9340014304;
            PTL[2] = -.7844834736;
            PTL[3] = -.5652353269;
            PTL[4] = -.2957581355;
            PTL[5] = 0.;
            PTL[6] = .2957581355;
            PTL[7] = .5652353269;
            PTL[8] = .7844834736;
            PTL[9] = .9340014304;
            PTL[10] = 1.;

            PTW[0] = .0181818181;
            PTW[1] = .1096122732;
            PTW[2] = .1871698817;
            PTW[3] = .2480481042;
            PTW[4] = .2868791247;
            PTW[5] = .3002175954;
            PTW[6] = .2868791247;
            PTW[7] = .2480481042;
            PTW[8] = .1871698817;
            PTW[9] = .1096122732;
            PTW[10] = .0181818181;
            break;
        }
        case 12: {
            PTL[0] = -1.;
            PTL[1] = -.9448992722;
            PTL[2] = -.8192793216;
            PTL[3] = -.6328761530;
            PTL[4] = -.3995309409;
            PTL[5] = -.1365529328;
            PTL[6] = .1365529328;
            PTL[7] = .3995309409;
            PTL[8] = .6328761530;
            PTL[9] = .8192793216;
            PTL[10] = .9448992722;
            PTL[11] = 1.;

            PTW[0] = .0151515151;
            PTW[1] = .0916845174;
            PTW[2] = .1579747055;
            PTW[3] = .2125084177;
            PTW[4] = .2512756031;
            PTW[5] = .2714052409;
            PTW[6] = .2714052409;
            PTW[7] = .2512756031;
            PTW[8] = .2125084177;
            PTW[9] = .1579747055;
            PTW[10] = .0916845174;
            PTW[11] = .0151515151;
            break;
        }
        case 13: {
            PTL[0] = -1.;
            PTL[1] = -.9533098466;
            PTL[2] = -.8463475646;
            PTL[3] = -.6861884690;
            PTL[4] = -.4829098210;
            PTL[5] = -.2492869301;
            PTL[6] = 0.;
            PTL[7] = .2492869301;
            PTL[8] = .4829098210;
            PTL[9] = .6861884690;
            PTL[10] = .8463475646;
            PTL[11] = .9533098466;
            PTL[12] = 1.;

            PTW[0] = .0128205128;
            PTW[1] = .0778016867;
            PTW[2] = .1349819266;
            PTW[3] = .1836468652;
            PTW[4] = .2207677935;
            PTW[5] = .2440157903;
            PTW[6] = .2519308493;
            PTW[7] = .2440157903;
            PTW[8] = .2207677935;
            PTW[9] = .1836468652;
            PTW[10] = .1349819266;
            PTW[11] = .0778016867;
            PTW[12] = .0128205128;
            break;
        }
        default: {
            printf("NOT YET IMPLEMENTED.\n");
            return;
        }
        }
    } else if(intType == IntegrationType::RADAU) {
        switch(intOrder) {
        case 2: {
            PTL[0] = -1.;
            PTL[1] = 1. / 3.;

            PTW[0] = .5;
            PTW[1] = 1.5;
            break;
        }
        case 3: {
            PTL[0] = -1.;
            PTL[1] = -.2898979485;
            PTL[2] = .6898979485;

            PTW[0] = 2. / 9.;
            PTW[1] = 1.0249716523;
            PTW[2] = .7528061254;
            break;
        }
        case 4: {
            PTL[0] = -1.;
            PTL[1] = -.5753189235;
            PTL[2] = .1810662711;
            PTL[3] = .8228240809;

            PTW[0] = .125;
            PTW[1] = .6576886399;
            PTW[2] = .7763869376;
            PTW[3] = .4409244223;
            break;
        }
        case 5: {
            PTL[0] = -1.;
            PTL[1] = -.7204802713;
            PTL[2] = -.1671808647;
            PTL[3] = -.44631398727;
            PTL[4] = .8857916077;

            PTW[0] = .08;
            PTW[1] = .4462078021;
            PTW[2] = .6236530459;
            PTW[3] = .5627120302;
            PTW[4] = .2874271215;
            break;
        }
        case 6: {
            PTL[0] = -1.;
            PTL[1] = -.8029298284;
            PTL[2] = .3909285467;
            PTL[3] = .1240503795;
            PTL[4] = .6039731642;
            PTL[5] = .9203802858;

            PTW[0] = 1. / 18.;
            PTW[1] = .3196407532;
            PTW[2] = .4853871884;
            PTW[3] = .5209267831;
            PTW[4] = .4169013343;
            PTW[5] = .2015883852;
            break;
        }
        case 7: {
            PTL[0] = -1.;
            PTL[1] = -.8538913426;
            PTL[2] = -.5384677240;
            PTL[3] = -.1173430375;
            PTL[4] = .3260306194;
            PTL[5] = .7038428006;
            PTL[6] = .9413671456;

            PTW[0] = 2. / 49.;
            PTW[1] = .2392274892;
            PTW[2] = .3809498736;
            PTW[3] = .4471098290;
            PTW[4] = .4247037790;
            PTW[5] = .3182042314;
            PTW[6] = .1489884711;
            break;
        }
        case 8: {
            PTL[0] = -1.;
            PTL[1] = -.8874748789;
            PTL[2] = -.6395186165;
            PTL[3] = -.2947505657;
            PTL[4] = .0943072526;
            PTL[5] = .4684203544;
            PTL[6] = .7706418936;
            PTL[7] = .9550412271;

            PTW[0] = 1. / 32.;
            PTW[1] = .1853581548;
            PTW[2] = .3041306206;
            PTW[3] = .3765175453;
            PTW[4] = .3915721674;
            PTW[5] = .3470147956;
            PTW[6] = .2496479013;
            PTW[7] = .1145088147;
            break;
        }
        case 9: {
            PTL[0] = -1.;
            PTL[1] = -.9107320894;
            PTL[2] = -.7112674859;
            PTL[3] = -.4263504857;
            PTL[4] = -.0903733696;
            PTL[5] = .2561356708;
            PTL[6] = .5713830412;
            PTL[7] = .8173527842;
            PTL[8] = .9644401697;

            PTW[0] = 2. / 81.;
            PTW[1] = .1476540190;
            PTW[2] = .2471893782;
            PTW[3] = .3168437756;
            PTW[4] = .3482730027;
            PTW[5] = .3376939669;
            PTW[6] = .2863866963;
            PTW[7] = .2005532980;
            PTW[8] = .0907145049;
            break;
        }
        case 10: {
            PTL[0] = -1.;
            PTL[1] = -.9274843742;
            PTL[2] = -.7638420424;
            PTL[3] = -.5256460303;
            PTL[4] = -.2362344693;
            PTL[5] = .0760591978;
            PTL[6] = .3806648401;
            PTL[7] = .6477666876;
            PTL[8] = .8512252205;
            PTL[9] = .9711751807;

            PTW[0] = .02;
            PTW[1] = .1202966705;
            PTW[2] = .2042701318;
            PTW[3] = .2681948378;
            PTW[4] = .3058592877;
            PTW[5] = .3135824572;
            PTW[6] = .2906101648;
            PTW[7] = .2391934317;
            PTW[8] = .1643760127;
            PTW[9] = .0736170054;
            break;
        }
        default: { break; }
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
    if(i < n_rows && j < n_cols) return int_pts[i][j];
    printf("OUT OF BOUND.\n");
    return 0.;
}

void IntegrationPlan::print() const {
    for(unsigned i = 0; i < n_rows; ++i) {
        printf("Node %u\t", i + 1);
        for(unsigned j = 0; j < n_cols - 1; ++j) printf("%+.6E\t", int_pts[i][j]);
        printf("Weight\t%+.6E\n", int_pts[i][n_cols - 1]);
    }
    printf("\n");
}
