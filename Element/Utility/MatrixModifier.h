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
 * @fn MatrixModifier
 * @brief The MatrixModifier class.
 *
 * @author T
 * @date 27/10/2017
 * @version 0.1.0
 * @file MatrixModifier.h
 * @addtogroup Utility
 * @ingroup Element
 * @{
 */

#ifndef MATRIXMODIFIER_H
#define MATRIXMODIFIER_H

#include <suanPan.h>
#include <Element/Element.h>
#include <Domain/Factory.hpp>

namespace suanpan
{
    namespace mass
    {
        struct lumped_simple {
            template<typename T>
            static void apply(Mat<T>&);
        };

        template <typename T> void lumped_simple::apply(Mat<T>&mass) {
            mass = diagmat(sum(mass));
        }

        struct lumped_scale {
            template<typename T>
            static void apply(Mat<T>&);
        };

        template <typename T> void lumped_scale::apply(Mat<T>&mass) {
        }
    }
    namespace damping
    {
        struct rayleigh {
            template<typename T>
            static void apply(const shared_ptr<Element>&,const double,const double);
        };

        template <typename T> void rayleigh::apply(const shared_ptr<Element>&element_obj,const double alpha, const double beta) {
            access::rw(element_obj->get_damping()) = alpha*element_obj->get_stiffness() + beta*element_obj->get_mass();
        }
    }
}

#endif

//! @}
