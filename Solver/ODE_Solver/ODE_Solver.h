/**
 * @class ODE_Solver
 * @brief A ODE_Solver class.
 *
 * The ODE_Solver object stores ODE system status and calls an ODE object to get trial
 * status.
 *
 * @author T
 * @date 16/07/2017
 * @version 0.1.0
 * @file ODE_Solver.h
 */

#ifndef ODE_SOLVER_H
#define ODE_SOLVER_H

#include <Solver/Solver.h>

class ODE;
class Workroom;

class ODE_Solver : public Solver
{
    shared_ptr<ODE> ode_system = nullptr;   /**<  */
    shared_ptr<Workroom> factory = nullptr; /**<  */
public:
    explicit ODE_Solver(const unsigned& = 0,
        const unsigned& = CT_ODESOLVER,
        const shared_ptr<ODE>& = nullptr,
        const shared_ptr<Convergence>& = nullptr,
        const shared_ptr<Workroom>& = nullptr);
    virtual ~ODE_Solver();

    void initialize() override;

    int analyze(const double& = 1.) override final;

    int update_status() override = 0;

    void setODE(const shared_ptr<ODE>&);
    const shared_ptr<ODE>& getODE() const;

    void setWorkroom(const shared_ptr<Workroom>);
    const shared_ptr<Workroom>& getWorkroom() const;
};

#endif
