/**
* @class Convergence
* @brief The Convergence class handles convergence test to indicate if the iteration
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
* @date 08/08/2017
* @version 0.2.1
* @file Convergence.h
* @addtogroup Convergence
* @{
*/

#ifndef CONVERGENCE_H
#define CONVERGENCE_H

#include "Domain/Tag.h"

class Domain;

class Convergence : public Tag
{
    shared_ptr<Domain> database = nullptr; /**< pointer to Domain */

    double tolerance; /**< tolerance */

    unsigned max_iteration;

    bool print_flag = false; /**< print flag */

    double error = 0.; /**< current error */

    bool conv_flag = false; /**< convergence flag */
public:
    explicit Convergence(const unsigned& = 0,
        const unsigned& = CT_CONVERGENCE,
        const shared_ptr<Domain>& = nullptr,
        const double& = 1E-8,
        const unsigned& = 7,
        const bool& = false);
    virtual ~Convergence();

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
