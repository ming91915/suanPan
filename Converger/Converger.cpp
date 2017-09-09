#include "Converger.h"
#include <Domain/Domain.h>

/**
 * \brief The complete constructor.
 * \param T `unique_tag`
 * \param CT `class_tag`
 * \param D `database`
 * \param E `tolerance`
 * \param P `print_flag`
 */
Converger::Converger(const unsigned& T, const unsigned& CT, const shared_ptr<Domain>& D, const double& E, const unsigned& M, const bool& P)
    : Tag(T, CT)
    , database(D)
    , tolerance(E)
    , max_iteration(M)
    , print_flag(P) {}

/**
 * \brief The default destructor.
 */
Converger::~Converger() {}

const bool& Converger::is_initialized() const { return initialized; }

int Converger::initialize() {
    if(!initialized) initialized = true;

    if(database == nullptr) {
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
 * \brief Method to set `domain`.
 * \param D `domain`
 */
void Converger::set_domain(const shared_ptr<Domain>& D) { database = D; }

/**
 * \brief Method to return `domain`.
 * \return `domain`
 */
const shared_ptr<Domain>& Converger::get_domain() const { return database; }

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
const bool& Converger::if_print() const { return print_flag; }

/**
 * \brief Method to return `conv_flag`.
 * \return `conv_flag`
 */
const bool& Converger::if_converged() {
    conv_flag = false;
    return conv_flag;
}
