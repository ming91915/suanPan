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
 * @class Section
 * @brief A Section class.
 * @author T
 * @date 27/07/2017
 * @version 0.1.0
 * @file Section.h
 */

#ifndef SECTION_H
#define SECTION_H

#include <Domain/DomainBase.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <Material/Material.h> // for derived class

class Section : public Tag {
protected:
    const unsigned material_tag; /**< material tags */

    vec resistance;        /**< resistance vector. */
    mat stiffness;         /**< stiffness matrix */
    mat initial_stiffness; /**< initial stiffness matrix */
public:
    explicit Section(const unsigned& T = 0, const unsigned& CT = CT_SECTION, const unsigned& MT = 0)
        : Tag(T, CT)
        , material_tag(MT) {}

    virtual ~Section() {}

    virtual void initialize(const shared_ptr<DomainBase>&) = 0;

    virtual const vec& get_resistance() const;
    virtual const mat& get_stiffness() const;
    virtual const mat& get_initial_stiffness() const;

    virtual unique_ptr<Section> get_copy() = 0;

    virtual int update_trial_status(const vec&);
    virtual int update_trial_status(const vec&, const vec&);
    virtual int clear_status() = 0;
    virtual int commit_status() = 0;
    virtual int reset_status() = 0;
};

#endif
