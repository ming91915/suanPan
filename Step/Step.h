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
class Converger;
class Integrator;

class Step : public Tag
{
    bool initialized = false;

    bool symm_mat = false;
    bool band_mat = false;

    double time_period = 1.0; /**< time period */

    shared_ptr<Workroom> factory = nullptr;
    shared_ptr<Domain> database = nullptr;
    shared_ptr<Solver> solver = nullptr;
    shared_ptr<Converger> tester = nullptr;
    shared_ptr<Integrator> modifier = nullptr;

public:
    explicit Step(const unsigned& = 0, const unsigned& = CT_STEP, const double& = 1.);
    virtual ~Step();

    const bool& is_initialized() const;

    virtual int initialize();

    virtual int analyze() = 0;

    void set_workroom(const shared_ptr<Workroom>&);
    const shared_ptr<Workroom>& get_workroom() const;

    void set_domain(const shared_ptr<Domain>&);
    const shared_ptr<Domain>& get_domain() const;

    void set_solver(const shared_ptr<Solver>&);
    const shared_ptr<Solver>& get_solver() const;

    void set_converger(const shared_ptr<Converger>&);
    const shared_ptr<Converger>& get_converger() const;

    void set_integrator(const shared_ptr<Integrator>&);
    const shared_ptr<Integrator>& get_integrator() const;

    void set_time_perid(const double&);
    const double& get_time_period() const;

    const bool& is_symm() const;
    const bool& is_band() const;

    void enable_symm();
    void disable_symm();

    void enable_band();
    void disable_band();
};

#endif
