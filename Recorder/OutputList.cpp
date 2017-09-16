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
﻿#include "OutputList.h"
#include <Toolbox/utility.h>

const char* to_char(const OutputList& L) {
    switch(L) {
    case OutputList::S:
        return "S";
    case OutputList::S11:
        return "S11";
    case OutputList::S22:
        return "S22";
    case OutputList::S33:
        return "S33";
    case OutputList::S12:
        return "S12";
    case OutputList::S23:
        return "S23";
    case OutputList::S13:
        return "S13";
    case OutputList::E:
        return "E";
    case OutputList::E11:
        return "E11";
    case OutputList::E22:
        return "E22";
    case OutputList::E33:
        return "E33";
    case OutputList::E12:
        return "E12";
    case OutputList::E23:
        return "E23";
    case OutputList::E13:
        return "E13";
    case OutputList::SP:
        return "SP";
    case OutputList::SP1:
        return "SP1";
    case OutputList::SP2:
        return "SP2";
    case OutputList::SP3:
        return "SP3";
    case OutputList::EP:
        return "EP";
    case OutputList::EP1:
        return "EP1";
    case OutputList::EP2:
        return "EP2";
    case OutputList::EP3:
        return "EP3";
    case OutputList::SINV:
        return "SINV";
    case OutputList::MISES:
        return "MISES";
    case OutputList::TRESC:
        return "TRESC";
    case OutputList::PE:
        return "PE";
    case OutputList::PE11:
        return "PE11";
    case OutputList::PE22:
        return "PE22";
    case OutputList::PE33:
        return "PE33";
    case OutputList::PE12:
        return "PE12";
    case OutputList::PE23:
        return "PE23";
    case OutputList::PE13:
        return "PE13";
    case OutputList::PEEQ:
        return "PEEQ";

    case OutputList::U:
        return "U";
    case OutputList::UT:
        return "UT";
    case OutputList::UR:
        return "UR";
    case OutputList::U1:
        return "U1";
    case OutputList::U2:
        return "U2";
    case OutputList::U3:
        return "U3";
    case OutputList::UR1:
        return "UR1";
    case OutputList::UR2:
        return "UR2";
    case OutputList::UR3:
        return "UR3";
    case OutputList::V:
        return "V";
    case OutputList::VT:
        return "VT";
    case OutputList::VR:
        return "VR";
    case OutputList::V1:
        return "V1";
    case OutputList::V2:
        return "V2";
    case OutputList::V3:
        return "V3";
    case OutputList::VR1:
        return "VR1";
    case OutputList::VR2:
        return "VR2";
    case OutputList::VR3:
        return "VR3";
    case OutputList::A:
        return "A";
    case OutputList::AT:
        return "AT";
    case OutputList::AR:
        return "AR";
    case OutputList::A1:
        return "A1";
    case OutputList::A2:
        return "A2";
    case OutputList::A3:
        return "A3";
    case OutputList::AR1:
        return "AR1";
    case OutputList::AR2:
        return "AR2";
    case OutputList::AR3:
        return "AR3";

    case OutputList::RF:
        return "RF";
    case OutputList::RF1:
        return "RF1";
    case OutputList::RF2:
        return "RF2";
    case OutputList::RF3:
        return "RF3";
    case OutputList::RM:
        return "RM";
    case OutputList::RM1:
        return "RM1";
    case OutputList::RM2:
        return "RM2";
    case OutputList::RM3:
        return "RM3";
    case OutputList::RT:
        return "RT";

    case OutputList::NL:
        return "NL";
    default:
        return "NL";
    }
}

OutputList to_list(const char* L) {
    if(if_equal(L, "S")) return OutputList::S;
    if(if_equal(L, "S11")) return OutputList::S11;
    if(if_equal(L, "S22")) return OutputList::S22;
    if(if_equal(L, "S33")) return OutputList::S33;
    if(if_equal(L, "S12")) return OutputList::S12;
    if(if_equal(L, "S23")) return OutputList::S23;
    if(if_equal(L, "S13")) return OutputList::S13;
    if(if_equal(L, "E")) return OutputList::E;
    if(if_equal(L, "E11")) return OutputList::E11;
    if(if_equal(L, "E22")) return OutputList::E22;
    if(if_equal(L, "E33")) return OutputList::E33;
    if(if_equal(L, "E12")) return OutputList::E12;
    if(if_equal(L, "E23")) return OutputList::E23;
    if(if_equal(L, "E13")) return OutputList::E13;
    if(if_equal(L, "SP")) return OutputList::SP;
    if(if_equal(L, "SP1")) return OutputList::SP1;
    if(if_equal(L, "SP2")) return OutputList::SP2;
    if(if_equal(L, "SP3")) return OutputList::SP3;
    if(if_equal(L, "EP")) return OutputList::EP;
    if(if_equal(L, "EP1")) return OutputList::EP1;
    if(if_equal(L, "EP2")) return OutputList::EP2;
    if(if_equal(L, "EP3")) return OutputList::EP3;
    if(if_equal(L, "SINV")) return OutputList::SINV;
    if(if_equal(L, "MISES")) return OutputList::MISES;
    if(if_equal(L, "TRESC")) return OutputList::TRESC;

    if(if_equal(L, "PE")) return OutputList::PE;
    if(if_equal(L, "PE11")) return OutputList::PE11;
    if(if_equal(L, "PE22")) return OutputList::PE22;
    if(if_equal(L, "PE33")) return OutputList::PE33;
    if(if_equal(L, "PE12")) return OutputList::PE12;
    if(if_equal(L, "PE23")) return OutputList::PE23;
    if(if_equal(L, "PE13")) return OutputList::PE13;
    if(if_equal(L, "PEEQ")) return OutputList::PEEQ;

    if(if_equal(L, "U")) return OutputList::U;
    if(if_equal(L, "UT")) return OutputList::UT;
    if(if_equal(L, "UR")) return OutputList::UR;
    if(if_equal(L, "U1")) return OutputList::U1;
    if(if_equal(L, "U2")) return OutputList::U2;
    if(if_equal(L, "U3")) return OutputList::U3;
    if(if_equal(L, "UR1")) return OutputList::UR1;
    if(if_equal(L, "UR2")) return OutputList::UR2;
    if(if_equal(L, "UR3")) return OutputList::UR3;
    if(if_equal(L, "V")) return OutputList::V;
    if(if_equal(L, "VT")) return OutputList::VT;
    if(if_equal(L, "VR")) return OutputList::VR;
    if(if_equal(L, "V1")) return OutputList::V1;
    if(if_equal(L, "V2")) return OutputList::V2;
    if(if_equal(L, "V3")) return OutputList::V3;
    if(if_equal(L, "VR1")) return OutputList::VR1;
    if(if_equal(L, "VR2")) return OutputList::VR2;
    if(if_equal(L, "VR3")) return OutputList::VR3;
    if(if_equal(L, "A")) return OutputList::A;
    if(if_equal(L, "AT")) return OutputList::AT;
    if(if_equal(L, "AR")) return OutputList::AR;
    if(if_equal(L, "A1")) return OutputList::A1;
    if(if_equal(L, "A2")) return OutputList::A2;
    if(if_equal(L, "A3")) return OutputList::A3;
    if(if_equal(L, "AR1")) return OutputList::AR1;
    if(if_equal(L, "AR2")) return OutputList::AR2;
    if(if_equal(L, "AR3")) return OutputList::AR3;

    if(if_equal(L, "RF")) return OutputList::RF;
    if(if_equal(L, "RF1")) return OutputList::RF1;
    if(if_equal(L, "RF2")) return OutputList::RF2;
    if(if_equal(L, "RF3")) return OutputList::RF3;
    if(if_equal(L, "RM")) return OutputList::RM;
    if(if_equal(L, "RM1")) return OutputList::RM1;
    if(if_equal(L, "RM2")) return OutputList::RM2;
    if(if_equal(L, "RM3")) return OutputList::RM3;
    if(if_equal(L, "RT")) return OutputList::RT;

    if(if_equal(L, "NL")) return OutputList::NL;

    return OutputList::NL;
}
