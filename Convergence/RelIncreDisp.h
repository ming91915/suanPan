/**
* @class RelIncreDisp
* @brief The RelIncreDisp class that handles convergence test to indicate if the iteration
* converges.
* @author T
* @date 08/08/2017
* @version 0.2.0
* @file RelIncreDisp.h
* @addtogroup Convergence
* @{
*/

#ifndef RELINCREDISP_H
#define RELINCREDISP_H

#include "Convergence.h"

class RelIncreDisp : public Convergence
{
public:
    explicit RelIncreDisp(const unsigned& = 0,
        const shared_ptr<Domain>& = nullptr,
        const double& = 1E-8,
        const bool& = false);
    explicit RelIncreDisp(const shared_ptr<Domain>&,
        const double& = 1E-8,
        const bool& = false);

    const bool& if_converged() override;
};

#endif

//! @}
