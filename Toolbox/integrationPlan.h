/**
 * @class integrationPlan
 * @brief An integrationPlan class.
 *
 * This is a simple class provides users the integration points coordinates
 * and corresponding weights.
 *
 * Invoke the object with following definition:
 *
 *     integrationPlan TEST(intDimension, intOrder, intType);
 *
 * `intDimension` is the dimension of the problem.
 * `intOrder` is the integration order.
 * `intType` is the integration type. 1 is Gauss and 2 is Lobatto.
 *
 * The data is stored in `double** intPts`.
 * The data can be acquired by using method `getIntPts()`.
 * Single element could be accessed by `(int, int)` operator.
 * The data is arranged as follows.
 *
 *     |-------------------------------------|
 *     | POINT |   COORDINATES   |   WEIGHT  |
 *     |-------|-----|-----|-----|-----|-----|  <---
 *     |   1   | X_1 | Y_1 | ... |  WEIGHT_1 |    |
 *     |   2   | X_2 | Y_2 | ... |  WEIGHT_2 |    |
 *     |   3   | X_3 | Y_3 | ... |  WEIGHT_3 |    |
 *     |   .   |  .  |  .  |  .  |     .     |    |---ROWS=intOrder^intDimension
 *     |   .   |  .  |  .  |  .  |     .     |    |
 *     |   .   |  .  |  .  |  .  |     .     |    |
 *     |   N   | X_N | Y_N | ... |  WEIGHT_N |    |
 *     |-------------------------------------|  <---
 *             ^                             ^
 *             |-----------------------------|
 *                            |
 *                   COLS=intDimension+1
 *
 * @author T
 * @date 03/07/2017
 * @file integrationPlan.h
 */

#ifndef INTEGRATIONPLAN_H
#define INTEGRATIONPLAN_H

#include <cmath>
#include <stdio.h>

class integrationPlan final
{
    double** intPts;
    unsigned num_row;
    unsigned num_col;

public:
    //! Default ctor.
    explicit integrationPlan(const unsigned& = 2,
        const unsigned& = 2,
        const unsigned& = 1);
    //! Default dtor.
    ~integrationPlan();

    //! GET NUMBER OF ROWS
    const unsigned& n_rows() const;
    //! GET NUMBER OF COLS
    const unsigned& n_cols() const;
    //! GET NUMBER OF ELEMENTS
    unsigned n_elem() const;

    //! GET POINTER TO THE DATA CHUCK
    double** getIntPts() const;

    //! GET SINGLE ELEMENT
    double operator()(const unsigned&, const unsigned&) const;

    //! PRINT
    void print() const;
};

#endif
