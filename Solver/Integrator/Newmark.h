/**
* @class Newmark
* @brief A Newmark class defines a solver using Newmark algorithm.
*
* `Newmark` algorithm is unconditionally stable if
* \f{gather}{\alpha\geq\dfrac{1}{4}\left(\dfrac{1}{2}+\beta\right)^2,\qquad\beta\geq\dfrac{1}{2}\f}.
*
* There are several choices for solver parameters.
*
* Constant Acceleration:
* \f{gather}{\alpha=\dfrac{1}{4},\qquad\beta=\dfrac{1}{2}\f}.
*
* Linear Acceleration:
* \f{gather}{\alpha=\dfrac{1}{6},\qquad\beta=\dfrac{1}{2}\f}.
*
* @author T
* @date 25/08/2017
* @version 0.1.1
* @file Newmark.h
*/

#ifndef NEWMARK_H
#define NEWMARK_H

#include "Integrator.h"

class Newmark final : public Integrator {
    const double alpha; /**< parameter = .25 */
    const double beta;  /**< parameter = .5 */

    double DT = 0.; /**< previous incremental time */

    double C0 = 0., C1 = 0., C2 = 0., C3 = 0., C4 = 0., C5 = 0., C6 = 0., C7 = 0.; /**< parameters */

    void update_parameter();

public:
    explicit Newmark(const unsigned& = 0, const shared_ptr<Domain>& = nullptr, const double& = .25, const double& = .5);
    explicit Newmark(const shared_ptr<Domain>&, const double& = .25, const double& = .5);
    ~Newmark();

    int initialize() override;

    void update_resistance() override;
    void update_stiffness() override;

    void commit_status() const override;

    void print() override;
};

#endif
