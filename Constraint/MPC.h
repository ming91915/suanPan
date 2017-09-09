/**
 * @class MPC
 * @brief A MPC class.
 *
 * @author T
 * @date 05/09/2017
 * @version 0.1.0
 * @file MPC.h
 * @addtogroup Constraint
 * @{
 */

#ifndef MPC_H
#define MPC_H

#include "Constraint.h"

class MPC : public Constraint {
public:
    explicit MPC(const unsigned& = 0, const unsigned& = CT_MPC, const unsigned& = 0);
    virtual ~MPC();
};

#endif

//! @}
