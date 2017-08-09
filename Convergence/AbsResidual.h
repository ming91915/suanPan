/**
* @class AbsResidual
* @brief The Convergence class handles convergence test to indicate if the iteration
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
* @date 08/08/2017
* @version 0.2.0
* @file AbsResidual.h
* @addtogroup Convergence
* @{
*/

#ifndef ABSRESIDUAL_H
#define ABSRESIDUAL_H

#include "Convergence.h"

class Domain;

class AbsResidual : public Convergence
{
public:
    explicit AbsResidual(const unsigned& = 0,
        const shared_ptr<Domain>& = nullptr,
        const double& = 1E-8,
        const bool& = false);
    explicit AbsResidual(const shared_ptr<Domain>&,
        const double& = 1E-8,
        const bool& = false);
    explicit AbsResidual(const double&, const bool& = false);

    const bool& if_converged() override;
};

#endif

//! @}
