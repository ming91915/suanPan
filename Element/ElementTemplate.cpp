#include "ElementTemplate.h"

const unsigned ElementTemplate::m_node = 3;
const unsigned ElementTemplate::m_dof = 2;

#ifndef ET_ELEMENTTEMPLATE
#define ET_ELEMENTTEMPLATE 0
#endif

/**
* @brief
* Here we target our ElementTemplate class to fulfill the functionality of a constant
* stress triangular element, viz., CPS3 in Abaqus notation.
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
* In our example, CT and F will be constants, NN is 3 and ND is 2. So we have three
* parameters plus thickness for our derived element. Except for initializing private
* member variables. We do not have to do anything. All other initializations will be
* handled by the Element and Domain classs. As simple as this.
*/
ElementTemplate::ElementTemplate(const unsigned& T,
    const uvec& NT,
    const unsigned& MT,
    const double& TH)
    : Element(T, ET_ELEMENTTEMPLATE, m_node, m_dof, NT, { MT }, false)
    , thickness(TH)
{
}

/**
 * @brief
 * As explained before, this method get all necessary information, which includes getting
 * copies of Material objects and other operations, from the Domain object.
 *
 * The displacement mode is
 * \f{gather}{\phi=\begin{bmatrix}1&x&y\end{bmatrix}.\f}
 *
 * The strain matrix is calculated as
 * \f{gather}{B=\partial{}N=\partial{}\left(\phi{}C^{-1}\right),\f}
 * where
 * \f{gather}{C=\begin{bmatrix}1&x_i&y_i\\1&x_j&y_j\\1&x_k&y_k\end{bmatrix}.\f}
 */
void ElementTemplate::initialize(const shared_ptr<Domain>& D)
{
    //! As CPS3 is a constant stress/strain element, one integration point at the center
    //! of the element is enough. Hence we only have one material model defined. First we
    //! get a reference of the Material object from the Domain and then call the
    //! `getCopy()` method to get a local copy.
    auto& material_proto = D->getMaterial(static_cast<unsigned>(material_tag(0)));
    //! Direct assignment is allowed, the move semantics will automatically be invoked.
    m_material = material_proto->getCopy();

    //! The node pointers are handled in the base Element class, we do not have to set it
    //! manually. Now we could fill in the `ele_coor` matrix. The area/natural coordinate
    //! is another version of implementation. Please refer to FEM textbooks for theories.
    //! This will be used for the computation of the shape function.
    mat ele_coor(m_node, m_node, fill::ones);
    for(auto I = 0; I < m_node; ++I) {
        auto& tmp_coor = node_ptr.at(I).lock()->getCoordinate();
        for(auto J = 0; J < 2; ++J) ele_coor(I, J + 1) = tmp_coor(J);
    }

    //! The area is half of the determinant of the above matrix.
    area = det(ele_coor) / 2.;

    mat inv_coor = inv(ele_coor);

    strain_mat.zeros(3, m_node * m_dof);
    strain_mat(0, 0) = inv_coor(1, 0);
    strain_mat(1, 1) = inv_coor(2, 0);
    strain_mat(2, 0) = inv_coor(2, 0);
    strain_mat(2, 1) = inv_coor(1, 0);

    strain_mat(0, 2) = inv_coor(1, 1);
    strain_mat(1, 3) = inv_coor(2, 1);
    strain_mat(2, 2) = inv_coor(2, 1);
    strain_mat(2, 3) = inv_coor(1, 1);

    strain_mat(0, 4) = inv_coor(1, 2);
    strain_mat(1, 5) = inv_coor(2, 2);
    strain_mat(2, 4) = inv_coor(2, 2);
    strain_mat(2, 5) = inv_coor(1, 2);

    auto tmp_density = m_material->getParameter();
    if(tmp_density != 0.) {
        tmp_density *= area * thickness;
        vec n = mean(ele_coor) * inv_coor;
        auto tmp_a = n(0) * n(0) * tmp_density;
        auto tmp_b = n(1) * n(1) * tmp_density;
        auto tmp_c = n(2) * n(2) * tmp_density;
        auto tmp_d = n(0) * n(1) * tmp_density;
        auto tmp_e = n(1) * n(2) * tmp_density;
        auto tmp_f = n(2) * n(0) * tmp_density;
        mass(0, 0) = tmp_a;
        mass(1, 1) = tmp_a;
        mass(2, 2) = tmp_b;
        mass(3, 3) = tmp_b;
        mass(4, 4) = tmp_c;
        mass(5, 5) = tmp_c;
        mass(0, 2) = tmp_d;
        mass(1, 3) = tmp_d;
        mass(2, 0) = tmp_d;
        mass(3, 1) = tmp_d;
        mass(0, 4) = tmp_f;
        mass(1, 5) = tmp_f;
        mass(4, 0) = tmp_f;
        mass(5, 1) = tmp_f;
        mass(2, 4) = tmp_e;
        mass(3, 5) = tmp_e;
        mass(4, 2) = tmp_e;
        mass(5, 3) = tmp_e;
    }
}

/**
* @brief
* Now we handle the status update method. We get trial displacement via pointers and pass
* trail strain to the material model. Then get updated stiffness and stress back to form
* element stiffness and resistance.
*/
int ElementTemplate::updateStatus()
{
    vec trial_disp(m_node * m_dof);
    auto idx = 0;
    for(auto I = 0; I < m_node; ++I) {
        auto& tmp_disp = node_ptr.at(I).lock()->getTrialDisplacement();
        for(const auto& J : tmp_disp) trial_disp(idx++) = J;
    }
    m_material->updateTrialStatus(strain_mat * trial_disp);

    stiffness =
        strain_mat.t() * m_material->getStiffness() * strain_mat * area * thickness;
    resistance = strain_mat.t() * m_material->getStress() * area * thickness;

    return 0;
}

int ElementTemplate::commitStatus() { return m_material->commitStatus(); }

int ElementTemplate::clearStatus() { return m_material->clearStatus(); }

int ElementTemplate::resetStatus() { return m_material->resetStatus(); }
