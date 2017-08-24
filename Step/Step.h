/**
 * @class Step
 * @brief A Step class.
 * @author T
 * @date 25/08/2017
 * @version 0.2.0
 * @file Step.h
 */

#ifndef STEP_H
#define STEP_H

#include <Domain/Tag.h>

class Workroom;
class Domain;
class Solver;
class Convergence;

class Step : public Tag
{
    bool band_mat = false;
    bool symm_mat = false;

    double time_period = 1.0; /**< time period */

    shared_ptr<Workroom> factory;
    shared_ptr<Domain> database;
    shared_ptr<Solver> solver;
    shared_ptr<Convergence> converger;

public:
    explicit Step(const unsigned& = 0,
        const unsigned& = CT_STEP,
        const shared_ptr<Domain>& = nullptr,
        const shared_ptr<Solver>& = nullptr,
        const shared_ptr<Convergence>& = nullptr,
        const double& = 1.);
    virtual ~Step() {}

    virtual int initialize();

    virtual int analyze() = 0;

    void set_workroom(const shared_ptr<Workroom>&);
    const shared_ptr<Workroom>& get_workroom() const;

    void set_domain(const shared_ptr<Domain>&);
    const shared_ptr<Domain>& get_domain() const;

    void set_solver(const shared_ptr<Solver>&);
    const shared_ptr<Solver>& get_solver() const;

    void set_convergence(const shared_ptr<Convergence>&);
    const shared_ptr<Convergence>& get_convergence() const;

    void set_time_perid(const double&);
    const double& get_time_period() const;

    void enable_band();
    void disable_band();
    void enable_symm();
    void disable_symm();
};

#endif
