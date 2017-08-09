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
 * @date 03/07/2017
 * @version 0.1.0
 * @file Newmark.h
 */

#ifndef NEWMARK_H
#define NEWMARK_H

#include "Solver.h"

class Newmark final : public Solver
{
    double alpha = .25; /**< parameter */
    double beta = .5;   /**< parameter */

    double DT = 0.; /**< previous incremental time */

    double C0 = 0., C1 = 0., C2 = 0., C3 = 0., C4 = 0., C5 = 0., C6 = 0.,
           C7 = 0.; /**< parameters */
public:
    explicit Newmark(const unsigned& = 0,
        const shared_ptr<Domain>& = nullptr,
        const shared_ptr<Convergence>& = nullptr,
        const double& = .25,
        const double& = .5);
    explicit Newmark(const shared_ptr<Domain>&,
        const shared_ptr<Convergence>& = nullptr,
        const double& = .25,
        const double& = .5);
    ~Newmark();

    int updateStatus() override;

    int analyze(const double&) override;

    void print() override;
};

#endif
