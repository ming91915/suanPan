/**
 * @class ODE_Solver
 * @brief A ODE_Solver class.
 *
 * The ODE_Solver object stores ODE system status and calls an ODE object
 * to get trial
 * status.
 *
 * @author T
 * @date 16/07/2017
 * @version 0.1.0
 * @file ODE_Solver.h
 * @addtogroup ODE_Solver
 * @{
 */

#ifndef ODE_SOLVER_H
#define ODE_SOLVER_H

#include <Domain/Factory.hpp>
#include <Solver/Solver.h>

class ODE;

class ODE_Solver : public Solver {
    shared_ptr<ODE> ode_system = nullptr;
    shared_ptr<Factory<double>> factory = nullptr;

public:
    explicit ODE_Solver(const unsigned& = 0, const unsigned& = CT_ODESOLVER, const shared_ptr<ODE>& = nullptr, const shared_ptr<Converger>& = nullptr, const shared_ptr<Factory<double>>& = nullptr);

    int initialize() override;

    int analyze(const unsigned& = 0) final;

    int update_status() override = 0;

    void set_ode(const shared_ptr<ODE>&);
    const shared_ptr<ODE>& get_ode() const;

    void set_factory(const shared_ptr<Factory<double>>&);
    const shared_ptr<Factory<double>>& get_factory() const;
};

#endif

//! @}
