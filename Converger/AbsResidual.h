/**
 * @class AbsResidual
 * @brief The Converger class handles converger test to indicate if the iteration converges according to various rules.
 *
 * The class stores a pointer `factory` pointed to the Workshop and get information from this Workroom. The `tolerance` and `error` are stored independently so that the Workshop will not be modified.
 *
 * The class further provides a `print_flag` to indicate if the test information should be printed out.
 *
 * @author T
 * @date 08/08/2017
 * @version 0.2.0
 * @file AbsResidual.h
 * @addtogroup Converger
 * @{
 */

#ifndef ABSRESIDUAL_H
#define ABSRESIDUAL_H

#include "Converger.h"

class DomainBase;

class AbsResidual : public Converger {
public:
    explicit AbsResidual(const unsigned& = 0, const shared_ptr<DomainBase>& = nullptr, const double& = 1E-8, const unsigned& = 7, const bool& = false);
    explicit AbsResidual(const shared_ptr<DomainBase>&, const double& = 1E-8, const unsigned& = 7, const bool& = false);
    explicit AbsResidual(const double&, const unsigned& = 7, const bool& = false);

    const bool& if_converged() override;
};

#endif

//! @}
