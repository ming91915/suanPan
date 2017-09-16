/*******************************************************************************
 * Copyright (C) 2017 Theodore Chang
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/
/**
 * @class Converger
 * @brief The Converger class handles converger test to indicate if the iteration converges according to various rules.
 *
 * The class stores a pointer `factory` pointed to the Workshop and get information from this Workshop. The `tolerance` and `error` are stored independently so that the Workshop will not be modified.
 *
 * The class further provides a `print_flag` to indicate if the test information should be printed out.
 *
 * @author T
 * @date 15/09/2017
 * @version 0.2.1
 * @file Converger.h
 * @addtogroup Converger
 * @{
 */

#ifndef CONVERGER_H
#define CONVERGER_H

#include "Domain/Tag.h"

class DomainBase;

class Converger : public Tag {
    shared_ptr<DomainBase> database; /**< pointer to DomainBase */

    double tolerance; /**< tolerance */

    unsigned max_iteration; /**< max iteration number */

    const bool print_flag; /**< print flag */

    double error = 0.; /**< current error */

    bool conv_flag = false; /**< converger flag */
public:
    explicit Converger(const unsigned& = 0, const unsigned& = CT_CONVERGER, const shared_ptr<DomainBase>& = nullptr, const double& = 1E-8, const unsigned& = 10, const bool& = false);
    virtual ~Converger();

    virtual int initialize();

    void set_tolerance(const double&);
    const double& get_tolerance() const;

    void set_max_iteration(const unsigned&);
    const unsigned& get_max_iteration() const;

    void set_domain(const shared_ptr<DomainBase>&);
    const shared_ptr<DomainBase>& get_domain() const;

    void set_error(const double&);
    const double& get_error() const;

    void set_conv_flag(const bool&);
    const bool& get_conv_flag() const;

    const bool& if_print() const;

    virtual const bool& if_converged() = 0;
};

#endif

//! @}
