/**
 * @class RK23
 * @brief A solver using Runge--Kutta 2/3 solver.
 *
 * The Runge--Kutta order 2/order 3 embedded pair.
 * \f{align}{
 * s_1&=f\left(t_i,w_i\right)\\
 * s_2&=f\left(t_i+h,w_i+hs_1\right)\\
 * s_3&=f\left(t_i+\dfrac{1}{2}h,w_i+\dfrac{1}{4}hs_1+\dfrac{1}{4}hs_2\right)\\
 * z_{i+1}&=w_i+\dfrac{h}{6}\left(s_1+s_2+4s_3\right)\\
 * \f}
 *
 * @author T
 * @date 27/07/2017
 * @version 0.1.1
 * @file RK23.h
 */

#ifndef RK23_H
#define RK23_H

#include "ODE_Solver.h"

class RK23 final : public ODE_Solver {
    vec S1, S2, S3; /**< intermediate variables */
public:
    explicit RK23(const unsigned& = 0, const shared_ptr<ODE>& = nullptr, const shared_ptr<Converger>& = nullptr, const shared_ptr<Workshop>& = nullptr);
    explicit RK23(const shared_ptr<ODE>&, const shared_ptr<Converger>& = nullptr, const shared_ptr<Workshop>& = nullptr);
    ~RK23();

    int update_status() override;

    void print() override;
};

#endif
