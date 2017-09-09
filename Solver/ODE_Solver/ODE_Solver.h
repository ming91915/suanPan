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

#include <Solver/Solver.h>

class ODE;
class Workshop;

class ODE_Solver : public Solver {
    shared_ptr<ODE> ode_system = nullptr;
    shared_ptr<Workshop> factory = nullptr;

public:
    explicit ODE_Solver(const unsigned& = 0, const unsigned& = CT_ODESOLVER, const shared_ptr<ODE>& = nullptr, const shared_ptr<Converger>& = nullptr, const shared_ptr<Workshop>& = nullptr);
    virtual ~ODE_Solver();

    int initialize() override;

    int analyze(const unsigned& = 1.) override final;

    int update_status() override = 0;

    void set_ode(const shared_ptr<ODE>&);
    const shared_ptr<ODE>& get_ode() const;

    void set_workroom(const shared_ptr<Workshop>&);
    const shared_ptr<Workshop>& get_workroom() const;
};

#endif

//! @}
