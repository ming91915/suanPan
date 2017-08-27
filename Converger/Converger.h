/**
* @class Converger
* @brief The Converger class handles converger test to indicate if the iteration
* converges according to various rules.
*
* The class stores a pointer `factory` pointed to the Workroom and get information from
* this Workroom. The `tolerance` and `error` are stored independently so that the
* Workroom will not be modified.
*
* The class further provides a `print_flag` to indicate if the test information should be
* printed out.
*
* @author T
* @date 25/08/2017
* @version 0.2.1
* @file Converger.h
* @addtogroup Converger
* @{
*/

#ifndef CONVERGER_H
#define CONVERGER_H

#include "Domain/Tag.h"

class Domain;

class Converger : public Tag
{
    shared_ptr<Domain> database; /**< pointer to Domain */

    double tolerance; /**< tolerance */

    unsigned max_iteration; /**< max iteration number */

    bool print_flag; /**< print flag */

    double error = 0.; /**< current error */

    bool conv_flag = false; /**< converger flag */

    bool initialized = false;

public:
    explicit Converger(const unsigned& = 0,
        const unsigned& = CT_CONVERGER,
        const shared_ptr<Domain>& = nullptr,
        const double& = 1E-8,
        const unsigned& = 10,
        const bool& = false);
    virtual ~Converger();

    const bool& is_initialized() const;

    virtual int initialize();

    void set_tolerance(const double&);
    const double& get_tolerance() const;

    void set_max_iteration(const unsigned&);
    const unsigned& get_max_iteration() const;

    void set_domain(const shared_ptr<Domain>&);
    const shared_ptr<Domain>& get_domain() const;

    void set_error(const double&);
    const double& get_error() const;

    void set_conv_flag(const bool&);
    const bool& get_conv_flag() const;

    const bool& if_print() const;

    virtual const bool& if_converged() = 0;
};

#endif

//! @}
