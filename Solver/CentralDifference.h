/**
* @class CentralDifference
* @brief A CentralDifference class defines a solver using Newmark algorithm.
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
* @date 29/07/2017
* @version 0.1.0
* @file CentralDifference.h
*/

#ifndef CENTRALDIFFERENCE_H
#define CENTRALDIFFERENCE_H

#include "Solver.h"

class CentralDifference : public Solver
{
    double max_dt = 0.;

    double DT = 0.;

    double C0 = 0., C1 = 0., C2 = 0., C3 = 0.;

public:
    explicit CentralDifference(const unsigned& = 0, const shared_ptr<Domain>& = nullptr);
    explicit CentralDifference(const shared_ptr<Domain>&);

    int update_status() override;

    int analyze(const unsigned&) override;
};

#endif
