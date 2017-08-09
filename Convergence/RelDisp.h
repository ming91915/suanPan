/**
* @class RelDisp
* @brief The RelDisp class that handles convergence test to indicate if the iteration
* converges.
* @author T
* @date 08/08/2017
* @version 0.2.0
* @file RelDisp.h
* @addtogroup Convergence
* @{
*/

#ifndef RELDISP_H
#define RELDISP_H

#include "Convergence.h"

class RelDisp final : public Convergence
{
public:
    explicit RelDisp(const unsigned& = 0,
        const shared_ptr<Domain>& = nullptr,
        const double& = 1E-8,
        const bool& = false);
    explicit RelDisp(const shared_ptr<Domain>&,
        const double& = 1E-8,
        const bool& = false);

    const bool& if_converged() override;
};

#endif

//! @}
