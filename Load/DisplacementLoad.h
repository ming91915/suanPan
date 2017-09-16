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
#ifndef DISPLACEMENTLOAD_H
#define DISPLACEMENTLOAD_H

#include "Load.h"

class DisplacementLoad : public Load {
    double pattern = 0.;

    uvec nodes;
    uvec dofs;

public:
    explicit DisplacementLoad(const unsigned& = 0, const unsigned& = 0, const double& = 0., const uvec& = {}, const unsigned& = 0, const shared_ptr<Amplitude>& = nullptr);
    DisplacementLoad(const unsigned&, const unsigned&, const double&, const uvec&, const uvec&, const shared_ptr<Amplitude>& = nullptr);

    int process(const shared_ptr<DomainBase>&) override;
};

#endif // DISPLACEMENTLOAD_H
