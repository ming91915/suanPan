/*******************************************************************************
 * Copyright (C) 2017 Theodore Chang
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/
/**
 * @class ODE
 * @brief An ODE class in charge of defining an ODE system.
 *
 * An ordinary differential equation (ODE) (system) is normally expressed as
 * \f{gather}{
 * y'=f\left(t,y\right),
 * \f}
 * where \f$t\f$ is the generalized time which is always a scalar, \f$y\f$ are independent variables and \f$y'\f$ are the corresponding derivatives. There could be only one independent variable, or more often, multiple variables. The key method is
 *
 *     const vec& eval(const double&T, const vec&Y)
 *
 * which returns the left hand side \f$y'\f$. This method is used in ODE_Solver.
 *
 * \f$y\f$ (const vec& Y) is a vector even if there is only one independent variable. If the ODE system is linear, one could conveniently rewrite it in a matrix form and then define a matrix to calculate the derivatives.
 *
 * For example, the following ODE system
 * \f{gather}{
 * x'=\begin{bmatrix}1&3\\5&3\end{bmatrix}x
 * \f}
 * should be defined as
 *
 *     class ODE_INSTANCE : public ODE
 *     {
 *         ODE_INSTANCE() : ODE(0, 0, 2) {}
 *         vec eval(const double& T, const vec& Y)
 *         {
 *             mat A = { { 1, 3 }, { 5, 3 } };
 *             return A * Y;
 *         }
 *     };
 *
 * For non-linear system, element-wise computation should be used.
 *
 * @author T
 * @date 20/08/2017
 * @version 0.1.1
 * @file ODE.h
 */

#ifndef ODE_H
#define ODE_H

#include <Domain/Tag.h>

class ODE : public Tag {
    unsigned n_size; /**< The dimension of the problem. */
public:
    //! Default ctor.
    explicit ODE(const unsigned& = 0, const unsigned& = CT_ODE, const unsigned& = 1);

    //! Method to set dimension of the problem.
    virtual void set_dimension(const unsigned&);
    virtual void operator()(const unsigned&);
    //! Method to return dimension of the problem, will be used to determine the size of input vector.
    virtual const unsigned& get_dimension() const;
    virtual unsigned operator()();

    //! Override the base class's print function.
    void print() override;

    //! Method to return LHS --- the derivatives.
    virtual vec eval(const double&, const vec&) = 0;
    virtual vec operator()(const double&, const vec&) = 0;
};

#endif
