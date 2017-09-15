/**
 * @class AbsDisp
 * @brief The AbsDisp class handles converger test to indicate if the iteration converges.
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
 * @addtogroup Converger
 * @{
 */

#ifndef ABSDISP_H
#define ABSDISP_H

#include "Converger.h"

class AbsDisp final : public Converger {
public:
    explicit AbsDisp(const unsigned& = 0, const shared_ptr<DomainBase>& = nullptr, const double& = 1E-8, const unsigned& = 7, const bool& = false);
    explicit AbsDisp(const shared_ptr<DomainBase>&, const double& = 1E-8, const unsigned& = 7, const bool& = false);

    const bool& if_converged() override;
};

#endif

//! @}
