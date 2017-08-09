#include "Tag.h"

/**
 * \brief Default Constructor.
 * \param T Unique Tag
 * \param CT Class Tag
 */
Tag::Tag(const unsigned& T, const unsigned& CT)
    : unique_tag(T)
    , class_tag(CT)
{
}

/**
 * \brief Empty destructor.
 */
Tag::~Tag() {}

/**
 * \brief Method to set `unique_tag`.
 * \param T Unique Tag
 */
void Tag::setTag(const unsigned& T) { unique_tag = T; }

/**
 * \brief Method to return `unique_tag`.
 * \return `unique_tag`
 */
const unsigned& Tag::getTag() const { return unique_tag; }

/**
 * \brief Method to set `class_tag`.
 * \param CT Class Tag
 */
void Tag::setClassTag(const unsigned& CT) { class_tag = CT; }

/**
 * \brief Method to return `class_tag`.
 * \return `class_tag`
 */
const unsigned& Tag::getClassTag() const { return class_tag; }

/**
 * \brief Method to enable an object.
 */
void Tag::enable() { alive = true; }

/**
 * \brief Method to disable one object.
 */
void Tag::disable() { alive = false; }

/**
 * \brief Method to return status.
 * \return `alive`
 */
const bool& Tag::getStatus() const { return alive; }

/**
 * \brief Method to print basic information.
 */
void Tag::print() { printf("A Tagged Object.\n"); }
