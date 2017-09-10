/**
 * @class BS23
 * @brief A solver using Bogacki--Shampine 2/3 pair.

 * The Bogacki--Shampine order 2/order 3 embedded pair.
 * \f{align}{
 * s_1&=f\left(t_i,w_i\right)\\
 * s_2&=f\left(t_i+\dfrac{1}{2}h,w_i+\dfrac{1}{2}hs_1\right)\\
 * s_3&=f\left(t_i+\dfrac{3}{4}h,w_i+\dfrac{3}{4}hs_2\right)\\
 * z_{i+1}&=w_i+\dfrac{h}{9}\left(2s_1+3s_2+4s_3\right)\\
 * s_4&=f\left(t_i+h,z_{i+1}\right)\\
 * \f}
 *
 * @author T
 * @date 27/07/2017
 * @version 0.1.1
 * @file BS23.h
 * @addtogroup ODE_Solver
 * @{
 */

#ifndef BS23_H
#define BS23_H

#include "ODE_Solver.h"

class BS23 final : public ODE_Solver {
    vec S1, S2, S3, S4; /**< intermediate variables */

public:
    explicit BS23(const unsigned& = 0, const shared_ptr<ODE>& = nullptr, const shared_ptr<Converger>& = nullptr, const shared_ptr<Factory<double>>& = nullptr);
    explicit BS23(const shared_ptr<ODE>&, const shared_ptr<Converger>& = nullptr, const shared_ptr<Factory<double>>& = nullptr);

    int update_status() override;

    void print() override;
};

#endif

//! @}
