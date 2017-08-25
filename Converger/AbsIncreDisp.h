/**
* @class AbsIncreDisp
* @brief The AbsIncreDisp class that handles converger test to indicate if the iteration
* converges.
* @author T
* @date 27/07/2017
* @version 0.1.0
* @file AbsIncreDisp.h
* @addtogroup Converger
* @{
*/

#ifndef ABSINCREDISP_H
#define ABSINCREDISP_H

#include "Converger.h"

class AbsIncreDisp : public Converger
{
public:
    explicit AbsIncreDisp(const unsigned& = 0,
        const shared_ptr<Domain>& = nullptr,
        const double& = 1E-8,
        const unsigned& = 7,
        const bool& = false);
    explicit AbsIncreDisp(const shared_ptr<Domain>&,
        const double& = 1E-8,
        const unsigned& = 7,
        const bool& = false);

    const bool& if_converged() override;
};

#endif

//! @}
