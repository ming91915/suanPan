/**
* @class RelResidual
* @brief The Converger class handles converger test to indicate if the iteration
* converges according to various rules.
*
* The class stores a pointer `factory` pointed to the Workroom and get information from
* this Workroom. The `tolerance` and `error` are stored independently so that the
* Workroom will not be modified.
*
* The class further provides a `print_flag` to indicate if the test information should be
* printed out.
*
* @author T
* @date 27/08/2017
* @version 0.1.0
* @file RelResidual.h
* @addtogroup Converger
* @{
*/

#ifndef RELRESIDUAL_H
#define RELRESIDUAL_H

#include "Converger.h"

class Domain;

class RelResidual : public Converger
{
public:
    explicit RelResidual(const unsigned& = 0,
        const shared_ptr<Domain>& = nullptr,
        const double& = 1E-8,
        const unsigned& = 7,
        const bool& = false);
    explicit RelResidual(const shared_ptr<Domain>&,
        const double& = 1E-8,
        const unsigned& = 7,
        const bool& = false);
    explicit RelResidual(const double&, const unsigned& = 7, const bool& = false);

    const bool& if_converged() override;
};

#endif

//! @}
