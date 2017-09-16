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
 * @class Database
 * @brief A Database class is a top level container.
 * @author T
 * @date 27/08/2017
 * @version 0.2.1
 * @file Database.h
 */

#ifndef DATABASE_H
#define DATABASE_H

#include "Domain/Domain.h"

class Database : public Tag {
    shared_ptr<Domain> domain;

public:
    explicit Database(const unsigned& = 0, const unsigned& = 0);
    virtual ~Database();

    void setDomain(const shared_ptr<Domain>& D);
    const shared_ptr<Domain>& getDomain() const;

    virtual int save(const char*) = 0;
};

#endif
