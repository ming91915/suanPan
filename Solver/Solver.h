/**
 * @class Solver
 * @brief A Solver class defines solvers used in analysis.
 *
 * As any Solver needs to compute the inverse of a given matrix, which is handle by
 * `solve()` (calls LAPACK) and `spsolve()` (calls SuperLU) depending on different matrix
 * types and Solver configurations, provided by `Armadillo`.
 *
 * By default, Workroom uses sparse matrix for storing data. Correspondingly, Solver uses
 * `spsolve()` for solving the system. The `spsolve()` provides two type flags, which can
 * convert the sparse matrix to dense matrix then call LAPACK subroutines to compute the
 * inverse. This can be controlled by the flag `dense_mat`.
 *
 * Permutation is important for solving speed. Bad permutation may lead to the failure of
 * solving. Two schemes are available: RCM and COLAMD. RCM algorithm is provided and used
 * in Domain object to reorder the DoF indices. So by default, the system is reordered
 * according to RCM algorithm before being sent to Solver. COLAMD algorithm is provided by
 * `SuperLU` library and can be invoked as a parameter when calling `spsolve()` method.
 * Two algorithms lead to different performance for LU decomposition.
 *
 * We will mainly deal with banded matrices so the following tips are tested only for
 * banded matrices:
 * - Sparse matrix is always preferred for all kinds of problems of various sizes with
 * different permutation or even without permutation.
 * - For dense matrix, permutations do not guarantee an improvement about the LU
 * decomposition speed. Sometimes directly solving the original matrix will be faster.
 * - For small problems with sparse storage, RCM seems to be slightly faster than COLAMD.
 * - For large problems with sparse storage, COLAMD is the known best permutation
 * algorithm.
 *
 * So for application, we shall always use sparse storage and call corresponding SuperLU
 * Solver. As RCM is applied by default in Domain and COLAMD is used by default in Solver,
 * for large problems, we do not have to do anything just use default settings. For small
 * problems, we could set `permutation_type=NATURAL` to avoid calling COLAMD every single
 * time, which may cost a significant percentage of the running time.
 *
 * @author T
 * @date 27/07/2017
 * @version 0.2.1
 * @file Solver.h
 * @addtogroup Solver
 * @{
 */

#ifndef SOLVER_H
#define SOLVER_H

#include <Domain/Tag.h>

class Converger;
class Integrator;

class Solver : public Tag
{
    bool initialized = false;

    shared_ptr<Converger> converger = nullptr;
    shared_ptr<Integrator> modifier = nullptr;

public:
    explicit Solver(const unsigned& = 0,
        const unsigned& = CT_SOLVER,
        const shared_ptr<Converger>& = nullptr,
        const shared_ptr<Integrator>& = nullptr);
    virtual ~Solver();

    const bool& is_initialized() const;

    virtual int initialize();

    virtual int update_status() = 0;

    virtual int analyze(const unsigned& = 0) = 0;

    void set_converger(const shared_ptr<Converger>&);
    const shared_ptr<Converger>& get_converger() const;

    void set_integrator(const shared_ptr<Integrator>&);
    const shared_ptr<Integrator>& get_integrator() const;
};

#endif

//! @}
