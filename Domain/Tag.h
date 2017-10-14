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

// ReSharper disable once CppUnusedIncludeDirective
#include <suanPan.h> // for derived classes

class Tag {
    bool alive = true; /**< Status flag. */

    const unsigned unique_tag; /**< The unique tag of the object. */
    const unsigned class_tag;  /**< The tag of the class type. */
public:
    explicit Tag(const unsigned& = 0, const unsigned& = 0);
    virtual ~Tag();

    void set_tag(const unsigned&) const;
    const unsigned& get_tag() const;

    void set_class_tag(const unsigned&) const;
    const unsigned& get_class_tag() const;

    void enable();
    void disable();

    const bool& is_active() const;

    virtual void print();
};

#endif
