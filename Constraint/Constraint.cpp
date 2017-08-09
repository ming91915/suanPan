#include "Constraint.h"
#include <Domain/Domain.h>

/**
 * \brief The default constructor.
 * \param T `unique_tag`
 * \param CT `class_tag`
 */
Constraint::Constraint(const unsigned& T, const unsigned& CT, const unsigned& ST)
    : Tag(T, CT)
    , step_tag(ST)
{
}

/**
 * \brief The default destructor.
 */
Constraint::~Constraint()
{
#ifdef SUANPAN_DEBUG
    printf("Constraint %u dtor() Called.\n", getTag());
#endif
}

/**
 * \brief Method to set `step_tag`.
 * \param ST `step_tag`
 */
void Constraint::setStepTag(const unsigned& ST) { step_tag = ST; }

/**
 * \brief Method to get `step_tag`.
 * \return `step_tag`
 */
const unsigned& Constraint::getStepTag() const { return step_tag; }

/**
 * \brief Method to apply the Constraint to the system.
 * \return Number of DoFs Modified
 */
int Constraint::process(const shared_ptr<Domain>&) { return -1; }
