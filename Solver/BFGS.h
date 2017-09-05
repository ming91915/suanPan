/**
* @class BFGS
* @brief The BFGS class defines a solver using BFGS iteration method.
*
* The BFGS method is a rank two method which tends to converge faster
* than the
* conventional Newton--Raphson method. The BFGS method solves the
* inverse of the stiffness
* matrix only once for each iterative step. But the sparsity is normally
* lost for the
* inverse matrix. Hence the storage is significantly higher than
* Newton--Raphson method.
* Hence for large scale problem, in which the RAM is a problem, probably
* the user shall
* choose other methods.
*
* \f{gather}{
* K_{n+1}^{-1}=\left(I-\dfrac{\Delta{}UR^T}{R^T\Delta{}U}\right)K_n^{-1}\left(I-\dfrac{R\Delta{}U^T}{R^T\Delta{}U}\right)+\dfrac{\Delta{}U\Delta{}U^T}{R^T\Delta{}U}.
* \f}
*
* The \f$I\f$ is identity matrix. The \f$\Delta{}U\f$ is current
* displacement increment.
* The \f$R\f$ is current residual. For brevity, in both terms, the
* subscript \f$n\f$
* representing current step is dropped.
* @author T
* @date 27/07/2017
* @version 0.1.0
* @file BFGS.h
*/

#ifndef BFGS_H
#define BFGS_H

#include "Solver.h"

class BFGS : public Solver {
public:
    explicit BFGS(const unsigned& = 0, const shared_ptr<Converger>& = nullptr, const shared_ptr<Integrator>& = nullptr);
    explicit BFGS(const shared_ptr<Converger>& = nullptr, const shared_ptr<Integrator>& = nullptr);

    int initialize() override;

    int update_status() override;

    int analyze(const unsigned&) override;

    void print() override;
};

#endif
