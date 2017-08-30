/**
* @class RK45
* @brief A solver using Runge--Kutta 4/5 solver.
*
* The Runge--Kutta--Fehlberg order 4/order 5 embedded pair.
* \f{align}{
* s_1&=f\left(t_i,w_i\right)\\
* s_2&=f\left(t_i+\dfrac{1}{4}h,w_i+\dfrac{1}{4}hs_1\right)\\
* s_3&=f\left(t_i+\dfrac{3}{8}h,w_i+\dfrac{3}{32}hs_1+\dfrac{9}{32}hs_2\right)\\
* s_4&=f\left(t_i+\dfrac{12}{13}h,w_i+\dfrac{1932}{2197}hs_1-\dfrac{7200}{2197}hs_2+\dfrac{7296}{2197}hs_3\right)\\
* s_5&=f\left(t_i+h,w_i+\dfrac{439}{216}hs_1-8hs_2+\dfrac{3680}{513}hs_3-\dfrac{845}{4104}hs_4\right)\\
* s_6&=f\left(t_i+\dfrac{1}{2}h,w_i-\dfrac{8}{27}hs_1+2hs_2-\dfrac{3544}{2565}hs_3+\dfrac{1859}{4104}hs_4-\dfrac{11}{40}hs_5\right)\\
* z_{i+1}&=w_i+h\left(\dfrac{16}{135}s_1+\dfrac{6656}{12825}s_3+\dfrac{28561}{56430}s_4-\dfrac{9}{50}s_5+\dfrac{2}{55}s_6\right)
* \f}
*
* @author T
* @date 22/08/2017
* @version 0.2.1
* @file RK45.h
*/

#ifndef RK45_H
#define RK45_H

#include "ODE_Solver.h"

class RK45 final : public ODE_Solver
{
    vec S1, S2, S3, S4, S5, S6; /**< intermediate variables */
public:
    explicit RK45(const unsigned& = 0,
        const shared_ptr<ODE>& = nullptr,
        const shared_ptr<Converger>& = nullptr,
        const shared_ptr<Workshop>& = nullptr);
    explicit RK45(const shared_ptr<ODE>&,
        const shared_ptr<Converger>& = nullptr,
        const shared_ptr<Workshop>& = nullptr);
    ~RK45();

    int update_status() override;

    void print() override;
};

#endif
