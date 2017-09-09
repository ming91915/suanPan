/**
 * @class Step
 * @brief A Step class.
 * @author T
 * @date 27/08/2017
 * @version 0.2.1
 * @file Step.h
 * @addtogroup Step
 * @{
 */

#ifndef STEP_H
#define STEP_H

#include <Domain/Factory.hpp>
#include <Domain/Tag.h>

class Workshop;
class Domain;
class Solver;
class Converger;
class Integrator;

class Step : public Tag {
    bool updated = false;

    bool symm_mat = true;
    bool band_mat = true;

    double time_period = 1.0; /**< time period */

    double max_step_size = time_period; /**< maximum step size */
    double min_step_size = 1E-8;        /**< minimum step size */

    double ini_step_size = time_period; /**< initial step size */

    unsigned max_increment = 100; /**< maximum increment number */

    bool fixed_step_size = false; /**< auto-stepping */

    shared_ptr<Factory<double>> factory = nullptr;
    shared_ptr<Workshop> workroom = nullptr;
    shared_ptr<Domain> database = nullptr;
    shared_ptr<Solver> solver = nullptr;
    shared_ptr<Converger> tester = nullptr;
    shared_ptr<Integrator> modifier = nullptr;

public:
    explicit Step(const unsigned& = 0, const unsigned& = CT_STEP, const double& = 1.);
    virtual ~Step();

    const bool& is_updated() const;

    virtual int initialize();

    virtual int analyze() = 0;

    void set_workshop(const shared_ptr<Workshop>&);
    const shared_ptr<Workshop>& get_workshop() const;

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

    void set_ini_step_size(const double&);
    void set_min_step_size(const double&);
    void set_max_step_size(const double&);
    void set_max_iteration(const unsigned&);

    const double& get_ini_step_size() const;
    const double& get_min_step_size() const;
    const double& get_max_step_size() const;
    const unsigned& get_max_iteration() const;

    const bool& is_fixed_step_size() const;
    void set_fixed_step_size(const bool&);

    const bool& is_symm() const;
    const bool& is_band() const;
    void set_symm(const bool&);
    void set_band(const bool&);
};

#endif

//! @}
