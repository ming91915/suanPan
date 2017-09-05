/**
 * @class Amplitude
 * @brief An Amplitude class that can generate Amplitude pattern.
 *
 * Similar to the strategy used in ABAQUS, the load pattern and the magnitude of that load can be separated. The Amplitude class is designed to generate different Amplitude patterns. The time and amplitude are stored in two different vectors: `TIME` and `AMP`. By calling the method
 *
 *     double get_amplitude(const double& NEW_TIME_POINT);
 *
 * The Amplitude is interpolated. If the given `NEW_TIME_POINT` is beyond the right bound, the Amplitude will be set to the maximum one. Such a case may happen to the auto step size control scheme, in which the (pseudo) time may exceed the given bound due to the machine error.
 *
 * Currently, there are five predefined amplitude patterns.
 * - Tabular
 * - Linear/Ramp
 *   \f{gather}{a=A_0+A\left(t-t_0\right)/t_s\f}
 * - Periodic
 *   \f{gather}{a=A_0+\sum_{n=1}^N\left(A_n\cos{}n\omega\left(t-t_0\right)+B_n\sin{}n\omega\left(t-t_0\right)\right)\f}
 * - Modulated
 *   \f{gather}{a=A_0+A\sin\omega_1\left(t-t_0\right)\sin\omega_2\left(t-t_0\right)\f}
 * - Decay
 *   \f{gather}{a=A_0+A\exp\left(-\left(t-t_0\right)/t_d\right)\f}
 *
 * @author T
 * @date 03/07/2017
 * @file Amplitude.h
 * @addtogroup Amplitude
 * @{
 */

#ifndef AMPLITUDE_H
#define AMPLITUDE_H

enum class AmplitudeType { RAMP, TABULAR, PERIODIC, MODULATED, DECAY };

#include <Domain/Tag.h>

class Amplitude : public Tag {
public:
    explicit Amplitude(const unsigned& = 0, const unsigned& = CT_AMPLITUDE);
    virtual ~Amplitude();

    virtual double get_amplitude(const double&);

    void print() override;
};

#endif

//! @}
