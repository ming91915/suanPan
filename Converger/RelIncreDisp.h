/**
* @class RelIncreDisp
* @brief The RelIncreDisp class that handles converger test to indicate if the iteration converges.
* @author T
* @date 08/08/2017
* @version 0.2.0
* @file RelIncreDisp.h
* @addtogroup Converger
* @{
*/

#ifndef RELINCREDISP_H
#define RELINCREDISP_H

#include "Converger.h"

class RelIncreDisp : public Converger {
public:
    explicit RelIncreDisp(const unsigned& = 0, const shared_ptr<Domain>& = nullptr, const double& = 1E-8, const unsigned& = 7, const bool& = false);
    explicit RelIncreDisp(const shared_ptr<Domain>&, const double& = 1E-8, const unsigned& = 7, const bool& = false);

    const bool& if_converged() override;
};

#endif

//! @}
