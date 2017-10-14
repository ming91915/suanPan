////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2017 Theodore Chang
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////

#include "ElementTemplate.h"
#include <Domain/DomainBase.h>
#include <Domain/Node.h>
#include <Material/Material2D/Material2D.h>

const unsigned ElementTemplate::m_node = 3;
const unsigned ElementTemplate::m_dof = 2;

/**
 * @brief Need a class tag to identify the type of the object. This attribute is rarely used in the analysis, the global solver is not interested in the type of the element. Hence it is okay to set it to zero, or directly pass zero to Element constructor in following constructor. You can use the class tag to check if the element is the one your want.
 */
#ifndef ET_ELEMENTTEMPLATE
#define ET_ELEMENTTEMPLATE 0
#endif

/**
 * @brief
 * Here we target our ElementTemplate class to fulfill the functionality of a constant stress triangular element, viz., CPS3, in Abaqus notation.
 *
 * The constructor of Element class asks for seven parameters, they are:
 * - Element Object Tag (T)
 * - Element Class Tag (CT)
 * - Number of Nodes (NN)
 * - Number of DoFS (ND)
 * - Node Tags (NT)
 * - Material Tags (MT)
 * - Nonlinear Switch (F)
 *
 * In our example, CT and F will be constants, NN is 3 and ND is 2. So we have three parameters plus thickness for our derived element. Except for initializing private member variables, we do not have to do anything. All other initializations will be handled by the Element and Domain class. As simple as this.
 */
ElementTemplate::ElementTemplate(const unsigned& T, const uvec& NT, const unsigned& MT, const double& TH)
    : Element(T, ET_ELEMENTTEMPLATE, m_node, m_dof, NT, uvec{ MT }, false)
    , thickness(TH) {}

/**
 * @brief
 * As explained before, this method get all necessary information, which includes getting copies of Material objects and other operations, from the Domain object.
 *
 * Please note that **we do not have to check the existance of any objects** which are used in the element. The validity of the connected node objects and the material models is checked before calling initialize(). The excution of this initialize() method means this is a valid Element object.
 *
 * The displacement mode is
 * \f{gather}{\phi=\begin{bmatrix}1&x&y\end{bmatrix}.\f}
 *
 * The strain matrix is calculated as
 * \f{gather}{B=\partial{}N=\partial{}\left(\phi{}C^{-1}\right),\f}
 * where
 * \f{gather}{C=\begin{bmatrix}1&x_i&y_i\\1&x_j&y_j\\1&x_k&y_k\end{bmatrix}.\f}
 *
 * One can also initialize stiffness matrix and/or other build-in matrices from Element class (check the definition for details) in the initialize() method. However, this it not necessary, as the Solver will always call update_status() method with a zero trial displacement to update current stiffness and resistance before running iterations.
 */
void ElementTemplate::initialize(const shared_ptr<DomainBase>& D) {
    //! As CPS3 is a constant stress/strain element, one integration point at the center of the element is enough. Hence we only have one material model defined. First we get a reference of the Material object from the Domain and then call the `get_copy()` method to get a local copy.
    //! Direct assignment is allowed, the move semantics will automatically be invoked.
    m_material = D->get_material(unsigned(material_tag(0)))->get_copy();

    //! The node pointers are handled in the base Element class, we do not have to set it manually. Now we could fill in the `ele_coor` matrix. The area/natural coordinate is another version of implementation. Please refer to FEM textbooks for theories.
    //! This will be used for the computation of the shape function.
    mat ele_coor(m_node, m_node, fill::ones);
    for(auto i = 0; i < m_node; ++i) {
        auto& tmp_coor = node_ptr[i].lock()->get_coordinate();
        for(auto j = 0; j < 2; ++j) ele_coor(i, j + 1) = tmp_coor(j);
    }

    //! The area is half of the determinant of the above matrix.
    area = det(ele_coor) / 2.;

    mat inv_coor = inv(ele_coor);

    //! A standerd way to construct the strain mat is to derive from the partial direvative of the shape function N. For CP3, as it is a constant stress/strain element, the direvatives are constants which can be directly obtained from above matrix.
    strain_mat.zeros(3, m_node * m_dof);
    for(auto i = 0; i < 3; ++i) {
        strain_mat(0, 2 * i) = inv_coor(1, i);
        strain_mat(1, 2 * i + 1) = inv_coor(2, i);
        strain_mat(2, 2 * i) = inv_coor(2, i);
        strain_mat(2, 2 * i + 1) = inv_coor(1, i);
    }

    const auto tmp_density = m_material->get_parameter();
    if(tmp_density != 0.) {
        //! Same as before, a quicker way to obtain shape function.
        vec n = mean(ele_coor) * inv_coor;
        mass = n * n.t() * tmp_density * area * thickness;
    }
}

/**
 * @brief Now we handle the status update method. We get trial displacement via pointers and pass trial strain to the material model. Then get updated stiffness and stress back to form element stiffness and resistance.
 *
 * The pointers of related node objects are stored in a base member `node_ptr`, which is a `std::vector` of `weak_ptr`. (Why `weak_ptr`? To avoid potential wrong deallocation.) To access any methods in Node class, we use
 * ```cpp
 *     auto& my_ptr = node_ptr[i];
 *     my_ptr.lock()->call_any_valid_method();
 * ```
 * Please note that the `weak_ptr` has to be locked to generate a valid shared_ptr before calling any methods.
 *
 * For a static analysis, **stiffness** and **resistance** have to be formulated. Apart from this, there is nothing you have to do. They will be send to global assembler by methods in base Element class, which can also be overridden to be customized.
 */
int ElementTemplate::update_status() {
    vec trial_disp(m_node * m_dof);
    auto idx = 0;
    for(auto i = 0; i < m_node; ++i) {
        auto& tmp_disp = node_ptr[i].lock()->get_trial_displacement();
        for(const auto& j : tmp_disp) trial_disp(idx++) = j;
    }
    m_material->update_trial_status(strain_mat * trial_disp);

    const mat tmp_mat = strain_mat.t() * area * thickness;
    stiffness = tmp_mat * m_material->get_stiffness() * strain_mat;
    resistance = tmp_mat * m_material->get_stress();

    return 0;
}

/**
 * \brief Simply call corresponding methods in material objects. If the Element itself has history variables, they should also be updated/modified in following methods.
 */
int ElementTemplate::commit_status() { return m_material->commit_status(); }

int ElementTemplate::clear_status() { return m_material->clear_status(); }

int ElementTemplate::reset_status() { return m_material->reset_status(); }
