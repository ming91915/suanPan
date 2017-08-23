#include "Convergence.h"
#include <Domain/Domain.h>

/**
 * \brief The complete constructor.
 * \param T `unique_tag`
 * \param CT `class_tag`
 * \param D `domain`
 * \param E `tolerance`
 * \param P `print_flag`
 */
Convergence::Convergence(const unsigned& T,
    const unsigned& CT,
    const shared_ptr<Domain>& D,
    const double& E,
    const bool& P)
    : Tag(T, CT)
    , domain(D)
    , tolerance(E)
    , print_flag(P)
{
}

/**
 * \brief The default destructor.
 */
Convergence::~Convergence() {}

/**
 * \brief Method to set `tolerance`.
 * \param T `tolerance`
 */
void Convergence::set_tolerance(const double& T) { tolerance = T; }

/**
 * \brief Method to return `tolerance`.
 * \return `tolerance`
 */
const double& Convergence::get_tolerance() const { return tolerance; }

/**
* \brief Method to set `domain`.
* \param D `domain`
*/
void Convergence::set_domain(const shared_ptr<Domain>& D) { domain = D; }

/**
* \brief Method to return `domain`.
* \return `domain`
*/
const shared_ptr<Domain>& Convergence::get_domain() const { return domain; }

/**
 * \brief Method to set `error`.
 * \param E `error`
 */
void Convergence::set_error(const double& E) { error = E; }

/**
 * \brief Method to return `error`.
 * \return `error`
 */
const double& Convergence::get_error() const { return error; }

/**
 * \brief Method to set `conv_flag`.
 * \param C `conv_flag`
 */
void Convergence::set_conv_flag(const bool& C) { conv_flag = C; }

/**
 * \brief Method to return `conv_flag`.
 * \return `conv_flag`
 */
const bool& Convergence::get_conv_flag() const { return conv_flag; }

/**
 * \brief Method to return `print_flag`.
 * \return `print_flag`
 */
const bool& Convergence::if_print() const { return print_flag; }

/**
 * \brief Method to return `conv_flag`.
 * \return `conv_flag`
 */
const bool& Convergence::if_converged()
{
    conv_flag = false;
    return conv_flag;
}
