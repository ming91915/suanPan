/**
 * @class integrationPlan
 * @brief An integrationPlan class.
 *
 * This is a simple class provides users the integration points coordinates and corresponding weights.
 *
 * Invoke the object with following definition:
 *
 *     integrationPlan TEST(intDimension, intOrder, intType);
 *
 * `intDimension` is the dimension of the problem.
 * `intOrder` is the integration order.
 * `intType` is the integration type. 1 is Gauss and 2 is Lobatto.
 *
 * The data is stored in `double** int_pts`.
 * The data can be acquired by using method `get_integration_scheme()`.
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
 * @date 05/09/2017
 * @version 0.1.0
 * @file integrationPlan.h
 * @addtogroup Utility
 * @{
 */

#ifndef INTEGRATIONPLAN_H
#define INTEGRATIONPLAN_H

class integrationPlan final {
    double** int_pts;

public:
    const unsigned n_rows;
    const unsigned n_cols;
    const unsigned n_elem;

    //! DEFAULT CTOR.
    explicit integrationPlan(const unsigned& = 2, const unsigned& = 2, const unsigned& = 1);
    //! DEFAULT DTOR.
    ~integrationPlan();

    //! GET POINTER TO THE DATA CHUCK
    double** get_integration_scheme() const;

    //! GET SINGLE ELEMENT
    double operator()(const unsigned&, const unsigned&) const;

    //! PRINT
    void print() const;
};

#endif

//! @}
