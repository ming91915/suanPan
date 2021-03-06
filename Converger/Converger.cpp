////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2017 Theodore Chang
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////

#include "Converger.h"
#include <Domain/DomainBase.h>

/**
 * \brief The complete constructor.
 * \param T `unique_tag`
 * \param CT `class_tag`
 * \param E `tolerance`
 * \param P `print_flag`
 */
Converger::Converger(const unsigned& T, const unsigned& CT, const double& E, const unsigned& M, const bool& P)
    : Tag(T, CT)
    , tolerance(E)
    , max_iteration(M)
    , print_flag(P) {}

/**
 * \brief The default destructor.
 */
Converger::~Converger() {}

int Converger::initialize() {
    if(database.lock() == nullptr) {
        suanpan_error("initialize() needs a valid Domain.\n");
        return -1;
    }

    return 0;
}

/**
 * \brief Method to set `tolerance`.
 * \param T `tolerance`
 */
void Converger::set_tolerance(const double& T) { tolerance = T; }

/**
 * \brief Method to return `tolerance`.
 * \return `tolerance`
 */
const double& Converger::get_tolerance() const { return tolerance; }

void Converger::set_max_iteration(const unsigned& M) { max_iteration = M; }

const unsigned& Converger::get_max_iteration() const { return max_iteration; }

/**
 * \brief Method to set `DomainBase`.
 * \param D `DomainBase`
 */
void Converger::set_domain(const weak_ptr<DomainBase>& D) {
    if(database.lock() != D.lock()) database = D;
}

/**
 * \brief Method to return `DomainBase`.
 * \return `DomainBase`
 */
const weak_ptr<DomainBase>& Converger::get_domain() const { return database; }

/**
 * \brief Method to set `error`.
 * \param E `error`
 */
void Converger::set_error(const double& E) { error = E; }

/**
 * \brief Method to return `error`.
 * \return `error`
 */
const double& Converger::get_error() const { return error; }

/**
 * \brief Method to set `conv_flag`.
 * \param C `conv_flag`
 */
void Converger::set_conv_flag(const bool& C) { conv_flag = C; }

/**
 * \brief Method to return `conv_flag`.
 * \return `conv_flag`
 */
const bool& Converger::get_conv_flag() const { return conv_flag; }

/**
 * \brief Method to return `print_flag`.
 * \return `print_flag`
 */
const bool& Converger::is_print() const { return print_flag; }

/**
 * \brief Method to return `conv_flag`.
 * \return `conv_flag`
 */
const bool& Converger::is_converged() {
    conv_flag = false;
    return conv_flag;
}
