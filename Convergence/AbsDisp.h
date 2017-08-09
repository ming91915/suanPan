/**
* @class AbsDisp
* @brief The AbsDisp class handles convergence test to indicate if the iteration
* converges.
*
* The criterion:
* \f{gather}{
* \big|\big|\Delta{}u\big|\big|_2<E,
* \f}
* where \f$E\f$ is the tolerance.
*
* @author T
* @date 08/08/2017
* @version 0.2.0
* @file AbsDisp.h
* @addtogroup Convergence
* @{
*/

#ifndef ABSDISP_H
#define ABSDISP_H

#include "Convergence.h"

class AbsDisp final : public Convergence
{
public:
    explicit AbsDisp(const unsigned& = 0,
        const shared_ptr<Domain>& = nullptr,
        const double& = 1E-8,
        const bool& = false);
    explicit AbsDisp(const shared_ptr<Domain>&,
        const double& = 1E-8,
        const bool& = false);

    const bool& if_converged() override;
};

#endif

//! @}
