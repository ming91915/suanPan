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
 * @class Load
 * @brief A Load class.
 *
 * The Load class is in charge of returning load level according to given time increment.
 *
 * @author T
 * @date 05/09/2017
 * @version 0.1.0
 * @file Load.h
 * @addtogroup Load
 * @{
 */

#ifndef LOAD_H
#define LOAD_H

#include <Domain/Tag.h>

class Amplitude;
class DomainBase;

class Load : public Tag {
protected:
    static const double multiplier;

    unsigned step_tag, amplitude_tag;

    uvec nodes, dofs;

    double pattern = 0.;

    shared_ptr<Amplitude> magnitude;

public:
    explicit Load(const unsigned& = 0, const unsigned& = CT_LOAD, const unsigned& = 0, const unsigned& = 0, const uvec& = {}, const uvec& = {}, const double& = 0.);
    virtual ~Load();

    virtual int initialize(const shared_ptr<DomainBase>&);

    virtual int process(const shared_ptr<DomainBase>&) = 0;

    void set_step_tag(const unsigned&);
    const unsigned& get_step_tag() const;
};

#endif

//! @}
