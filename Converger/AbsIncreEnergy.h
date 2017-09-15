/**
 * @class AbsIncreEnergy
 * @brief The AbsIncreEnergy class handles converger test to indicate if the iteration converges.
 *
 * The criterion:
 * \f{gather}{
 * \big|\big|\Delta{}u\cdot{}\Delta{}r\big|\big|_2<E,
 * \f}
 * where \f$E\f$ is the tolerance.
 *
 * @author T
 * @date 16/09/2017
 * @version 0.1.0
 * @file AbsIncreEnergy.h
 * @addtogroup Converger
 * @{
 */

#ifndef ABSINCREENERGY_H
#define ABSINCREENERGY_H

#include "Converger.h"

class AbsIncreEnergy final : public Converger {
public:
    explicit AbsIncreEnergy(const unsigned& = 0, const shared_ptr<Domain>& = nullptr, const double& = 1E-8, const unsigned& = 7, const bool& = false);
    explicit AbsIncreEnergy(const shared_ptr<Domain>&, const double& = 1E-8, const unsigned& = 7, const bool& = false);

    const bool& if_converged() override;
};

#endif

//! @}
