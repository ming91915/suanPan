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
 * @class MaterialTemplate
 * @brief A MaterialTemplate class.
 *
 * The Material class defines material models for FEM analysis.
 *
 * The design pattern of this class is slightly different than the one of the Element class. The main one is that, the Material model needs to be copied for Element local use while the Element object does not need to be duplicated.
 *
 * As suanPan uses smart pointers for dynamic memory management, the copy of a Material prototype is defined as a unique pointer so that at Element level, there is no need to handle the lifetime of the Material object. Although the `new` way is still possible, there is a danger of memory leak like dangling pointers.
 *
 * The Material class itself defines a couple of member variables. Currently, strain, stress, strain rate, general purpose history variables and stiffness quantities for both current status and trial status are supported.
 *
 * Correspondingly, there are getters to return the trial status of the Material model. They are
 * ```cpp
 * virtual const vec& get_strain() const;
 * virtual const vec& get_strain_rate() const;
 * virtual const vec& get_stress() const;
 * virtual const mat& get_stiffness() const;
 * virtual const mat& get_initial_stiffness() const;
 * ```
 * There is no need to override those methods if one decides to use pre-defined status variables. As defined as virtual functions, the users are free to ignore all pre-defined variables and define their own status variables. In that case, the above methods should be overridden.
 *
 * Similar to other classes, there is a initialize method to allow the user to initialize the object without creating a new object.
 * ```cpp
 * virtual void initialize() = 0;
 * ```
 * This should be overridden in derived class to initialize the status variables. If your Material model does not need to re-initialize, it is okay to place all initializations inside of the constructors.
 *
 * The method that is in charge of returning a copy is called
 * ```cpp
 * virtual unique_ptr<Material> get_copy() = 0;
 * ```
 * By calling the copy constructor, this method can be implemented in a very concise way.
 * ```cpp
 * unique_ptr<Material> get_copy() { return
 * make_unique<MaterialName>(*this); };
 * ```
 * According to the rule of three or the rule of five, since here the method calls the default copy constructor of a derived class, if the derived class is so complicated that the default constructor cannot handle, it is necessary to define the copy/assignment (move) constructors and the destructor.
 *
 * The Material class defines four updating method.
 * ```cpp
 * virtual int update_incre_status(const vec&); // incre_strain
 * virtual int update_incre_status(const vec&, const vec&); // incre_strain incre_strain_rate
 * virtual int update_trial_status(const vec&); // trial_strain
 * virtual int update_trial_status(const vec&, const vec&); // trial_strain trial_strain_rate
 * ```
 * As the Material model only talks to Element objects, the developers are free to implement any of them, even all four methods, providing that the corresponding Element calls the proper method.
 *
 * The other status control methods are also need to be implemented.
 * ```cpp
 * virtual int clear_status() = 0;
 * virtual int commit_status() = 0;
 * virtual int reset_status() = 0;
 * ```
 *
 * There is an additional method called
 * ```cpp
 * virtual double get_parameter(const unsigned& = 0) const;
 * ```
 * This is universal method to return material properties based on the input argument, as some Element objects may need some material properties to compute particular data.
 *
 * @author T
 * @date 05/09/2017
 * @version 0.1.1
 * @file MaterialTemplate.h
 * @addtogroup Material
 * @{
 */

#ifndef MaterialTemplate_H
#define MaterialTemplate_H

#include "Material.h"

class MaterialTemplate : public Material {
public:
    explicit MaterialTemplate(const unsigned& = 0);

    void initialize() override;

    double get_parameter(const unsigned& = 0) const override;

    unique_ptr<Material> get_copy() override;

    int update_trial_status(const vec&) override;

    int clear_status() override;
    int commit_status() override;
    int reset_status() override;
};

#endif

//! @}
