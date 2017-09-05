/**
* @class RelError
* @brief The RelError class that handles converger test to indicate if the iteration converges.
* @author T
* @date 08/08/2017
* @version 0.2.0
* @file RelError.h
* @addtogroup Converger
* @{
*/

#ifndef RELERROR_H
#define RELERROR_H

#include "Converger.h"

class RelError final : public Converger {
public:
    explicit RelError(const unsigned& = 0, const shared_ptr<Domain>& = nullptr, const double& = 1E-8, const unsigned& = 7, const bool& = false);
    explicit RelError(const shared_ptr<Domain>&, const double& = 1E-8, const unsigned& = 7, const bool& = false);

    const bool& if_converged() override;
};

#endif

//! @}
