/**
 * @class Tag
 * @brief A base Tag class.
 *
 * The `Tag` class is a base class which stores the object's `unique_tag` and `class_tag`. Additionally, the Tag class defines status of an object, which is stored in variable `alive`. By testing its value, the object can be removed or added to the global system.
 *
 * @author T
 * @date 27/07/2017
 * @version 0.1.3
 * @file Tag.h
 */

#ifndef TAG_H
#define TAG_H

//! Will be used in derived classes.
#include <suanPan.h>

class Tag {
    bool alive = true; /**< Status flag. */

    unsigned unique_tag = 0; /**< The unique tag of the object. */
    unsigned class_tag = 0;  /**< The tag of the class type. */
public:
    explicit Tag(const unsigned& = 0, const unsigned& = 0);
    virtual ~Tag();

    void set_tag(const unsigned&);
    const unsigned& get_tag() const;

    void set_class_tag(const unsigned&);
    const unsigned& get_class_tag() const;

    void enable();
    void disable();

    const bool& is_active() const;

    virtual void print();
};

#endif
