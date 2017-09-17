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
 * @class Constraint
 * @brief A Constraint class.
 *
 * The Constraint class.
 *
 * @author T
 * @date 03/07/2017
 * @file Constraint.h
 * @addtogroup Constraint
 * @{
 */

#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include <Domain/Tag.h>

class DomainBase;

class Constraint : public Tag {
    unsigned step_tag = 0; /**< step tag */
protected:
    static const double multiplier;

public:
    explicit Constraint(const unsigned& = 0, const unsigned& = CT_CONSTRAINT, const unsigned& = 0);
    virtual ~Constraint();

    void set_step_tag(const unsigned&);
    const unsigned& get_step_tag() const;

    virtual int process(const shared_ptr<DomainBase>&) = 0;
};

#endif

//! @}
