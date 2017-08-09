/**
* @class MPC
* @brief A MPC class.
*
* @author T
* @date 27/07/2017
* @file MPC.h
*/

#ifndef MPC_H
#define MPC_H

#include "Constraint.h"

class MPC : public Constraint
{
public:
    explicit MPC(const unsigned& = 0, const unsigned& = CT_MPC, const unsigned& = 0);
    virtual ~MPC();
};

#endif
