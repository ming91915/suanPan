#include "OutputList.h"
#include <string.h>

const char* to_char(const OutputList& L)
{
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

OutputList to_list(const char* L)
{
    if(_strcmpi(L, "S") == 0) return OutputList::S;
    if(_strcmpi(L, "S11") == 0) return OutputList::S11;
    if(_strcmpi(L, "S22") == 0) return OutputList::S22;
    if(_strcmpi(L, "S33") == 0) return OutputList::S33;
    if(_strcmpi(L, "S12") == 0) return OutputList::S12;
    if(_strcmpi(L, "S23") == 0) return OutputList::S23;
    if(_strcmpi(L, "S13") == 0) return OutputList::S13;
    if(_strcmpi(L, "E") == 0) return OutputList::E;
    if(_strcmpi(L, "E11") == 0) return OutputList::E11;
    if(_strcmpi(L, "E22") == 0) return OutputList::E22;
    if(_strcmpi(L, "E33") == 0) return OutputList::E33;
    if(_strcmpi(L, "E12") == 0) return OutputList::E12;
    if(_strcmpi(L, "E23") == 0) return OutputList::E23;
    if(_strcmpi(L, "E13") == 0) return OutputList::E13;
    if(_strcmpi(L, "SP") == 0) return OutputList::SP;
    if(_strcmpi(L, "SP1") == 0) return OutputList::SP1;
    if(_strcmpi(L, "SP2") == 0) return OutputList::SP2;
    if(_strcmpi(L, "SP3") == 0) return OutputList::SP3;
    if(_strcmpi(L, "EP") == 0) return OutputList::EP;
    if(_strcmpi(L, "EP1") == 0) return OutputList::EP1;
    if(_strcmpi(L, "EP2") == 0) return OutputList::EP2;
    if(_strcmpi(L, "EP3") == 0) return OutputList::EP3;
    if(_strcmpi(L, "SINV") == 0) return OutputList::SINV;
    if(_strcmpi(L, "MISES") == 0) return OutputList::MISES;
    if(_strcmpi(L, "TRESC") == 0) return OutputList::TRESC;

    if(_strcmpi(L, "PE") == 0) return OutputList::PE;
    if(_strcmpi(L, "PE11") == 0) return OutputList::PE11;
    if(_strcmpi(L, "PE22") == 0) return OutputList::PE22;
    if(_strcmpi(L, "PE33") == 0) return OutputList::PE33;
    if(_strcmpi(L, "PE12") == 0) return OutputList::PE12;
    if(_strcmpi(L, "PE23") == 0) return OutputList::PE23;
    if(_strcmpi(L, "PE13") == 0) return OutputList::PE13;
    if(_strcmpi(L, "PEEQ") == 0) return OutputList::PEEQ;

    if(_strcmpi(L, "U") == 0) return OutputList::U;
    if(_strcmpi(L, "UT") == 0) return OutputList::UT;
    if(_strcmpi(L, "UR") == 0) return OutputList::UR;
    if(_strcmpi(L, "U1") == 0) return OutputList::U1;
    if(_strcmpi(L, "U2") == 0) return OutputList::U2;
    if(_strcmpi(L, "U3") == 0) return OutputList::U3;
    if(_strcmpi(L, "UR1") == 0) return OutputList::UR1;
    if(_strcmpi(L, "UR2") == 0) return OutputList::UR2;
    if(_strcmpi(L, "UR3") == 0) return OutputList::UR3;
    if(_strcmpi(L, "V") == 0) return OutputList::V;
    if(_strcmpi(L, "VT") == 0) return OutputList::VT;
    if(_strcmpi(L, "VR") == 0) return OutputList::VR;
    if(_strcmpi(L, "V1") == 0) return OutputList::V1;
    if(_strcmpi(L, "V2") == 0) return OutputList::V2;
    if(_strcmpi(L, "V3") == 0) return OutputList::V3;
    if(_strcmpi(L, "VR1") == 0) return OutputList::VR1;
    if(_strcmpi(L, "VR2") == 0) return OutputList::VR2;
    if(_strcmpi(L, "VR3") == 0) return OutputList::VR3;
    if(_strcmpi(L, "A") == 0) return OutputList::A;
    if(_strcmpi(L, "AT") == 0) return OutputList::AT;
    if(_strcmpi(L, "AR") == 0) return OutputList::AR;
    if(_strcmpi(L, "A1") == 0) return OutputList::A1;
    if(_strcmpi(L, "A2") == 0) return OutputList::A2;
    if(_strcmpi(L, "A3") == 0) return OutputList::A3;
    if(_strcmpi(L, "AR1") == 0) return OutputList::AR1;
    if(_strcmpi(L, "AR2") == 0) return OutputList::AR2;
    if(_strcmpi(L, "AR3") == 0) return OutputList::AR3;

    if(_strcmpi(L, "RF") == 0) return OutputList::RF;
    if(_strcmpi(L, "RF1") == 0) return OutputList::RF1;
    if(_strcmpi(L, "RF2") == 0) return OutputList::RF2;
    if(_strcmpi(L, "RF3") == 0) return OutputList::RF3;
    if(_strcmpi(L, "RM") == 0) return OutputList::RM;
    if(_strcmpi(L, "RM1") == 0) return OutputList::RM1;
    if(_strcmpi(L, "RM2") == 0) return OutputList::RM2;
    if(_strcmpi(L, "RM3") == 0) return OutputList::RM3;
    if(_strcmpi(L, "RT") == 0) return OutputList::RT;

    if(_strcmpi(L, "NL") == 0) return OutputList::NL;

    return OutputList::NL;
}
