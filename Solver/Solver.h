/**
 * @class Solver
 * @brief A Solver class defines solvers used in analysis.
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

class Solver : public Tag {
    bool initialized = false;

    shared_ptr<Converger> converger = nullptr;
    shared_ptr<Integrator> modifier = nullptr;

public:
    explicit Solver(const unsigned& = 0, const unsigned& = CT_SOLVER, const shared_ptr<Converger>& = nullptr, const shared_ptr<Integrator>& = nullptr);
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
