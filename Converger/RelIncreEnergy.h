/**
 * @class RelIncreEnergy
 * @brief The RelIncreEnergy class handles converger test to indicate if the iteration converges.
 *
 * The criterion:
 * \f{gather}{
 * \big|\big|\Delta{}u\cdot{}\Delta{}r\big|\big|_2<E,
 * \f}
 * where \f$E\f$ is the tolerance.
 *
 * @author T
 * @date 08/08/2017
 * @version 0.2.0
 * @file RelIncreEnergy.h
 * @addtogroup Converger
 * @{
 */

#ifndef RELINCREENERGY_H
#define RELINCREENERGY_H

#include "Converger.h"

class RelIncreEnergy final : public Converger {
public:
    explicit RelIncreEnergy(const unsigned& = 0, const shared_ptr<Domain>& = nullptr, const double& = 1E-8, const unsigned& = 7, const bool& = false);
    explicit RelIncreEnergy(const shared_ptr<Domain>&, const double& = 1E-8, const unsigned& = 7, const bool& = false);

    const bool& if_converged() override;
};

#endif

//! @}
