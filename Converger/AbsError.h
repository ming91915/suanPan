/**
* @class AbsError
* @brief The AbsError class that handles converger test to indicate if the iteration
* converges.
*
* The criterion:
* \f{gather}{
* e<E,
* \f}
* where \f$E\f$ is the tolerance and \f$e\f$ is the error provided by the Workshop.
*
* @author T
* @date 08/08/2017
* @version 0.2.0
* @file AbsError.h
* @addtogroup Converger
* @{
*/

#ifndef ABSERROR_H
#define ABSERROR_H

#include "Converger.h"

class AbsError final : public Converger
{
public:
    explicit AbsError(const unsigned& = 0,
        const shared_ptr<Domain>& = nullptr,
        const double& = 1E-8,
        const unsigned& = 7,
        const bool& = false);
    explicit AbsError(const shared_ptr<Domain>&,
        const double& = 1E-8,
        const unsigned& = 7,
        const bool& = false);

    const bool& if_converged() override;
};

#endif

//! @}
